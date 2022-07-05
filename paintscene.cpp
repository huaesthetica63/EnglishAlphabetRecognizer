#include "paintscene.h"

paintscene::paintscene(QObject *parent) : QGraphicsScene(parent)
{
    setSceneRect(0,0,sceneWidth,sceneHeight);//set scene rect fixed size
}

paintscene::~paintscene()
{

}

void paintscene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // drawing ellipse when mouse is clicking first time
    addEllipse(event->scenePos().x(),
               event->scenePos().y(),
               1,
               1,
               QPen(Qt::NoPen),
               QBrush(Qt::black));
    // save this point
    previousPoint = event->scenePos();
}

void paintscene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // when mouse is holding clicked
    addLine(previousPoint.x(),
            previousPoint.y(),
            event->scenePos().x(),
            event->scenePos().y(),
            QPen(Qt::black,brushSize,Qt::SolidLine,Qt::RoundCap));
    previousPoint = event->scenePos();//update last point
}
void paintscene::clearscene(){
    this->clear();
}
