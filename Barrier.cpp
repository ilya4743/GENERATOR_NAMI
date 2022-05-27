#include "Barrier.h"

QDataStream& operator <<(QDataStream &out, const Barrier &b)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out << b.position;
    out << b.scale;
    return out;
}

QDataStream& operator >>(QDataStream &in, Barrier &b)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in >> b.position;
    in >> b.scale;
    return in;
}

void BarrierXMLWriter::print(QXmlStreamWriter& XMLWriter, const Barrier& barrier)
{
    XMLWriter.writeStartElement("node");
    XMLWriter.writeAttribute("name", "Cube." + QString::number(countBarrier));

    XMLWriter.writeStartElement("position");
    XMLWriter.writeAttribute("x",QString::number(barrier.position.x));
    XMLWriter.writeAttribute("y",QString::number(barrier.position.y));
    XMLWriter.writeAttribute("z",QString::number(barrier.position.z));
    XMLWriter.writeAttribute("w",QString::number(barrier.position.w));
    XMLWriter.writeEndElement();

    XMLWriter.writeStartElement("rotation");
    XMLWriter.writeAttribute("qw","0");
    XMLWriter.writeAttribute("qx","0");
    XMLWriter.writeAttribute("qy","0");
    XMLWriter.writeAttribute("qz","0" );
    XMLWriter.writeEndElement();

    XMLWriter.writeStartElement("scale");
    XMLWriter.writeAttribute("x",QString::number(barrier.scale.x));
    XMLWriter.writeAttribute("y",QString::number(barrier.scale.y));
    XMLWriter.writeAttribute("z",QString::number(barrier.scale.z));
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
