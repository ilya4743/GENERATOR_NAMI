#ifndef BARRIER_H
#define BARRIER_H

#include<QDataStream>
#include<QXmlStreamWriter>
#include<point.h>

/*class Barrier:public IBarrier
{
public:
    /// Конструктор по умолчанию
    Barrier();
    /// Конструктор с параметрами
    Barrier(float x, float y, float z, float w, float sx, float sy, float sz);
    Barrier(const Position& position, const Scale& scale);
    /// Конструктор копирования
    Barrier(const Barrier &o);
    /// Деструктор
    ~Barrier();
    /// Позиция
    Position position;
    /// Масштабирование
    Scale scale;*/

class Barrier
{
public:
    Position position;
    Scale scale;
    Barrier():position(),scale(){}
    Barrier(float x,float y, float z, float w, float sx,float sy, float sz):position(x,y,z,w),scale(sx,sy,sz){}
    Barrier(const Position& position, const Scale& scale):position(position),scale(scale){};
    Barrier(const Barrier& barrier):position(barrier.position),scale(barrier.scale){}
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
