#pragma once
#include "Display.hpp"

bool AuxDisplay::SendCommand(int fd, AuxDisplay::Command cmd, int x, int y, int ex, int ey) {
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

        std::cout << "Comando enviado: " << static_cast<int>(cmd)
                  << " Bytes: ";
        for (int i = 0; i < 6; ++i)
            std::cout << std::hex << (int)byteBuffer[i] << " ";
        std::cout << std::dec << "\n";

        return true;
    };

    