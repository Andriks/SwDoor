#ifndef SWDOOR_H
#define SWDOOR_H

#include <QtCore/qmath.h>
#include <QPainter>

class SwDoor
{
private:
    enum Orient {LEFT,  RIGHT,  ONLINE};
    enum State {INSIDE, L_CHANGE, U_CHANGE, CHANGE};

public:
    SwDoor();

    void setMainData(float e, int tlim);

    //hatdling and store (if needs) new input point
    void hadlePoint(QPointF);

    bool storePointChanged() const;
    QPointF getLastStoredPoint() const;

private:
    void setE(float e);
    void setTlim(int tlim);
    void backToInitState();

    void setNewStoredPoint(QPointF&, QPointF&);
    void parsePointInfo(QPointF&);
    //save all params relatively new stored point
    void storePoint(QPointF&);
    // cheking input point if it inside current door
    State pointLocation(QPointF&);
    // movin some of door lines to new point
    bool moveDoorLine(QPointF&,QPointF&);
    //coefs for checking door state (opened/closed)
    void calcCoef(const QPointF&, const QPointF&);
    float det(float, float, float, float) const;

    Orient classifyLocation(QPointF&, QPointF&, QPointF&) const;

    bool setP();

private:
    bool door_ready_;
    bool store_p_changed_;
    float E_;    // range for U/L (from stored point)
    QPointF U_;  // upper point of door
    QPointF L_;  // lower point of door
    QPointF P_;
    QPointF store_p_;    //last stored point of working signal
    QPointF prev_p_;     //previous point of working signal
    float LA_, LB_, LC_, UA_, UB_, UC_;   //coefs for calculating door state (opened/closed)

    // for mechanism of storing points after some number of inputs even if door was closed
    int t_;      //current num of unstored inputs
    int tlim_;   //max num of unstored inputs
};


#endif // SWDOOR_H
