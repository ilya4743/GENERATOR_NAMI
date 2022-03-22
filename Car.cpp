#include"Car.h"

QDataStream& operator <<(QDataStream &out, const Car &car)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out << car.width;
    out << car.height;
    return out;
}

QDataStream& operator >>(QDataStream &in, Car &car)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in >> car.width;
    in >> car.height;
    return in;
}

void CarXMLWriter::print(QXmlStreamWriter& XMLWriter, const Car& car)
{
    XMLWriter.writeStartElement("Car");
    XMLWriter.writeAttribute("width",QString::number(car.width));
    XMLWriter.writeAttribute("height",QString::number(car.height));
    XMLWriter.writeEndElement();
}
