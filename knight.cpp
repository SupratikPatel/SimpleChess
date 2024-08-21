#include "knight.h"

#include "box.h"
#include "game.h"

extern Game* game;

Knight::Knight(const QString& team, QGraphicsItem* parent)
    : ChessPiece(team, parent) {
    setImage();
}

void Knight::setImage() {
    setPixmap(QPixmap(side == "WHITE" ? ":/images/white_knight.png"
                                      : ":/images/black_knight.png"));
}

void Knight::moves() {
    location.clear();
    int row = getCurrentBox()->rowLoc;
    int col = getCurrentBox()->colLoc;
    QString team = getSide();

    int possibleMoves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                               {1, -2},  {1, 2},  {2, -1},  {2, 1}};
    for (const auto& move : possibleMoves) {
        int newRow = row + move[0];
        int newCol = col + move[1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            ChessBox* box = game->collection[newRow][newCol];
            if (box->getChessPieceColor() != team) {
                location.append(box);
                box->setColor(box->getHasChessPiece() ? Qt::red : Qt::darkRed);
            }
        }
    }
}
