#include <QApplication>
#include <fstream>
#include <iostream>
#include <string>
#include "src/Display.hpp"
#include "src/MainWindow.h"
#include "src/Serial.hpp"
int main(int argc, char* argv[]) {
    SerialDetector detector;
    AuxDisplay display;
    auto device = detector.connect();
    if (!device) {
        std::cout << "Display não encontrado\n";
    } else {
        std::cout << "Conectado em: " << device->devnode << "\n";

        // std::ifstream cpuinfo("/proc/cpuinfo");
        // std::string line;
        // while (std::getline(cpuinfo, line)) {
        //     std::cout << line << "\n";
        // }
        display.sendCommand(device->fd, AuxDisplay::Command::RESET, 0, 0, 0, 0);
        display.sendCommand(device->fd, AuxDisplay::Command::SCREEN_ON, 0, 0, 0, 0);
    }
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    app.exec();
    display.sendCommand(device->fd, AuxDisplay::Command::SCREEN_OFF, 0, 0, 0, 0);
    return 0;
};
