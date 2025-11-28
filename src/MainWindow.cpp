#include "MainWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), statusLabel_(nullptr) {
    setupUi();
    setupConnections();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi() {
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    statusLabel_ = new QLabel(tr("AuxDesk — auxiliary display app"), central);
    layout->addWidget(statusLabel_);

    setCentralWidget(central);
    setWindowTitle(tr("AuxDesk"));
    resize(480, 272); // exemplo para mini display
}

void MainWindow::setupConnections() {
    // conecta sinais/slots aqui (exemplo mínimo)
}
