#include"Car.h"

Car::Car():scale(),speed(0)
{

}

Car::Car(Scale scale, float speed):scale(scale), speed(speed)
{

}

Car::Car(const Car&car):scale(car.scale), speed(car.speed)
{

}

ostream& operator <<(ostream &out, const Car &car)
{
    out<<car.scale<<endl<<car.speed;
    return out;
}

istream& operator >>(istream &in, Car &car)
{
    in>>car.scale>>car.speed;
    return in;
}

QDataStream& operator <<(QDataStream &out, const Car &car)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out<<car.scale<<car.speed;
    return out;
}

QDataStream& operator >>(QDataStream &in, Car &car)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in>>car.scale>>car.speed;
    return in;
}

void CarXMLWriter::print(QXmlStreamWriter& XMLWriter, const Car& car)
{
    XMLWriter.writeStartElement("сar");
    XMLWriter.writeAttribute("speed",QString::number(car.speed));

    XMLWriter.writeStartElement("scale");
    XMLWriter.writeAttribute("x",QString::number(car.scale.x));
    XMLWriter.writeAttribute("y",QString::number(car.scale.y));
    XMLWriter.writeAttribute("z",QString::number(car.scale.z));
    XMLWriter.writeEndElement();

    XMLWriter.writeStartElement("rotation");
    XMLWriter.writeAttribute("qw","0");
    XMLWriter.writeAttribute("qx","0");
    XMLWriter.writeAttribute("qy","0");
    XMLWriter.writeAttribute("qz","0" );
    XMLWriter.writeEndElement();

    XMLWriter.writeEndElement();
}
