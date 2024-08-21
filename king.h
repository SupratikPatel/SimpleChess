#ifndef KING_H
#define KING_H

#include "pieces.h"

class King : public ChessPiece {
public:
    King(const QString& team, QGraphicsItem* parent = nullptr);
    void setImage() override;
    void moves() override;
    bool canCastle(bool kingSide) const;
    void castle(bool kingSide);


private:
    void findUnSafeLocation();
};

#endif  // KING_H
