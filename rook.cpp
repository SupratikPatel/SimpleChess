#include "rook.h"

#include "box.h"
#include "game.h"

extern Game* game;

Rook::Rook(const QString& team, QGraphicsItem* parent)
    : ChessPiece(team, parent) {
    setImage();
}

void Rook::setImage() {
    setPixmap(QPixmap(side == "WHITE" ? ":/images/white_rook.png"
                                      : ":/images/black_rook.png"));
}

void Rook::moves() {
    location.clear();
    int row = getCurrentBox()->rowLoc;
    int col = getCurrentBox()->colLoc;
    QString team = getSide();

    // Directions: up, down, left, right
    int directions[4][2] = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    for (const auto& dir : directions) {
        for (int i = 1; i < 8; ++i) {
            int newRow = row + i * dir[0];
            int newCol = col + i * dir[1];

            if (newRow < 0 || newRow > 7 || newCol < 0 || newCol > 7) break;

            ChessBox* box = game->collection[newRow][newCol];
            if (box->getChessPieceColor() == team) break;

            location.append(box);
            if (boxSetting(box)) break;
        }
    }
}
