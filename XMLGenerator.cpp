#include "XMLGenerator.h"

GeneratorXMLParser::GeneratorXMLParser()
{
    dataStream=new QDataStream(&data,QIODevice::WriteOnly);
    dataStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
    dataStream->setByteOrder(QDataStream::LittleEndian);
}

GeneratorXMLParser::~GeneratorXMLParser()
{
    delete dataStream;
}

bool GeneratorXMLParser::startElement (const QString& namespaceURI, const QString& localName, const QString& name, const QXmlAttributes& attrs)
{
    if(name=="node" && attrs.value(0)[0]=='C'&&attrs.value(0)[1]=='u'&&attrs.value(0)[2]=='b'&&attrs.value(0)[3]=='e')
    {
        ++countBracket;
        return true;
    }
    else if(countBracket>0)
    {
        if(name=="position")
        {
            if(attrs.qName(0)=='x' && attrs.qName(1)=='y' && attrs.qName(2)=='z')
            {
                *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat();
                pos++;
                return true;
            }
        }
        if( name=="scale")
        {
            *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat();
            scale++;
            return true;
        }
            return true;
    }
    else
    {
        if(name=="GameMap")
        {
            *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat()<<attrs.value(3).toInt();
            isMap=true;
            return true;
        }
        if(name=="Car")
        {
            *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat();
            isCar=true;
            return true;
        }
        if(name=="Goal")
        {
            *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat();
            isGoal=true;
            return true;
        }
    }
    return true;
}

bool GeneratorXMLParser::characters(const QString& strText)
{
    m_strText=strText;
    return true;
}

bool GeneratorXMLParser::endElement(const QString& namespaceURI, const QString& localName, const QString& qName)
{
    if(qName=="node"&&countBracket>0)
    {
        --countBracket;
        int pos1=pos;
        int scale1=scale;
        pos--;
        scale--;
        return (pos1==scale1&&(pos1!=0&&scale1!=0));
    }
    return true;
}

bool GeneratorXMLParser::fatalError (const QXmlParseException& exception)
{
    qDebug()<<"Line: "<<exception.lineNumber()<<", Column:"<<exception.columnNumber()<<", Message:"<<exception.message();
    return false;
}

bool GeneratorXMLParser::endDocument()
{
    return isGoal&&isMap&&isCar;
}

void XMLGenerator::exportXML(const QString& str, const GameMap& map, const Car& car, const Point& goal_point, const std::vector<Barrier>& vec_barrier)
{
    QFile file(str);
    if(file.open(QIODevice::WriteOnly))
    {
        QXmlStreamWriter XMLWriter(&file);
        XMLWriter.setAutoFormatting(true);
        XMLWriter.writeStartDocument();
        XMLWriter.writeStartElement("scene");
        XMLWriter.writeAttribute("author", "suvairin" );
        XMLWriter.writeAttribute("formatVersion", "1.1");
        GameMapXMLWriter mapXML;
        mapXML.print(XMLWriter, map);
        CarXMLWriter carXML;
        carXML.print(XMLWriter, car);
        XMLWriter.writeStartElement("Goal");
        XMLWriter.writeAttribute("x", QString::number(goal_point.x));
        XMLWriter.writeAttribute("y", QString::number(goal_point.y));
        XMLWriter.writeEndElement();
        XMLWriter.writeStartElement("CountBarriers");
        XMLWriter.writeAttribute("n", QString::number(vec_barrier.size()));
        XMLWriter.writeEndElement();
        BarrierXMLWriter barrierXMLWriter;
        for(unsigned int i=0; i<vec_barrier.size();i++)
            barrierXMLWriter.print(XMLWriter, vec_barrier[i]);

        XMLWriter.writeStartElement("externals");
        XMLWriter.writeEndElement();
        XMLWriter.writeStartElement("environment");
        XMLWriter.writeStartElement("colourBackground");
        XMLWriter.writeAttribute("b", "0.050876");
        XMLWriter.writeAttribute("g", "0.050876");
        XMLWriter.writeAttribute("r", "0.050876");
        XMLWriter.writeEndElement();
        XMLWriter.writeEndElement();

        XMLWriter.writeEndElement();
    }
    file.close();
}

void XMLGenerator::importXML(QString& str, GameMap& map, Car& car, Point& goal_point, std::vector<Barrier>& vec_barrier, int& countBarriers)
{
    GeneratorXMLParser handler;
    QFile file(str);
    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
        QXmlInputSource source(&file);
        QXmlSimpleReader reader;
        reader.setContentHandler(&handler);
        reader.setErrorHandler(&handler);
        if(reader.parse(source))
        {
            float x, y, width, height;
            GameMap m;
            Car c;
            QDataStream stream(&handler.data, QIODevice::ReadOnly);
            stream>>map;
            stream>>car;
            stream>>goal_point;
            vec_barrier.clear();
            while(!stream.atEnd())
            {
                stream>>x>>y;
                stream.skipRawData(4);
                stream>>width>>height;
                stream.skipRawData(4);
                vec_barrier.push_back(Barrier(x,y,width,height));
                ++countBarriers;
            }
        }
    }
}
