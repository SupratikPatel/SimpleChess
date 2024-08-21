#include "king.h"

#include "box.h"
#include "game.h"
#include "pawn.h"
#include "rook.h"

extern Game* game;

King::King(const QString& team, QGraphicsItem* parent)
    : ChessPiece(team, parent) {
    setImage();
}

void King::setImage() {
    setPixmap(QPixmap(side == "WHITE" ? ":/images/white_king.png"
                                      : ":/images/black_king.png"));
}

void King::moves() {
    location.clear();
    int row = getCurrentBox()->rowLoc;
    int col = getCurrentBox()->colLoc;
    QString team = getSide();

    // Normal king moves
    int possibleMoves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                               {0, 1},   {1, -1}, {1, 0},  {1, 1}};
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

    // Castling moves
    if (!firstMove) {
        // Kingside castling
        ChessPiece* right_rook = game->collection[row][7]->currentPiece;
        if (dynamic_cast<Rook*>(right_rook) && !right_rook->firstMove) {
            bool canCastle = true;
            for (int c = 5; c < 7; ++c) {
                if (game->collection[row][c]->getHasChessPiece()) {
                    canCastle = false;
                    break;
                }
            }
            if (canCastle) {
                ChessBox* castleBox = game->collection[row][6];
                location.append(castleBox);
                castleBox->setColor(Qt::darkGreen);
            }
        }

        // Queenside castling
        ChessPiece* left_rook = game->collection[row][0]->currentPiece;
        if (dynamic_cast<Rook*>(left_rook) && !left_rook->firstMove) {
            bool canCastle = true;
            for (int c = 1; c < 4; ++c) {
                if (game->collection[row][c]->getHasChessPiece()) {
                    canCastle = false;
                    break;
                }
            }
            if (canCastle) {
                ChessBox* castleBox = game->collection[row][2];
                location.append(castleBox);
                castleBox->setColor(Qt::darkGreen);
            }
        }
    }

    findUnSafeLocation();
}


void King::findUnSafeLocation() {
    for (ChessPiece* piece : game->alivePieces) {
        if (piece->getSide() != this->getSide()) {
            QList<ChessBox*> dangerBoxes = piece->moveLocation();
            for (ChessBox* dangerBox : dangerBoxes) {
                if (!dynamic_cast<Pawn*>(piece)) {
                    for (ChessBox* kingBox : location) {
                        if (dangerBox == kingBox) {
                            kingBox->setColor(Qt::blue);
                        }
                    }
                }
            }
        }
    }
}




bool King::canCastle(bool kingSide) const {
    int row = getCurrentBox()->rowLoc;
    int col = getCurrentBox()->colLoc;
    int direction = kingSide ? 1 : -1;
    int rookCol = kingSide ? 7 : 0;

    // Check if the king and rook haven't moved
    if (firstMove) return false;
    ChessPiece* rook = game->collection[row][rookCol]->currentPiece;
    if (!rook || !dynamic_cast<Rook*>(rook) || rook->firstMove) return false;

    // Check if the path is clear
    for (int i = 1; i <= (kingSide ? 2 : 3); ++i) {
        if (game->collection[row][col + i * direction]->getHasChessPiece()) return false;
    }

    return true;
}

void King::castle(bool kingSide) {
    int row = getCurrentBox()->rowLoc;
    int col = getCurrentBox()->colLoc;
    int direction = kingSide ? 1 : -1;
    int rookCol = kingSide ? 7 : 0;

    // Move the king
    ChessBox* newKingBox = game->collection[row][col + 2 * direction];
    getCurrentBox()->setHasChessPiece(false);
    getCurrentBox()->currentPiece = nullptr;
    setCurrentBox(newKingBox);
    newKingBox->placePiece(this);

    // Move the rook
    Rook* rook = dynamic_cast<Rook*>(game->collection[row][rookCol]->currentPiece);
    ChessBox* newRookBox = game->collection[row][col + direction];
    rook->getCurrentBox()->setHasChessPiece(false);
    rook->getCurrentBox()->currentPiece = nullptr;
    rook->setCurrentBox(newRookBox);
    newRookBox->placePiece(rook);

    firstMove = false;
    rook->firstMove = false;
}
