#ifndef GAME_H
#define GAME_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>

#include "board.h"
#include "pieces.h"
#include "button.h"

class Game : public QGraphicsView {
    Q_OBJECT

public:
    Game(QWidget* parent = nullptr);

    void drawDeadHolder(int x, int y, QColor color);
    void displayDeadPieces(const QList<ChessPiece*>& deadPieces, bool isWhite);
    void placeInDeadPlace(ChessPiece* piece);
    void addToScene(QGraphicsItem* item);
    void removeFromScene(QGraphicsItem* item);
    QString getTurn() const { return m_turn; }
    void setTurn(const QString& value);
    void changeTurn();
    void displayMainMenu();
    void gameOver();

    void keyPressEvent(QKeyEvent* event) override;
    void togglePause();
    void showMainMenu();
    void hideMainMenu();
    void promotePawn(ChessPiece* pawn);

    ChessPiece* pieceToMove;
    ChessBox* collection[8][8];
    QGraphicsTextItem* check;
    QList<ChessPiece*> alivePieces;

public slots:
    void start();
    void resumeGame();
    void resetGame();
    void exitGame();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    QGraphicsScene* m_gameScene;
    QList<ChessPiece*> m_whiteDead;
    QList<ChessPiece*> m_blackDead;
    QGraphicsRectItem* m_deadHolder;
    QString m_turn;
    ChessBoard* m_chess;
    QList<QGraphicsItem*> m_menuItems;
    QGraphicsTextItem* m_turnDisplay;

    bool m_isPaused;
    QList<Button*> m_menuButtons;
    void createMenuButtons();


    void drawChessBoard();
    void removeAll();

};

#endif  // GAME_H
