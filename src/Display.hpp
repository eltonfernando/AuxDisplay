#pragma once
#include <cstdint>
#include <mutex>
#include <vector>
#include "Image.hpp"

class AuxDisplay {
   public:
    enum class Command : uint8_t {
        RESET           = 101,
        CLEAR           = 102,
        TO_BLACK        = 103,
        SCREEN_OFF      = 108,
        SCREEN_ON       = 109,
        SET_BRIGHTNESS  = 110,
        SET_ORIENTATION = 121,
        DISPLAY_BITMAP  = 197,
        HELLO           = 69
    };

    bool sendCommand(int fd, Command cmd, int x, int y, int ex, int ey);
    bool sendLine(int fd, const std::vector<uint8_t>& data);

    void displayImage(int fd, const Image& img, int x = 0, int y = 0, int imageWidth = 0, int imageHeight = 0);

    int getWidth() const noexcept {
        return 240;
    }  // exemplo
    int getHeight() const noexcept {
        return 320;
    }  // exemplo

   private:
    std::mutex updateQueueMutex;
};
