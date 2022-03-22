#ifndef BARRIER_H
#define BARRIER_H

#include<QDataStream>
#include<QXmlStreamWriter>

class Barrier
{
public:
    float x;
    float y;
    float width;
    float height;
    Barrier():x(0),y(0),width(0), height(0){}
    Barrier(float x,float y,float width,float height):x(x),y(y),width(width),height(height){}
    Barrier(const Barrier& barrier):x(barrier.x),y(barrier.y), width(barrier.width), height(barrier.height){}
    ~Barrier(){};
    friend QDataStream& operator <<(QDataStream &out, const Barrier &b);
    friend QDataStream& operator >>(QDataStream &in, Barrier &b);
};

class BarrierXMLWriter
{
public:
    int countBarrier=0;
    void print(QXmlStreamWriter& XMLWriter, const Barrier& barrier);
};

#endif
