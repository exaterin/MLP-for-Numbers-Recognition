#ifndef DRAWINGBOARD_H
#define DRAWINGBOARD_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>

class DrawingBoard : public QWidget{

    Q_OBJECT

    public:
        explicit DrawingBoard(QWidget *parent = nullptr);
        QSize sizeHint() const override;
        void clearBoard();
        QImage getScaledImage() const;

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void paintEvent(QPaintEvent *event) override;

    private:
        void drawLineTo(const QPoint &endPoint);

        bool drawing;
        QImage image;
        QPoint lastPoint;
        QPen pen;
};

#endif // DRAWINGBOARD_H
