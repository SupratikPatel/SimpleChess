#include "pieces.h"

#include <QGraphicsSceneMouseEvent>

#include "box.h"
#include "game.h"

extern Game* game;

ChessPiece::ChessPiece(const QString& team, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
      side(team),
      isPlaced(true),
      firstMove(true) {}

void ChessPiece::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (this == game->pieceToMove) {
        game->pieceToMove->getCurrentBox()->resetOriginalColor();
        game->pieceToMove->decolor();
        game->pieceToMove = nullptr;
        return;
    }

    if (!getIsPlaced() ||
        (game->getTurn() != this->getSide() && !game->pieceToMove)) {
        return;
    }

    if (!game->pieceToMove) {
        game->pieceToMove = this;
        game->pieceToMove->getCurrentBox()->setColor(Qt::yellow);
        game->pieceToMove->moves();
    } else if (this->getSide() != game->pieceToMove->getSide()) {
        this->getCurrentBox()->mousePressEvent(event);
    }
}

void ChessPiece::setCurrentBox(ChessBox* box) {
    currentBox = box;
}

ChessBox* ChessPiece::getCurrentBox() const {
    return currentBox;
}

QString ChessPiece::getSide() const {
    return side;
}

void ChessPiece::setSide(const QString& value) {
    side = value;
}

bool ChessPiece::getIsPlaced() const {
    return isPlaced;
}

void ChessPiece::setIsPlaced(bool value) {
    isPlaced = value;
}

QList<ChessBox*> ChessPiece::moveLocation() {
    return location;
}

void ChessPiece::decolor() {
    for (ChessBox* box : location) {
        box->resetOriginalColor();
    }
}

bool ChessPiece::boxSetting(ChessBox* box) {
    if (box->getHasChessPiece()) {
        box->setColor(Qt::red);
        return true;
    } else {
        box->setColor(Qt::darkCyan);
        return false;
    }
}
