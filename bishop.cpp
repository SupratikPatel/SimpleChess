#include "bishop.h"

#include "box.h"
#include "game.h"

extern Game* game;

Bishop::Bishop(const QString& team, QGraphicsItem* parent)
    : ChessPiece(team, parent) {
    setImage();
}

void Bishop::setImage() {
    setPixmap(QPixmap(side == "WHITE" ? ":/images/white_bishop.png"
                                      : ":/images/black_bishop.png"));
}

void Bishop::moves() {
    location.clear();
    int row = getCurrentBox()->rowLoc;
    int col = getCurrentBox()->colLoc;

    int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for (const auto& dir : directions) {
        for (int i = 1; i < 8; ++i) {
            int newRow = row + i * dir[0];
            int newCol = col + i * dir[1];

            if (newRow < 0 || newRow > 7 || newCol < 0 || newCol > 7) break;

            ChessBox* box = game->collection[newRow][newCol];
            if (box->getChessPieceColor() == side) break;

            location.append(box);
            if (boxSetting(box)) break;
        }
    }
}
