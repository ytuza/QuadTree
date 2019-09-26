#include "mainwindow.h"
#include "point.h"
#include <QtMath>

bool MainWindow::circleInt(Point tl, Point br){

    //qDebug() << tl.x << "," << tl.y << "=" << br.x << "," << br.y;

    float distanceX,distanceY;
    Point centerR((tl.x+br.x)/2,(tl.y+br.y)/2);
    float width = abs(tl.x-br.x);
    float height = abs(tl.y-br.y);

    //qDebug() << centerR.x << " {+} " << centerR.y;

    distanceX = abs(cCircle.x - centerR.x);
    distanceY = abs(cCircle.y - centerR.y);

    //qDebug() << distanceX << " {-} " << distanceY;

    if (distanceX > (width/2 + angle)) { return false; }
    if (distanceY > (height/2 + angle)) { return false; }

    //qDebug() << "no esta lejos " ;

    if (distanceX <= (width/2)) { return true; }
    if (distanceY <= (height/2)) { return true; }

    //qDebug() << " no es menor a la mitad " << endl;

    float cornerDistance = qPow((distanceX - width/2),2) +
                           qPow((distanceY - height/2),2);

    return (cornerDistance <= qPow(angle,2));
}

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius){
    int i;
    int lineAmount = 100; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * M_PI;

    glBegin(GL_LINE_LOOP);
    glColor3d(0,0,255);
        for(i = 0; i <= lineAmount;i++) {
            glVertex2f(
                x + (radius * cos(i *  twicePi / lineAmount)),
                y + (radius* sin(i * twicePi / lineAmount))
            );
        }
    glEnd();
}

MainWindow::MainWindow(QWidget *parent)
{

    setSurfaceType(QWindow::OpenGLSurface);


    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2,1);

    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();

    context->makeCurrent(this);
    openGLFunctions = context->functions();

    //qDebug() << "lo creare" ;
    Point al(-1.0,1.0);
    Point bl(1.0,-1.0);

    qTree = nullptr;
    qTree = new QuadTree(al,bl);

    cCircle.x = 0.0;
    cCircle.y = 0.0;
    angle= 0.2;

    zoomScale = 1.0;


}

MainWindow::~MainWindow()
{
    delete qTree;

}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    //qDebug() << ev->x() << " - " << ev-> y();

    cCircle.x = float(ev->x())/float(this->width())-0.5;
    cCircle.y = float(ev->y())/float(this->height())-0.5;
    cCircle.x *= 2;
    cCircle.y *= -2;

    update();
}

void MainWindow::wheelEvent(QWheelEvent *ev){


    angle += (ev->delta()/3000.0);
    if(angle < 0) angle = 0.0001;
    update();

    //QPoint numDegrees = ev->angleDelta();
    //if (numDegrees.y() < 0)  zoomScale = zoomScale/1.1;
    //if (numDegrees.y() > 0)  zoomScale = zoomScale*1.1;
    //update(); // call paintGL()



}

bool MainWindow::pInCircle(Point pt){
    if(qSqrt(qPow(pt.x-cCircle.x,2)+qPow(pt.y-cCircle.y,2))<=angle){
        return true;
    }
    return false;
}

void MainWindow::pushPoint(QuadTree **p)
{
    if((*p)->leaf){
        for(int i=0; i<(*p)->data.size();i++){
            pSelec.push_back((*p)->data[i]);
        }
    }else{
        pushPoint(&(*p)->northEast);
        pushPoint(&(*p)->southEast);
        pushPoint(&(*p)->northWest);
        pushPoint(&(*p)->southWest);
    }
}

void MainWindow::findC(QuadTree **p){
    if(circleInt((*p)->topLeft,(*p)->botRight)){
        //qDebug() << (*p)->topLeft.x << "," << (*p)->topLeft.y << "}{" << (*p)->botRight.x << "," << (*p)->botRight.y;

        if(pInCircle((*p)->topLeft)  and pInCircle((*p)->botRight)){
            pushPoint(p);
            return;
        }
        if((*p)->leaf){
            for(int i=0;i<(*p)->data.size();i++){
                if(pInCircle(*(*p)->data[i]))
                    pSelec.push_back((*p)->data[i]);
            }
            return;
        }else{
            findC(&(*p)->northEast);
            findC(&(*p)->southEast);
            findC(&(*p)->northWest);
            findC(&(*p)->southWest);

        }
    }


}

void MainWindow::keyPressEvent(QKeyEvent *ev){
    //qDebug() << "key: " << ev->key();
    if(ev->key() == 70){

        pSelec.clear();

        //qDebug() << "buscar";
        findC(&qTree);

        /*
        for(int i=0;i<pSelec.size();i++){
            qDebug() << pSelec[i]->x << " - " << pSelec[i]->y ;
        }
        */
        //qDebug() << "----" <<pSelec.size()<< " puntos." ;
        update();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{

    float x_p = float(ev->x())/float(this->width())-0.5;
    float y_p = float(ev->y())/float(this->height())-0.5;

    pointsW.push_back(pair<float,float>(x_p*2,y_p*-2));

    qTree->insert(new Point(x_p*2,y_p*-2));


    linesA.clear();
    linesB.clear();



    printQT(&qTree);
    //paintGL();
    update();

}

void MainWindow::initializeGL()
{
    resizeGL(this->width(),this->height());

}


void MainWindow::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,w,h);

   qreal aspectratio = qreal(w)/qreal(h);
   glOrtho(-1* aspectratio,1*aspectratio,-1,1,1,-1);


}



void MainWindow::paintGL()
{

    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    //glScaled(zoomScale, zoomScale, zoomScale);

    glPointSize(2);


    //gluPerspective(45.0,1.0,1.0,100.0);
   //glTranslatef(0,0,-2);
    //glRotatef(70,,0,0);


    glBegin(GL_POINTS);
    glColor3d(255,255,255);
    for(int i=0;i<int(pointsW.size());i++){
        //qDebug() <<pointsW[i].first<< "paint " << pointsW[i].second<< "size" <<pointsW.size() ;
        glVertex2d(pointsW[i].first,pointsW[i].second);
    }
    glEnd();




    glBegin(GL_POINTS);
    glColor3d(0,0,255);
    for(int i=0;i<int(pSelec.size());i++){

        glVertex2d(pSelec[i]->x,pSelec[i]->y);
    }
    glEnd();




    glBegin(GL_LINES);
    glColor3d(255,0,0);
    //qDebug() <<" gh "<<linesA.size() ;

    for(int i=0;i<int(linesA.size());i++){

        glVertex2d(linesA[i].first,linesA[i].second);
        glVertex2d(linesB[i].first,linesB[i].second);
    }

    glEnd();

    //qDebug() << x_m << " pa " << y_m ;line
    drawHollowCircle(cCircle.x,cCircle.y,angle);

    glFlush();


}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeGL(this->width(),this->height());
    this->update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    paintGL();

}

void MainWindow::printQT(QuadTree **quad)
{


    if(!((*quad)->leaf)){
        float midH = (((*quad)->topLeft).x + ((*quad)->botRight).x)/2;
        float midV = (((*quad)->topLeft).y + ((*quad)->botRight).y)/2;

        linesA.push_back(pair<float,float>(((*quad)->topLeft).x,midV));
        linesB.push_back(pair<float,float>(((*quad)->botRight).x,midV));
        linesA.push_back(pair<float,float>(midH,((*quad)->topLeft).y));
        linesB.push_back(pair<float,float>(midH,((*quad)->botRight).y));

            printQT(&((*quad)->northEast));
            printQT(&((*quad)->southEast));
            printQT(&((*quad)->northWest));
            printQT(&((*quad)->southWest));

    }
}
