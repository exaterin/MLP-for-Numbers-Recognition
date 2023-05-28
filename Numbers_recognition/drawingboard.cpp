#include "drawingboard.h"

DrawingBoard::DrawingBoard(QWidget *parent) : QWidget(parent), drawing(false){

    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    image = QImage(28 * 10, 28 * 10, QImage::Format_ARGB32);
    image.fill(Qt::white);

    pen.setColor(Qt::magenta); // Set the color to pink
    pen.setWidth(17); // Set the width of the pen
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
}

QSize DrawingBoard::sizeHint() const {
    return QSize(28 * 10, 28 * 10);
}

void DrawingBoard::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        drawing = true;
    }
}

void DrawingBoard::mouseMoveEvent(QMouseEvent *event){
    if ((event->buttons() & Qt::LeftButton) && drawing){
        drawLineTo(event->pos());
    }
}

void DrawingBoard::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton && drawing){
        drawLineTo(event->pos());
        drawing = false;
    }
}

void DrawingBoard::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void DrawingBoard::drawLineTo(const QPoint &endPoint){
    QPainter painter(&image);
    painter.setPen(pen);
    painter.drawLine(lastPoint, endPoint);

    int rad = (pen.width() / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void DrawingBoard::clearBoard(){
    image.fill(Qt::white);
    update();
}

QImage DrawingBoard::getScaledImage() const{
    return image.scaled(28, 28, Qt::AspectRatioMode::IgnoreAspectRatio, Qt::SmoothTransformation);
}
