#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QTextStream>
#include "point.h"



void Read(QString Filename,QVector<float> &vector){
    QFile mFile(Filename);

    if(!mFile.open(QFile::ReadOnly | QFile:: Text)){
        qDebug() << "no pude";
        return;
    }

    QTextStream in(&mFile);
    QString mText = in.readAll();
    mText.remove(' ');


    QStringList strList = mText.split(QRegExp("[,]"),QString::SkipEmptyParts);


    for(int i=0; i<strList.length(); i++)
    {
        vector.push_back(strList[i].toFloat());
    }

    mFile.close();

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    QVector<float> dat;

    QString mFilename = "C:/Users/Acer/Documents/QuadTree2/tt.txt";

    Read(mFilename,dat);
    float aum = 4.5;



    for(int i=0; i<dat.size()-2; i=i+2)
    {
        w.qTree->insert(new Point(aum*(dat[i]+87.7),aum*(dat[i+1]-41.8)));
        w.pointsW.push_back(pair<float,float>(aum*(dat[i]+87.7),aum*(dat[i+1]-41.8)));
    }

    w.paintGL();

    w.setTitle("OpenGL QT");
    w.resize(680,680);
    w.show();

    return a.exec();
}
