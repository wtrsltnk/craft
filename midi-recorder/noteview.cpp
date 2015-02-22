#include "noteview.h"
#include <QMouseEvent>
#include <QScrollBar>
#include "midiclip.h"

NoteItem::NoteItem(MidiNote* note, QGraphicsItem *parent)
    : QGraphicsRectItem(parent), _note(note)
{
    qreal x = note->_start / 10;
    qreal y = int(note->_note) * 12;
    qreal w = (note->_end - note->_start) / 10;
    qreal h = 12;

    this->setRect(x, y, w, h);
    this->setBrush(QBrush(Qt::red));
    this->setPen(QPen(Qt::transparent));
}

NoteItem::~NoteItem()
{ }

NoteView::NoteView(QWidget *parent)
    : QGraphicsView(parent)
{ }

NoteView::~NoteView()
{ }

void NoteView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        this->_pan = true;
        this->_panStartX = event->x();
        this->_panStartY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void NoteView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        this->_pan = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void NoteView::mouseMoveEvent(QMouseEvent *event)
{
    if (this->_pan)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - this->_panStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - this->_panStartY));
        this->_panStartX = event->x();
        this->_panStartY = event->y();
        event->accept();
        return;
    }
    event->ignore();
}

void NoteView::drawBackground(QPainter *p, const QRectF& rect)
{
    static QPen transparent(Qt::transparent);
    static QPen darkGrayThick(QBrush(Qt::darkGray), 3.0);

    const QBrush& b = p->brush();
    p->setBrush(this->palette().background());
    p->setPen(transparent);
    p->drawRect(rect);
    const int gridSize = 12;
    const int noteSize = 24;

    qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    QVarLengthArray<QRectF, 100> rects;

    for (qreal y = top; y < rect.bottom(); y += gridSize)
    {
        int i = (int(y)/gridSize) % 12;
        if (i < 0) i = -i;
        if (i == 1 || i == 3 || i == 6 || i == 8 || i == 10)
        rects.append(QRectF(rect.left(), y, rect.width(), gridSize));
    }

    p->setBrush(this->palette().highlight());
    p->drawRects(rects.data(), rects.size());
    p->setBrush(b);
    p->setPen(QPen(this->palette().dark(), 1));


    QVarLengthArray<QLineF, 100> lines;

    for (qreal y = top; y < rect.bottom(); y += gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    p->drawLines(lines.data(), lines.size());

    qreal left = int(rect.left()) - (int(rect.left()) % gridSize);

    QVarLengthArray<QLineF, 100> vlines;

    for (qreal x = left; x < rect.right(); x += noteSize)
        vlines.append(QLineF(x, rect.top(), x, rect.bottom()));

    p->drawLines(vlines.data(), vlines.size());
    p->setPen(darkGrayThick);
    p->drawLine(QLineF(0, rect.top(), 0, rect.bottom()));
}
