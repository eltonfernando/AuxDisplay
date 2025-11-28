#pragma once
#include <string>
#include <optional>
#include <libudev.h>
#include <vector>
#include <unistd.h>
#include <cstdint> 
#include <iostream>
class SerialDetector {
public:

    struct DeviceInfo {
        std::string devnode;
        std::string vid;
        std::string pid;
        std::string serial;
        int fd{-1};
    };

    std::optional<DeviceInfo> findDisplay() const;
    std::optional<DeviceInfo> connect() const;
    bool writeBytes(int fd, const std::vector<uint8_t>& data) const;
    std::vector<uint8_t> readResponse(int fd, int timeout_ms = 200) const;
    


private:
    static constexpr const char* TARGET_VID = "1a86";
    static constexpr const char* TARGET_PID = "5722";
};
