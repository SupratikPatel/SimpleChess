#include "queen.h"

#include "box.h"
#include "game.h"

extern Game* game;

Queen::Queen(const QString& team, QGraphicsItem* parent)
    : ChessPiece(team, parent) {
    setImage();
}

void Queen::setImage() {
    setPixmap(QPixmap(side == "WHITE" ? ":/images/white_queen.png"
                                      : ":/images/black_queen.png"));
}

void Queen::moves() {
    location.clear();
    int row = getCurrentBox()->rowLoc;
    int col = getCurrentBox()->colLoc;
    QString team = getSide();

    // Directions: up, down, left, right, up-left, up-right, down-left,
    // down-right
    int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
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
