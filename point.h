#ifndef POINT_H
#define POINT_H

#include<fstream>
#include<QDataStream>

class Point
{

public:
    float x;
    float y;
    Point():x(0),y(0)
    {

    }
    Point(float x, float y):x(x),y(y)
    {

    }
    Point (const Point& o):x(o.x),y(o.y)
    {

    }
    friend std::ofstream& operator<<(std::ofstream &out, const Point &p);
    friend QDataStream& operator <<(QDataStream &out, const Point &point);
    friend QDataStream& operator >>(QDataStream &in, Point &point);
};
#endif // POINT_H
