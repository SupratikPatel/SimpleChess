#include "mainwindow.h"

#include <QVBoxLayout>

#include "game.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setFixedSize(1400, 900);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    m_game = new Game(this);
    layout->addWidget(m_game);
    m_game->setFocus(); // Set focus on the game widget

    m_game->displayMainMenu();
}

MainWindow::~MainWindow() {
}
