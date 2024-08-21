#ifndef QUEEN_H
#define QUEEN_H

#include "pieces.h"

class Queen : public ChessPiece {
public:
    Queen(const QString& team, QGraphicsItem* parent = nullptr);
    void setImage() override;
    void moves() override;
};

#endif  // QUEEN_H
