#ifndef BOX_H
#define BOX_H

#include <QBrush>
#include <QGraphicsRectItem>

class ChessPiece;

class ChessBox : public QGraphicsRectItem {
public:
    ChessBox(QGraphicsItem* parent = nullptr);
    ~ChessBox();

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void setColor(QColor color);
    void placePiece(ChessPiece* piece);
    void resetOriginalColor();
    void setOriginalColor(QColor value);
    bool getHasChessPiece() const;
    void setHasChessPiece(bool value, ChessPiece* piece = nullptr);
    void checkForCheck();
    QString getChessPieceColor() const;
    void setChessPieceColor(const QString& value);

    int rowLoc;
    int colLoc;
    ChessPiece* currentPiece;

private:
    QBrush brush;
    QColor originalColor;
    bool hasChessPiece;
    QString chessPieceColor;
};

#endif  // BOX_H
