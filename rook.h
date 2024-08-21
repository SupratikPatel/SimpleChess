#ifndef ROOK_H
#define ROOK_H

#include "pieces.h"

class Rook : public ChessPiece {
public:
    Rook(const QString& team, QGraphicsItem* parent = nullptr);
    void setImage() override;
    void moves() override;
};

#endif  // ROOK_H
