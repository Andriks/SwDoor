#include "swdoor.h"


SwDoor::SwDoor():
    t_(0),
    door_ready_(false),
    store_p_changed_(false)
{
}

void SwDoor::setMainData(float e, int tlim)
{
    setE(e);
    setTlim(tlim);
    backToInitState();
}

void SwDoor::hadlePoint(QPointF inp_p)
{
    store_p_changed_ = false;

    if (!door_ready_) {
        storePoint(inp_p);
        P_ = inp_p;  // to start working

        return;
    }

    parsePointInfo(inp_p);
}

QPointF SwDoor::getLastStoredPoint() const
{
    return store_p_;
}

void SwDoor::setE(float e)
{
    if (e != E_) {
        E_ = e;
    }
}

void SwDoor::setTlim(int tlim)
{
    if (tlim != tlim_) {
        tlim_ = tlim;
    }
}

void SwDoor::backToInitState()
{
    t_ = 0;
    door_ready_ = false;
    store_p_changed_ = false;
}

bool SwDoor::storePointChanged() const
{
    return store_p_changed_;
}

////////////////////////////////////////////////////////////////////////////////////////

void SwDoor::parsePointInfo(QPointF &inp_p) {
    State state = pointLocation(inp_p);

    // we are storing prev point to prevent losing of info and to have all info for normal working (stored and next after stored points)
    switch (state){
        case CHANGE: {
            if( !moveDoorLine(inp_p, L_) || !moveDoorLine(inp_p, U_) )
                setNewStoredPoint(prev_p_, inp_p);

            break;
            }
        case L_CHANGE:
            {
            if( !moveDoorLine(inp_p, L_) )
                setNewStoredPoint(prev_p_, inp_p);

                break;
            }
        case U_CHANGE:
            {
            if( !moveDoorLine(inp_p, U_) )
                setNewStoredPoint(prev_p_, inp_p);

                break;
            }
        case INSIDE:
            {
                break;
            }

    }
    prev_p_ = inp_p;

    //mechanism of storing points after some number of inputs even if door was closed
    if (t_++ >= tlim_) setNewStoredPoint(prev_p_, inp_p);
}

void SwDoor::setNewStoredPoint(QPointF &inp_p, QPointF &next_p) {
    storePoint(inp_p);
    P_ = next_p;

    calcCoef(P_, U_);
    calcCoef(P_, L_);
}


void SwDoor::storePoint(QPointF &inp_p)
{
    L_.setX(inp_p.x()); L_.setY(inp_p.y() - E_);
    U_.setX(inp_p.x()); U_.setY(inp_p.y() + E_);

    store_p_ = inp_p;
    prev_p_ = inp_p;
    door_ready_ = true;
    store_p_changed_ = true;

    t_ = 0;
}

SwDoor::State SwDoor::pointLocation(QPointF &p) {
    Orient stLU = classifyLocation(L_, U_, p);
    Orient stL  = classifyLocation(L_, P_, p);
    Orient stU  = classifyLocation(U_, P_, p);

    if (stLU == ONLINE && stL == ONLINE && stU == ONLINE)
        return INSIDE;
    else if (stLU == RIGHT && stL == LEFT && stU == RIGHT)
        return INSIDE;
    else if(stL == RIGHT && stU == LEFT)
        return CHANGE;
    else if(stL == RIGHT)
        return L_CHANGE;
    else if(stU == LEFT)
        return U_CHANGE;
}


SwDoor::Orient SwDoor::classifyLocation(QPointF &p0, QPointF &p1, QPointF &p) const
{
  QPointF p2 = p;
  QPointF a = p1 - p0;
  QPointF b = p2 - p0;
  qreal sa = a.x() * b.y() - b.x() * a.y();
  if (sa > 0.0)
    return LEFT;
  if (sa < 0.0)
    return RIGHT;
  return ONLINE;
}

void SwDoor::calcCoef(const QPointF &p, const QPointF &ul)
{
    if (ul == U_) {
        UA_ = U_.y() - p.y();
        UB_ = p.x() - U_.x();
        UC_ = -(U_.x()*p.y() - p.x()*U_.y());  //for cramer method
    } else if (ul == L_) {
        LA_ = L_.y() - p.y();
        LB_ = p.x() - L_.x();
        LC_ = -(L_.x()*p.y() - p.x()*L_.y());  //for cramer method
    }
}

float SwDoor::det(float a, float b, float c, float d) const {
    return a * d - b * c;
}

bool SwDoor::moveDoorLine(QPointF &p, QPointF &what_moving)
{
    calcCoef(p, what_moving);
    if (setP())
        return true;
    else
        return false;
}

bool SwDoor::setP() {
    // new point, if U L are parallel or P in back - false

    float delta  = det(LA_, LB_, UA_, UB_);
    if (delta == 0) return false;

    float deltax = det(LC_, LB_, UC_, UB_);
    float deltay = det(LA_, LC_, UA_, UC_);

    float x = deltax/delta;
    float y = deltay/delta;

    if (x > P_.x()) {
        P_.setX(x); P_.setY(y);
        return true;
    } else
        return false;

}
