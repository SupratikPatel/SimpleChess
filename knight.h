#ifndef KNIGHT_H
#define KNIGHT_H

#include "pieces.h"

class Knight : public ChessPiece {
public:
    Knight(const QString& team, QGraphicsItem* parent = nullptr);
    void setImage() override;
    void moves() override;
};

#endif  // KNIGHT_H
