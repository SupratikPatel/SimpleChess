#ifndef PAWN_H
#define PAWN_H

#include "pieces.h"

class Pawn : public ChessPiece {
public:
    Pawn(const QString& team, QGraphicsItem* parent = nullptr);
    void setImage() override;
    void moves() override;
    void promote();
};

#endif  // PAWN_H
