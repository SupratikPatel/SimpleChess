#include "box.h"

#include <QGraphicsSceneMouseEvent>

#include "game.h"
#include "king.h"

extern Game* game;

ChessBox::ChessBox(QGraphicsItem* parent) : QGraphicsRectItem(parent) {
    setRect(0, 0, 100, 100);
    brush.setStyle(Qt::SolidPattern);
    setZValue(-1);
    setHasChessPiece(false);
    setChessPieceColor("NONE");
    currentPiece = nullptr;
}

ChessBox::~ChessBox() = default;

void ChessBox::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (currentPiece == game->pieceToMove && currentPiece) {
        currentPiece->mousePressEvent(event);
        return;
    }
    if (game->pieceToMove) {
        if (this->getChessPieceColor() == game->pieceToMove->getSide()) return;
        QList<ChessBox*> movLoc = game->pieceToMove->moveLocation();
        if (!movLoc.contains(this)) return;

        game->pieceToMove->decolor();
        game->pieceToMove->firstMove = false;

        if (this->getHasChessPiece()) {
            this->currentPiece->setIsPlaced(false);
            this->currentPiece->setCurrentBox(nullptr);
            game->placeInDeadPlace(this->currentPiece);
        }

        game->pieceToMove->getCurrentBox()->setHasChessPiece(false);
        game->pieceToMove->getCurrentBox()->currentPiece = nullptr;
        game->pieceToMove->getCurrentBox()->resetOriginalColor();
        placePiece(game->pieceToMove);

        game->pieceToMove = nullptr;
        game->changeTurn();
        checkForCheck();
    } else if (this->getHasChessPiece()) {
        this->currentPiece->mousePressEvent(event);
    }
}

void ChessBox::setColor(QColor color) {
    brush.setColor(color);
    setBrush(color);
}

void ChessBox::placePiece(ChessPiece* piece) {
    piece->setPos(x() + 50 - piece->pixmap().width() / 2,
                  y() + 50 - piece->pixmap().height() / 2);
    piece->setCurrentBox(this);
    setHasChessPiece(true, piece);
    currentPiece = piece;
}

void ChessBox::resetOriginalColor() {
    setColor(originalColor);
}

void ChessBox::setOriginalColor(QColor value) {
    originalColor = value;
    setColor(originalColor);
}

bool ChessBox::getHasChessPiece() const {
    return hasChessPiece;
}

void ChessBox::setHasChessPiece(bool value, ChessPiece* piece) {
    hasChessPiece = value;
    setChessPieceColor(value ? piece->getSide() : "NONE");
}

void ChessBox::checkForCheck() {
    int c = 0;
    QList<ChessPiece*> pList = game->alivePieces;
    for (ChessPiece* piece : pList) {
        King* king = dynamic_cast<King*>(piece);
        if (king) continue;

        piece->moves();
        piece->decolor();
        QList<ChessBox*> bList = piece->moveLocation();

        for (ChessBox* box : bList) {
            King* king = dynamic_cast<King*>(box->currentPiece);
            if (king && king->getSide() != piece->getSide()) {
                box->setColor(Qt::blue);
                piece->getCurrentBox()->setColor(Qt::darkRed);
                if (!game->check->isVisible()) {
                    game->check->setVisible(true);
                } else {
                    box->resetOriginalColor();
                    piece->getCurrentBox()->resetOriginalColor();
                    game->gameOver();
                }
                c++;
            }
        }
    }

    if (!c) {
        game->check->setVisible(false);
        for (ChessPiece* piece : pList) {
            piece->getCurrentBox()->resetOriginalColor();
        }
    }
}

QString ChessBox::getChessPieceColor() const {
    return chessPieceColor;
}

void ChessBox::setChessPieceColor(const QString& value) {
    chessPieceColor = value;
}
