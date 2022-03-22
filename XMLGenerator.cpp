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
        if(name!="scene" && name!="node" && name!="rotation" && name!="entity" && name!="userData" && name !="externals" && name!="environment" && name!="colourBackground")
        {
            if(name=="position"&&isCorrect==false)
            {
                if(attrs.qName(0)=='x' && attrs.qName(1)=='y' && attrs.qName(2)=='z')
                {
                    *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat();
                    qDebug()<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat();
                    isCorrect=true;
                    return true;
                }
            }
            if( name=="scale"&&isCorrect==true)
            {
                *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat();
                qDebug()<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat();
                isCorrect=false;
                return true;
            }
            if(name=="GameMap")
            {
                *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat()<<attrs.value(3).toInt();
                return true;
            }
            if(name=="Car")
            {
                *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat();
                return true;
            }
            if(name=="CountBarriers")
            {
                *dataStream<<attrs.value(0).toInt();
                return true;
            }
            if(name=="Goal")
            {
                *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat();
                return true;
            }
            return false;
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
    if(qName!="contact"&&qName!="addressbook")
    {
        qDebug()<<"TagName:"<<qName<<"\tText:"<<m_strText;
    }
    return true;
}

bool GeneratorXMLParser::fatalError (const QXmlParseException& exception)
{
    qDebug()<<"Line: "<<exception.lineNumber()<<", Column:"<<exception.columnNumber()<<", Message:"<<exception.message();
    return false;
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
            stream>>countBarriers;
            vec_barrier.clear();
            vec_barrier.reserve(countBarriers);
            for(int j=0; j<countBarriers; j++)
            {
                stream>>x>>y;
                stream.skipRawData(4);
                stream>>width>>height;
                stream.skipRawData(4);
                vec_barrier.push_back(Barrier(x,y,width,height));
            }
        }
    }
}
