#ifndef NOTEVIEW_H
#define NOTEVIEW_H

#include <QGraphicsView>
#include <QGraphicsRectItem>

class NoteItem : public QGraphicsRectItem
{
public:
    NoteItem(class MidiNote* note, QGraphicsItem *parent = 0);
    virtual ~NoteItem();

    class MidiNote* _note;
};

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

    bool _pan;
    int _panStartX, _panStartY;
};

#endif // NOTEVIEW_H
