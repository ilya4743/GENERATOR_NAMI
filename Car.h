#ifndef CAR_H
#define CAR_H

#include<QDataStream>
#include<QXmlStreamWriter>
#include"point.h"

/// @brief Класс автомобиля
class Car
{
public:
    ///Конструктор по умолчанию
    Car();

    /// @brief Конструктор с параметрами
    /// @param width ширина авто
    /// @param height высота авто
    /// @param speed скорость авто
    Car(Scale scale, float speed);

    /// @brief Конструктор копирования
    /// @param car экземпляр авто
    /// @return экземпляр авто
    Car(const Car&car);

    /// Масштабирование
    Scale scale;

    /// Скорость авто
    float speed;

    friend ostream& operator <<(ostream &out, const Car &b);
    friend istream& operator >>(istream &in, Car &b);
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
