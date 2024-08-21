#include "pawn.h"

#include "box.h"
#include "game.h"
#include "queen.h"
extern Game* game;

Pawn::Pawn(const QString& team, QGraphicsItem* parent)
    : ChessPiece(team, parent) {
    setImage();
    firstMove = true;
}

void Pawn::setImage() {
    setPixmap(QPixmap(side == "WHITE" ? ":/images/white_pawn.png"
                                      : ":/images/black_pawn.png"));
}

void Pawn::moves() {
    location.clear();
    int row = getCurrentBox()->rowLoc;
    int col = getCurrentBox()->colLoc;
    QString enemyColor = (getSide() == "WHITE") ? "BLACK" : "WHITE";
    int direction = (getSide() == "WHITE") ? -1 : 1;

    // Move forward
    if (row + direction >= 0 && row + direction < 8 &&
        !game->collection[row + direction][col]->getHasChessPiece()) {
        location.append(game->collection[row + direction][col]);
        boxSetting(location.last());

        // First move can be two steps
        if (firstMove &&
            !game->collection[row + 2 * direction][col]->getHasChessPiece()) {
            location.append(game->collection[row + 2 * direction][col]);
            boxSetting(location.last());
        }
    }

    // Capture diagonally
    for (int i : {-1, 1}) {
        if (col + i >= 0 && col + i < 8 && row + direction >= 0 &&
            row + direction < 8) {
            if (game->collection[row + direction][col + i]
                    ->getChessPieceColor() == enemyColor) {
                location.append(game->collection[row + direction][col + i]);
                boxSetting(location.last());
            }
        }
    }
    // Check for promotion
    if ((getSide() == "WHITE" && row == 0) || (getSide() == "BLACK" && row == 7)) {
        game->promotePawn(this);
    }
}

void Pawn::promote() {
    ChessBox* currentBox = getCurrentBox();
    game->removeFromScene(this);
    game->alivePieces.removeAll(this);

    Queen* newQueen = new Queen(getSide(), nullptr);
    newQueen->setCurrentBox(currentBox);
    currentBox->placePiece(newQueen);
    game->addToScene(newQueen);
    game->alivePieces.append(newQueen);
}
