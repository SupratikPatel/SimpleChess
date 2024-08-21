#include "button.h"

#include <QBrush>
#include <QGraphicsTextItem>

Button::Button(const QString& name, QGraphicsItem* parent)
    : QGraphicsRectItem(parent) {
    setRect(0, 0, 200, 50);
    setButtonColor(Qt::darkRed);

    text = new QGraphicsTextItem(name, this);
    int xPos = rect().width() / 2 - text->boundingRect().width() / 2;
    int yPos = rect().height() / 2 - text->boundingRect().height() / 2;
    text->setPos(xPos, yPos);
    text->setDefaultTextColor(Qt::white);

    setAcceptHoverEvents(true);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event) {
        emit clicked();
    }
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    if (event) {
        setButtonColor(Qt::red);
    }
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    if (event) {
        setButtonColor(Qt::darkRed);
    }
}

void Button::setButtonColor(const QColor& color) {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    setBrush(brush);
}
