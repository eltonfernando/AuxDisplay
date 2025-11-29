#include "Serial.hpp"
#include <fcntl.h>
#include <poll.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <vector>
std::optional<SerialDetector::DeviceInfo> SerialDetector::findDisplay() const {
    udev* udev_ctx = udev_new();
    if (!udev_ctx) {
        return std::nullopt;
    }

    udev_enumerate* enumerate = udev_enumerate_new(udev_ctx);
    udev_enumerate_add_match_subsystem(enumerate, "tty");
    udev_enumerate_scan_devices(enumerate);

    udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
    udev_list_entry* entry;

    udev_list_entry_foreach(entry, devices) {
        const char* syspath = udev_list_entry_get_name(entry);
        udev_device* dev    = udev_device_new_from_syspath(udev_ctx, syspath);

        const char* devnode = udev_device_get_devnode(dev);
        if (!devnode) continue;

        udev_device* parent = udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");

        if (!parent) {
            udev_device_unref(dev);
            continue;
        }

        const char* vid = udev_device_get_sysattr_value(parent, "idVendor");
        const char* pid = udev_device_get_sysattr_value(parent, "idProduct");

        if (vid && pid && std::string(vid) == TARGET_VID && std::string(pid) == TARGET_PID) {
            const char* serial = udev_device_get_sysattr_value(parent, "serial");

            DeviceInfo info{.devnode = devnode, .vid = vid, .pid = pid, .serial = serial ? serial : ""};

            udev_device_unref(dev);
            udev_enumerate_unref(enumerate);
            udev_unref(udev_ctx);
            std::cout << "Display encontrado em: " << info.devnode << "\n"
                      << "VID: " << info.vid << "\n"
                      << "PID: " << info.pid << "\n"
                      << "Serial: " << info.serial << "\n";

            return info;
        }

        udev_device_unref(dev);
    }

    udev_enumerate_unref(enumerate);
    udev_unref(udev_ctx);
    return std::nullopt;
}

std::optional<SerialDetector::DeviceInfo> SerialDetector::connect() const {
    auto dev = findDisplay();
    if (!dev) return std::nullopt;

    int fd = open(dev->devnode.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) return std::nullopt;

    termios tty{};
    tcgetattr(fd, &tty);

    cfsetispeed(&tty, B230400);
    cfsetospeed(&tty, B230400);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 0;

    tcsetattr(fd, TCSANOW, &tty);

    dev->fd = fd;
    return dev;
}

bool SerialDetector::writeBytes(int fd, const std::vector<uint8_t>& data) const {
    if (fd < 0) return false;
    if (data.empty()) return true;

    ssize_t sent = ::write(fd, data.data(), data.size());

    return (sent == static_cast<ssize_t>(data.size()));
}

std::vector<uint8_t> SerialDetector::readResponse(int fd, int timeout_ms) const {
    std::vector<uint8_t> buffer;
    uint8_t temp[256];

    pollfd p{};
    p.fd     = fd;
    p.events = POLLIN;

    int ret = poll(&p, 1, timeout_ms);
    if (ret <= 0) {
        return buffer;  // vazio = sem resposta
    }

    ssize_t n = ::read(fd, temp, sizeof(temp));
    if (n > 0) {
        buffer.insert(buffer.end(), temp, temp + n);
    }

    return buffer;
}
