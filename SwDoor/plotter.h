#ifndef PLOTTER_H
#define PLOTTER_H

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

protected:
   void paintEvent( QPaintEvent * event ) ;
   void doPaint(QPainter &p);

   int noise;
   int tlim;
   double E;

};

#endif // PLOTTER_H
