#include "GameMap.h"

QDataStream& operator <<(QDataStream &out, const GameMap &map)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out << map.width;
    out << map.height;
    out << map.step;
    out << map.center;
    return out;
}

QDataStream& operator >>(QDataStream &in, GameMap &map)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in >> map.width;
    in >> map.height;
    in >> map.step;
    in >> map.center;
    return in;
}

void GameMapXMLWriter::print(QXmlStreamWriter& XMLWriter, const GameMap& map)
{
    XMLWriter.writeStartElement("GameMap");
    XMLWriter.writeAttribute("width",QString::number(map.width));
    XMLWriter.writeAttribute("height",QString::number(map.height));
    XMLWriter.writeAttribute("step",QString::number(map.step));
    XMLWriter.writeAttribute("center",QString::number(map.center));
    XMLWriter.writeEndElement();
}
