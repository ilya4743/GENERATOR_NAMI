#include "Visualizer.h"

Visualizer::Visualizer(QWidget* parent) : QOpenGLWidget(parent)
{
    wax=500; way=500; // начальный размер окна
    QSurfaceFormat form;
    form.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    setFormat(form); // Двойная буферизация
    glDepthFunc(GL_LEQUAL); // Буфер глубины
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(paintGL()));
    timer->start(750);
}

void Visualizer::initializeGL()
{
    glClearColor(0,0,0,1); // Черный цвет фона
}

void Visualizer::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    wax=nWidth;
    way=nHeight;
}

void create_time_grid(std::vector<double>& T, double& tmin, double& tmax,
                      std::vector<double>& X, std::vector<double>& Y, bool is_closed_curve)
{

    assert(X.size()==Y.size() && X.size()>2);

    int idx_first=-1, idx_last=-1;
    if(is_closed_curve) {
        if(X[0]==X.back() && Y[0]==Y.back()) {
            X.pop_back();
            Y.pop_back();
        }

        const int num_loops=3;  // number of times we go through the closed loop
        std::vector<double> Xcopy, Ycopy;
        for(int i=0; i<num_loops; i++) {
            Xcopy.insert(Xcopy.end(), X.begin(), X.end());
            Ycopy.insert(Ycopy.end(), Y.begin(), Y.end());
        }
        idx_last  = (int)Xcopy.size()-1;
        idx_first = idx_last - (int)X.size();
        X = Xcopy;
        Y = Ycopy;

        // add first point to the end (so that the curve closes)
        X.push_back(X[0]);
        Y.push_back(Y[0]);
    }
    T.resize(X.size());
    T[0]=0.0;
    for(size_t i=1; i<T.size(); i++) {
        // time is proportional to the distance, i.e. we go at a const speed
        T[i] = T[i-1] + sqrt( (X[i]-X[i-1])*(X[i]-X[i-1]) + (Y[i]-Y[i-1])*(Y[i]-Y[i-1]) );
    }
    if(idx_first<0 || idx_last<0) {
        tmin = T[0] - 0.0;
        tmax = T.back() + 0.0;
    } else {
        tmin = T[idx_first];
        tmax = T[idx_last];
    }
}

void Visualizer::paintPath()
{

}

void Visualizer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины
    glMatrixMode(GL_PROJECTION); // устанавливаем матрицу
    glLoadIdentity(); // загружаем матрицу
    glOrtho(0,wax,way,0,1,0); // подготавливаем плоскости для матрицы
    if(path.size()>0)
    {
        glBegin(GL_LINE_STRIP);
        glColor3f(1,1,1);

        std::vector<double> X; // must be increasing
        std::vector<double> Y;
        X.reserve(path.size());
        Y.reserve(path.size());
        for(int i=0; i<path.size();i++)
        {
            X.push_back(path[i].first);
            Y.push_back(path[i].second);
            X[i]=wax/2+X[i];
            Y[i]=way-Y[i];
        }
        std::vector<double> T;
        double tmin = 0.0, tmax = 0.0;
        create_time_grid(T,tmin,tmax,X,Y,false);
        tk::spline sx, sy;
        sx.set_points(T,X,tk::spline::cspline);
        sy.set_points(T,Y,tk::spline::cspline);
        int n=1000;
        for(int i=0; i<1000; i++)
        {
            double t = tmin + (double)i*(tmax-tmin)/(n-1);
            glVertex2d(sx(t),sy(t));
        }
        glEnd();
        glPointSize(5);

        glBegin(GL_POINTS);
        glColor3f(1,0,0);
        for(int i=0; i<X.size();i++)
            glVertex2d(X[i],Y[i]);
        glEnd();

    }
    glPointSize(1);

    glBegin(GL_QUADS);
    glColor3f(0,0,1);
    for(int i=0; i<barriers.size();i++)
    {
        glVertex2d(wax/2+barriers[i].x-barriers[i].width/2-car.width/2, way-barriers[i].y+barriers[i].height/2+car.height/2);
        glVertex2d(wax/2+barriers[i].x-barriers[i].width/2-car.width/2, way-barriers[i].y-barriers[i].height/2-car.height/2);
        glVertex2d(wax/2+barriers[i].x+barriers[i].width/2+car.width/2, way-barriers[i].y-barriers[i].height/2-car.height/2);
        glVertex2d(wax/2+barriers[i].x+barriers[i].width/2+car.width/2, way-barriers[i].y+barriers[i].height/2+car.height/2);
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0,1,0);
    for(int i=0; i<barriers.size();i++)
    {
        glVertex2d(wax/2+barriers[i].x-barriers[i].width/2, way-barriers[i].y+barriers[i].height/2);
        glVertex2d(wax/2+barriers[i].x-barriers[i].width/2, way-barriers[i].y-barriers[i].height/2);
        glVertex2d(wax/2+barriers[i].x+barriers[i].width/2, way-barriers[i].y-barriers[i].height/2);
        glVertex2d(wax/2+barriers[i].x+barriers[i].width/2, way-barriers[i].y+barriers[i].height/2);
    }
    glEnd();
}
