#ifndef PAINTSCENE_H
#define PAINTSCENE_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
class paintscene : public QGraphicsScene
{
public:
    explicit paintscene(QObject *parent = 0);
    ~paintscene();
    QPointF     previousPoint;
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void clearscene();
    int sceneWidth=50;//50 by default
    int sceneHeight=50;
    int brushSize=10;
};
#endif // PAINTSCENE_H
