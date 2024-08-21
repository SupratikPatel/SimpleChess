#ifndef PIECES_H
#define PIECES_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QString>

class ChessBox;
class Game;

class ChessPiece : public QGraphicsPixmapItem {
public:
    ChessPiece(const QString& team, QGraphicsItem* parent = nullptr);
    virtual ~ChessPiece() = default;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void setCurrentBox(ChessBox* box);
    ChessBox* getCurrentBox() const;
    QString getSide() const;
    void setSide(const QString& value);
    virtual void setImage() = 0;
    bool getIsPlaced() const;
    void setIsPlaced(bool value);
    QList<ChessBox*> moveLocation();
    virtual void moves() = 0;
    void decolor();
    bool boxSetting(ChessBox* box);

    bool firstMove;

protected:
    ChessBox* currentBox;
    QString side;
    bool isPlaced;
    QList<ChessBox*> location;
};

#endif  // PIECES_H
