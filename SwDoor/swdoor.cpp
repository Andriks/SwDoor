#include "swdoor.h"


SwDoor::SwDoor(float inp_E, int inp_tlim):
    E(inp_E),
    tlim(inp_tlim),
    t(0),
    door_ready(false),
    store_p_changed(false)
{
}

void SwDoor::hadlePoint(QPointF inp_p)
{
    store_p_changed = false;

    if (!door_ready) {
        storePoint(inp_p);
        P = inp_p;  // to start working

        return;
    }

    parsePointInfo(inp_p);
}

QPointF SwDoor::getLastStoredPoint() const
{
    return store_p;
}

bool SwDoor::storePointChanged() const
{
    return store_p_changed;
}

////////////////////////////////////////////////////////////////////////////////////////

void SwDoor::parsePointInfo(QPointF &inp_p) {
    State state = point_location(inp_p);

    // we are storing prev point to prevent losing of info and to have all info for normal working (stored and next after stored points)
    switch (state){
        case CHANGE: {
            if( !move_door_line(inp_p, L) || !move_door_line(inp_p, U) )
                setNewStoredPoint(prev_p, inp_p);

            break;
            }
        case L_CHANGE:
            {
            if( !move_door_line(inp_p, L) )
                setNewStoredPoint(prev_p, inp_p);

                break;
            }
        case U_CHANGE:
            {
            if( !move_door_line(inp_p, U) )
                setNewStoredPoint(prev_p, inp_p);

                break;
            }
        case INSIDE:
            {
                break;
            }

    }
    prev_p = inp_p;

    //mechanism of storing points after some number of inputs even if door was closed
    if (t++ >= tlim) setNewStoredPoint(prev_p, inp_p);
}

void SwDoor::setNewStoredPoint(QPointF &inp_p, QPointF &next_p) {
    storePoint(inp_p);
    P = next_p;

    calc_coef(P, U);
    calc_coef(P, L);
}


void SwDoor::storePoint(QPointF &inp_p)
{
    L.setX(inp_p.x()); L.setY(inp_p.y() - E);
    U.setX(inp_p.x()); U.setY(inp_p.y() + E);

    store_p = inp_p;
    prev_p = inp_p;
    door_ready = true;
    store_p_changed = true;

    t = 0;
}

SwDoor::State SwDoor::point_location(QPointF &p) {
    Orient stLU = classify_location(L, U, p);
    Orient stL  = classify_location(L, P, p);
    Orient stU  = classify_location(U, P, p);

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


SwDoor::Orient SwDoor::classify_location(QPointF &p0, QPointF &p1, QPointF &p) const
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

void SwDoor::calc_coef(const QPointF &p, const QPointF &ul)
{
    if (ul == U) {
        UA = U.y() - p.y();
        UB = p.x() - U.x();
        UC = -(U.x()*p.y() - p.x()*U.y());  //for cramer method
    } else if (ul == L) {
        LA = L.y() - p.y();
        LB = p.x() - L.x();
        LC = -(L.x()*p.y() - p.x()*L.y());  //for cramer method
    }
}

float SwDoor::det(float a, float b, float c, float d) const {
    return a * d - b * c;
}

bool SwDoor::move_door_line(QPointF &p, QPointF &what_moving)
{
    calc_coef(p, what_moving);
    if (setP())
        return true;
    else
        return false;
}

bool SwDoor::setP() {
    // new point, if U L are parallel or P in back - false

    float delta  = det(LA, LB, UA, UB);
    if (delta == 0) return false;

    float deltax = det(LC, LB, UC, UB);
    float deltay = det(LA, LC, UA, UC);

    float x = deltax/delta;
    float y = deltay/delta;

    if (x > P.x()) {
        P.setX(x); P.setY(y);
        return true;
    } else
        return false;

}
