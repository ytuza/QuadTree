#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QOpenGLWindow>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>
#include <QMouseEvent>


#include "point.h"
#include "quadtree.h"

using namespace std;
class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void keyPressEvent(QKeyEvent *);

    vector<pair<float,float>> pointsW;
    vector<pair<float,float>> linesA;
    vector<pair<float,float>> linesB;
    vector<Point*> pSelec;

    virtual void paintGL();
    QuadTree *qTree ;

    void printQT(QuadTree **quad);

    float zoomScale;

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);

    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

    void findC(QuadTree **p);
    bool circleInt(Point tl, Point br);
    bool pInCircle(Point pt);

    void pushPoint(QuadTree **p);

    Point cCircle;
    float angle;

private:
    QOpenGLContext *context;
    QOpenGLFunctions *openGLFunctions;
};

#endif // MAINWINDOW_H
