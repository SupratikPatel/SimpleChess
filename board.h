#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsRectItem>

#include "pieces.h"

class ChessBoard {
public:
    ChessBoard();

    // drawing public function
    void drawBoxes(int x, int y);

    void setUpWhite();
    void setUpBlack();
    void addChessPiece();
    void reset();

private:
    QList<ChessPiece*> white;
    QList<ChessPiece*> black;
};

#endif  // BOARD_H
