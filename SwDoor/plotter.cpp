#include "plotter.h"

#include <QtCore/qmath.h>
#include <QPainterPath>
#include <QFileDialog>
#include <QTime>


Plotter::Plotter(QWidget *parent) :
    QWidget(parent),
    tlim_ (100),
    noise_(5),
    E_(8)
{
}

void Plotter::noiseChanged(int v)
{
   noise_ = v;
   repaint();
}

void Plotter::tlimChanged(int v)
{
   tlim_ = v;
   repaint();
}

void Plotter::EChanged(double v)
{
   E_ = v;
   repaint();
}

void Plotter::updateSignal()
{
    repaint();
}

int randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}

qreal MyFunc(float x) {
    int k = 40;

    if(0) {
        static int count = 0;
        count++;
        if(count == 30) {
            k *= 2;
            count = 0;  }
    }

    return k*qSin(x);
}

void Plotter::paintEvent(QPaintEvent *event)
{
   int w = width();
   int h = height();
   int side = qMin(w, h);

   QPainter painter(this);
   painter.setRenderHint(QPainter::Antialiasing);
   painter.translate(0 , h / 2);
   painter.scale(side / 200.0, side / 200.0);
   doPaint(painter);
}


void Plotter::doPaint(QPainter &painter)
{
    QColor c1(127, 0, 0);
    QColor c2(0, 127, 0);
    QColor c3(0, 0, 127);
    QPen p(c1);
    p.setWidth(2);
    painter.setPen(p);

    QPainterPath path_sign;
    QPainterPath path_res;

    //////////////////////////////////
    door_.setMainData(E_, tlim_);
    //////////////////////////////////

    //for rand()
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    //for signal
    QPointF p_sign;
    float sig_end = 6*M_PI;
    float sig_count = width()/2;
    float sig_step = sig_end/sig_count;
    float count = 0;

    for (float i=sig_step; i<sig_end; i+=sig_step) {
        int k = randInt(noise_,-1*noise_);

        //drawing signal
        p_sign.setX(count++);
        p_sign.setY(MyFunc(i)+k);
        path_sign.lineTo(p_sign);

        door_.hadlePoint(p_sign);
        if (door_.storePointChanged()) {
            path_res.lineTo(door_.getLastStoredPoint());
            painter.drawPoint(door_.getLastStoredPoint());
        }
    }

    p.setColor(c2);
    p.setWidth(0.5);
    painter.setPen(p);
    painter.drawPath(path_sign);
    p.setColor(c3);
    p.setWidth(0.5);
    painter.setPen(p);
    painter.drawPath(path_res);
}
