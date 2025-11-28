#include "src/MainWindow.h"
#include "src/Serial.hpp"
#include "src/Display.hpp"
#include <QApplication>
#include <iostream>
int main(int argc, char *argv[]) {
    SerialDetector detector;
    AuxDisplay display;
    auto device = detector.connect();
if (!device) {
    std::cout << "Display não encontrado\n";
} else {
    std::cout << "Conectado em: " << device->devnode << "\n";

    
    display.SendCommand(device->fd, AuxDisplay::Command::RESET, 0, 0, 0, 0);
    display.SendCommand(device->fd, AuxDisplay::Command::SCREEN_ON, 0, 0, 0, 0);   
    }
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    app.exec();
    display.SendCommand(device->fd, AuxDisplay::Command::SCREEN_OFF, 0, 0, 0, 0);
    return 0;
};