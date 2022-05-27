#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QDataStream>
#include <QXmlStreamWriter>

class GameMap
{
private:

public:
    float width;
    float height;
    float step;
    int center;
    GameMap():width(0),height(0),step(0),center(0){}
    GameMap(float width, float height,float step,float center):width(width),height(height),step(step),center(center){}
    GameMap(const GameMap& map):width(map.width),height(map.height),step(map.step),center(map.center){}
    ~GameMap(){}
    friend QDataStream& operator <<(QDataStream &out, const GameMap &map);
    friend QDataStream& operator >>(QDataStream &in, GameMap &map);    
};

class GameMapXMLWriter
{
private:

public:
    void print(QXmlStreamWriter& XMLWriter, const GameMap& map);
};

#endif
