#include "Barrier.h"

QDataStream& operator <<(QDataStream &out, const Barrier &b)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out << b.x;
    out << b.y;
    out << b.width;
    out << b.height;
    return out;
}

QDataStream& operator >>(QDataStream &in, Barrier &b)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in >> b.x;
    in >> b.y;
    in >> b.width;
    in >> b.height;
    return in;
}

void BarrierXMLWriter::print(QXmlStreamWriter& XMLWriter, const Barrier& barrier)
{
    XMLWriter.writeStartElement("node");
    XMLWriter.writeAttribute("name", "Cube." + QString::number(countBarrier));

    XMLWriter.writeStartElement("position");
    XMLWriter.writeAttribute("x",QString::number(barrier.x));
    XMLWriter.writeAttribute("y",QString::number(barrier.y));
    XMLWriter.writeAttribute("z","0" );
    XMLWriter.writeEndElement();

    XMLWriter.writeStartElement("rotation");
    XMLWriter.writeAttribute("qw","0");
    XMLWriter.writeAttribute("qx","0");
    XMLWriter.writeAttribute("qy","0");
    XMLWriter.writeAttribute("qz","0" );
    XMLWriter.writeEndElement();

    XMLWriter.writeStartElement("scale");
    XMLWriter.writeAttribute("x",QString::number(barrier.width));
    XMLWriter.writeAttribute("y",QString::number(barrier.height));
    XMLWriter.writeAttribute("z","1" );
    XMLWriter.writeEndElement();

    XMLWriter.writeStartElement("entity");
    XMLWriter.writeAttribute("meshFile","Cube."+ QString::number(countBarrier)+".mesh");
    XMLWriter.writeAttribute("name","Cube." + QString::number(countBarrier));
    XMLWriter.writeStartElement("userData");
    XMLWriter.writeEndElement();
    XMLWriter.writeEndElement();
    XMLWriter.writeEndElement();

    ++countBarrier;
}
