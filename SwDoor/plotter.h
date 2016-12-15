#ifndef PLOTTER_H
#define PLOTTER_H

#include "swdoor.h"

#include <QWidget>
#include <QPainter>

class Plotter : public QWidget
{
   Q_OBJECT
public:
   explicit Plotter(QWidget *parent = 0);

signals:

public slots:
   void noiseChanged(int);
   void tlimChanged(int);
   void EChanged(double);
   void updateSignal();

protected:
   void paintEvent( QPaintEvent*) ;
   void doPaint(QPainter&);

   SwDoor door_;
   int noise_;
   int tlim_;
   double E_;

};

#endif // PLOTTER_H
