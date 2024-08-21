#ifndef BISHOP_H
#define BISHOP_H

#include "pieces.h"

class Bishop : public ChessPiece {
public:
    Bishop(const QString& team, QGraphicsItem* parent = nullptr);
    void setImage() override;
    void moves() override;
};

#endif  // BISHOP_H
