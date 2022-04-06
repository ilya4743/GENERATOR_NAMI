#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QOpenGLWidget>
#include <QtOpenGL>
#include <QTimer>
#include "spline.h"
#include "Barrier.h"
#include "Car.h"

class Visualizer :  public QOpenGLWidget
{
    Q_OBJECT
private:
    std::vector<Barrier>barriers;
    std::vector<std::pair<float,float>> path;
    QTimer *timer;
    Car car;
public:
    Visualizer(QWidget *parent = 0);
    void setBarriers(std::vector<Barrier>barriers){this->barriers=barriers;}
    void setPath(std::vector<std::pair<float,float>> path){this->path=path;}
    void setCar(const Car& car){this->car=car;};
    void paintPath();
protected:
    int wax ,way; // Размеры окна
    void initializeGL(); // Метод для инициализирования opengl
    void resizeGL(int nWidth, int nHeight); // Метод вызываемый после каждого изменения размера окна
public:
    void paintGL(); // Метод для вывода изображения на экран

public:
};

#endif // VISUALIZER_H
