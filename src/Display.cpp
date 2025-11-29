
#include "Display.hpp"
#include <unistd.h>
#include <cstdint>
#include <iostream>

bool AuxDisplay::sendCommand(int fd, AuxDisplay::Command cmd, int x, int y, int ex, int ey) {
    uint8_t byteBuffer[6];
    byteBuffer[0] = static_cast<uint8_t>(x >> 2);
    byteBuffer[1] = static_cast<uint8_t>(((x & 3) << 6) + (y >> 4));
    byteBuffer[2] = static_cast<uint8_t>(((y & 15) << 4) + (ex >> 6));
    byteBuffer[3] = static_cast<uint8_t>(((ex & 63) << 2) + (ey >> 8));
    byteBuffer[4] = static_cast<uint8_t>(ey & 0xFF);
    byteBuffer[5] = static_cast<uint8_t>(cmd);

    ssize_t sent = write(fd, byteBuffer, 6);
    if (sent != 6) {
        std::cerr << "Erro ao enviar comando, bytes enviados: " << sent << "\n";
        return false;
    }

    std::cout << "Comando enviado: " << static_cast<int>(cmd) << " Bytes: ";
    for (int i = 0; i < 6; ++i) std::cout << std::hex << (int)byteBuffer[i] << " ";
    std::cout << std::dec << "\n";

    return true;
};
bool AuxDisplay::sendLine(int fd, const std::vector<uint8_t>& line) {
    if (fd < 0) return false;

    ssize_t sent = write(fd, line.data(), line.size());
    return sent == (ssize_t)line.size();
}
void AuxDisplay::displayImage(int fd, const Image& image, int x, int y, int imageWidth, int imageHeight) {
    const int W = getWidth();
    const int H = getHeight();

    int srcW = image.getWidth();
    int srcH = image.getHeight();

    if (imageWidth == 0) imageWidth = srcW;
    if (imageHeight == 0) imageHeight = srcH;

    if (x + imageWidth > W) imageWidth = W - x;
    if (y + imageHeight > H) imageHeight = H - y;

    if (imageWidth <= 0 || imageHeight <= 0) throw std::runtime_error("displayImage: dimensões inválidas");

    Image cropped = (imageWidth == srcW && imageHeight == srcH) ? image : image.crop(0, 0, imageWidth, imageHeight);

    int x0 = x;
    int y0 = y;
    int x1 = x + imageWidth - 1;
    int y1 = y + imageHeight - 1;

    auto rgb565 = image_to_RGB565(cropped, Endianness::Little);

    sendCommand(/*fd=*/1, Command::DISPLAY_BITMAP, x0, y0, x1, y1);

    std::lock_guard<std::mutex> lock(updateQueueMutex);

    const size_t chunkSize = size_t(W) * 8;
    for (size_t i = 0; i < rgb565.size(); i += chunkSize) {
        size_t end = std::min(i + chunkSize, rgb565.size());
        sendLine(fd, std::vector<uint8_t>(rgb565.begin() + i, rgb565.begin() + end));
    }
}
