#include "game.h"

#include <QApplication>
#include <QGraphicsTextItem>
#include <QMouseEvent>

#include "button.h"
#include "king.h"
#include "box.h"
#include "queen.h"
#include "pawn.h"

Game::Game(QWidget* parent) : QGraphicsView(parent) {
    m_gameScene = new QGraphicsScene();
    m_gameScene->setSceneRect(0, 0, 1400, 900);
    setFixedSize(1400, 900);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(m_gameScene);

    setBackgroundBrush(QBrush(Qt::black));
    pieceToMove = nullptr;

    m_turnDisplay = new QGraphicsTextItem();
    m_turnDisplay->setPos(width() / 2 - 100, 10);
    m_turnDisplay->setZValue(1);
    m_turnDisplay->setDefaultTextColor(Qt::white);
    m_turnDisplay->setFont(QFont("", 18));
    m_turnDisplay->setPlainText("Turn : WHITE");

    check = new QGraphicsTextItem();
    check->setPos(width() / 2 - 100, 860);
    check->setZValue(4);
    check->setDefaultTextColor(Qt::red);
    check->setFont(QFont("", 18));
    check->setPlainText("CHECK!");
    check->setVisible(false);

    setTurn("WHITE");
}

void Game::drawChessBoard() {
    m_chess = new ChessBoard();
    drawDeadHolder(0, 0, Qt::lightGray);
    drawDeadHolder(1100, 0, Qt::lightGray);
    m_chess->drawBoxes(width() / 2 - 400, 50);
}

void Game::displayDeadPieces(const QList<ChessPiece*>& deadPieces,
    bool isWhite) {
    int SHIFT = 50;
    int j = 0;
    int k = 0;
    int baseX = isWhite ? 40 : 1140;
    for (ChessPiece* piece : deadPieces) {
        if (j == 4) {
            k++;
            j = 0;
        }
        piece->setPos(baseX + SHIFT * j++, 100 + SHIFT * 2 * k);
    }
}

void Game::placeInDeadPlace(ChessPiece* piece) {
    if (piece->getSide() == "WHITE") {
        m_whiteDead.append(piece);
        displayDeadPieces(m_whiteDead, true);
    } else {
        m_blackDead.append(piece);
        displayDeadPieces(m_blackDead, false);
    }
    alivePieces.removeAll(piece);
    if (dynamic_cast<King*>(piece)) {
        check->setPlainText(piece->getSide() == "WHITE" ? "Black Won"
                                                        : "White Won");
        gameOver();
    }
}

void Game::addToScene(QGraphicsItem* item) {
    m_gameScene->addItem(item);
}

void Game::removeFromScene(QGraphicsItem* item) {
    m_gameScene->removeItem(item);
}

void Game::setTurn(const QString& value) {
    m_turn = value;
    m_turnDisplay->setPlainText("Turn =>" + m_turn);
}

void Game::changeTurn() {
    setTurn(m_turn == "WHITE" ? "BLACK" : "WHITE");
}

void Game::mousePressEvent(QMouseEvent* event) {
    if (m_isPaused) return;

    QGraphicsView::mousePressEvent(event);
    QPoint point = event->pos();
    QGraphicsItem* item = itemAt(point);
    if (item) {
        ChessBox* clickedBox = dynamic_cast<ChessBox*>(item);
        if (clickedBox) {
            if (pieceToMove) {
                King* king = dynamic_cast<King*>(pieceToMove);
                if (king && abs(king->getCurrentBox()->colLoc - clickedBox->colLoc) == 2) {
                    // This is a castling move
                    bool kingSide = clickedBox->colLoc > king->getCurrentBox()->colLoc;
                    king->castle(kingSide);
                    changeTurn();
                    pieceToMove = nullptr;
                } else if (pieceToMove->moveLocation().contains(clickedBox)) {
                    // Regular move
                    if (clickedBox->getHasChessPiece()) {
                        placeInDeadPlace(clickedBox->currentPiece);
                    }
                    clickedBox->placePiece(pieceToMove);
                    pieceToMove->getCurrentBox()->setHasChessPiece(false);
                    pieceToMove->getCurrentBox()->currentPiece = nullptr;
                    pieceToMove->setCurrentBox(clickedBox);
                    pieceToMove->firstMove = false;
                    changeTurn();
                    pieceToMove = nullptr;
                }
                // Decolor all boxes after move
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        collection[i][j]->resetOriginalColor();
                    }
                }
            } else if (clickedBox->getHasChessPiece() && clickedBox->currentPiece->getSide() == getTurn()) {
                pieceToMove = clickedBox->currentPiece;
                pieceToMove->moves();
            }
        }

        QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMousePress);
        mouseEvent.setButton(event->button());
        mouseEvent.setPos(mapToScene(point));
        QApplication::sendEvent(scene(), &mouseEvent);
    }
}

void Game::start() {
    for (QGraphicsItem* item : m_menuItems) {
        removeFromScene(item);
    }
    m_menuItems.clear();
    addToScene(m_turnDisplay);

    QGraphicsTextItem* whitePiece = new QGraphicsTextItem("Black's Killings");
    whitePiece->setPos(70, 10);
    whitePiece->setDefaultTextColor(Qt::darkBlue);
    whitePiece->setFont(QFont("", 14));
    addToScene(whitePiece);

    QGraphicsTextItem* blackPiece = new QGraphicsTextItem("White's Killings");
    blackPiece->setPos(1170, 10);
    blackPiece->setDefaultTextColor(Qt::darkBlue);
    blackPiece->setFont(QFont("", 14));
    addToScene(blackPiece);

    addToScene(check);
    m_chess->addChessPiece();

    m_isPaused = false;
    setFocusPolicy(Qt::StrongFocus);
}

void Game::drawDeadHolder(int x, int y, QColor color) {
    m_deadHolder = new QGraphicsRectItem(x, y, 300, 900);
    m_deadHolder->setBrush(QBrush(color));
    addToScene(m_deadHolder);
}

void Game::displayMainMenu() {
    QGraphicsPixmapItem* whiteKing =
        new QGraphicsPixmapItem(QPixmap(":/images/white_king.png"));
    whiteKing->setPos(920, 170);
    addToScene(whiteKing);
    m_menuItems.append(whiteKing);

    QGraphicsPixmapItem* blackKing =
        new QGraphicsPixmapItem(QPixmap(":/images/black_king.png"));
    blackKing->setPos(420, 170);
    addToScene(blackKing);
    m_menuItems.append(blackKing);

    QGraphicsTextItem* titleText = new QGraphicsTextItem("Chess");
    titleText->setFont(QFont("arial", 50));
    titleText->setPos(width() / 2 - titleText->boundingRect().width() / 2, 150);
    addToScene(titleText);
    m_menuItems.append(titleText);

    Button* playButton = new Button("Play");
    playButton->setPos(width() / 2 - playButton->boundingRect().width() / 2,
                       300);
    connect(playButton, SIGNAL(clicked()), this, SLOT(start()));
    addToScene(playButton);
    m_menuItems.append(playButton);

    Button* quitButton = new Button("Exit");
    quitButton->setPos(width() / 2 - quitButton->boundingRect().width() / 2,
                       375);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    addToScene(quitButton);
    m_menuItems.append(quitButton);

    drawChessBoard();
}

void Game::promotePawn(ChessPiece* pawn) {
    ChessBox* currentBox = pawn->getCurrentBox();
    removeFromScene(pawn);
    alivePieces.removeAll(pawn);

    Queen* newQueen = new Queen(pawn->getSide(), nullptr);
    newQueen->setCurrentBox(currentBox);
    currentBox->placePiece(newQueen);
    addToScene(newQueen);
    alivePieces.append(newQueen);
}
void Game::gameOver() {
    setTurn("WHITE");
    alivePieces.clear();
    m_chess->reset();
}

void Game::removeAll() {
    for (QGraphicsItem* item : m_gameScene->items()) {
        if (item != check) {
            removeFromScene(item);
        }
    }
}



void Game::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        togglePause();
    }
    QGraphicsView::keyPressEvent(event);
}

void Game::togglePause() {
    m_isPaused = !m_isPaused;
    if (m_isPaused) {
        showMainMenu();
    } else {
        hideMainMenu();
    }
}

void Game::showMainMenu() {
    // Pause game logic (e.g., stop timers)
    createMenuButtons();
    for (Button* button : m_menuButtons) {
        addToScene(button);
    }
}

void Game::hideMainMenu() {
    // Resume game logic (e.g., start timers)
    for (Button* button : m_menuButtons) {
        removeFromScene(button);
    }
    qDeleteAll(m_menuButtons);
    m_menuButtons.clear();
}

void Game::createMenuButtons() {
    Button* playButton = new Button("Play");
    playButton->setPos(width() / 2 - playButton->boundingRect().width() / 2, 300);
    connect(playButton, SIGNAL(clicked()), this, SLOT(resumeGame()));

    Button* resetButton = new Button("Restart");
    resetButton->setPos(width() / 2 - resetButton->boundingRect().width() / 2, 375);
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetGame()));

    Button* exitButton = new Button("Exit");
    exitButton->setPos(width() / 2 - exitButton->boundingRect().width() / 2, 450);
    connect(exitButton, SIGNAL(clicked()), this, SLOT(exitGame()));

    m_menuButtons << playButton << resetButton << exitButton;
}

void Game::resumeGame() {
    if (m_isPaused) {
        togglePause();
    }
}

void Game::resetGame() {
    gameOver();

    // Clear the dead pieces
    m_whiteDead.clear();
    m_blackDead.clear();


    hideMainMenu();
    m_isPaused = false;

    removeAll();
    drawChessBoard();

    start();
}

void Game::exitGame() {
    QApplication::quit();
}

