#ifndef XMLGENERATOR_H
#define XMLGENERATOR_H

#include<QXmlStreamWriter>
#include <QXmlAttributes>
#include <QXmlDefaultHandler>
#include <QDataStream>
#include <QDebug>
#include "GameMap.h"
#include "Car.h"
#include "Barrier.h"
#include "point.h"

class GeneratorXMLParser : public QXmlDefaultHandler
{
private:
    QString m_strText;
    bool isCorrect=false;
    QDataStream *dataStream;
    unsigned int countBracket=0;
    bool isCar=false;
    bool isMap=false;
    bool isGoal=false;
    bool isPosition=false;
    bool isScale=false;
    int pos=0;
    int scale=0;
public:
    QByteArray data;
    GeneratorXMLParser();
    ~GeneratorXMLParser();
    bool startElement (const QString& namespaceURI, const QString& localName, const QString& name, const QXmlAttributes& attrs);
    bool characters(const QString& strText);
    bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName);
    bool fatalError (const QXmlParseException& exception);
    bool endDocument();
};

class XMLGenerator
{
private:
public:
    static void exportXML(const QString& str, const GameMap& map, const Car& car, const Position& goal_point, const std::vector<Barrier>& vec_barrier);
    static void importXML(QString& str, GameMap& map, Car& car, Position& goal_point, std::vector<Barrier>& vec_barrier, int& countBarriers);
};

#endif
