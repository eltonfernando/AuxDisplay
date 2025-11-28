#pragma once
#include <string>
#include <optional>
#include <libudev.h>
#include <vector>
#include <unistd.h>
#include <cstdint> 
#include <iostream>
class AuxDisplay {
public:
enum class Command : uint8_t {
        RESET = 101,
        CLEAR = 102,
        TO_BLACK = 103,
        SCREEN_OFF = 108,
        SCREEN_ON = 109,
        SET_BRIGHTNESS = 110,
        SET_ORIENTATION = 121,
        DISPLAY_BITMAP = 197,
        HELLO = 69};
    bool SendCommand(int fd, Command cmd, int x, int y, int ex, int ey);
    };
    