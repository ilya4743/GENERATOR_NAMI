#ifndef CAR_H
#define CAR_H

#include<QDataStream>
#include<QXmlStreamWriter>

class Car
{
private:

public:
    float width;
    float height;
    Car():width(0),height(0){}
    Car(float width, float height):width(width),height(height){}
    Car(const Car& car):width(car.width), height(car.height){}
    ~Car(){}
    friend QDataStream& operator <<(QDataStream &out, const Car &car);
    friend QDataStream& operator >>(QDataStream &in, Car &car);
};

class CarXMLWriter
{
private:
public:
    void print(QXmlStreamWriter& XMLWriter, const Car& car);
};

#endif
