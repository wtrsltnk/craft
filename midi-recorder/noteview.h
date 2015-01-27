#ifndef NOTEVIEW_H
#define NOTEVIEW_H

#include <QGraphicsView>

class NoteView : public QGraphicsView
{
    Q_OBJECT

public:
    NoteView(QWidget *parent);
    ~NoteView();

private:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void drawBackground(QPainter *p, const QRectF& crect);
    virtual void resize(int w, int h);

    bool _pan;
    int _panStartX, _panStartY;
};

#endif // NOTEVIEW_H
