#include "point.h"

std::ofstream& operator<<(std::ofstream &out, const Point &p)
{
    out<<p.x<<std::endl<<p.y<<std::endl;
    return out;
}

QDataStream& operator <<(QDataStream &out, const Point &point)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out << point.x;
    out << point.y;
    return out;
}

QDataStream& operator >>(QDataStream &in, Point &point)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in >> point.x;
    in >> point.y;
    return in;
}
