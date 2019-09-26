#ifndef QUADTREE_H
#define QUADTREE_H
#include "point.h"
#include <vector>
#include <QtDebug>
using namespace std;
class QuadTree
{
public:
    QuadTree(){
        topLeft = Point(-1,1);
        botRight = Point(1,-1);
        leaf = true;
    }
    QuadTree(Point tl, Point br){
        //qDebug() << "se creo " ;
        topLeft = tl;
        botRight = br;
        leaf = true;
    }
    ~QuadTree(){
        for(int i = 0;i<data.size();i++)
            delete data[i];
        if(northWest){
            delete northWest;
            delete southWest;
            delete northEast;
            delete southEast;
        }
    }

    QuadTree *northWest;
    QuadTree *southWest;
    QuadTree *northEast;
    QuadTree *southEast;

    bool find(Point *pt, QuadTree **pQT ){
        if(leaf){
            *pQT = this;

            for(int i=0;i<data.size();i++){
                if(data[i]->x == pt->x && data[i]->y == pt->y){
                    return true;
                }
            }
            return false;
        }
        return ubic(pt)->find(pt,pQT);
    }

    QuadTree* ubic(Point *p){
        float midH = (topLeft.x + botRight.x)/2,midV = (topLeft.y + botRight.y)/2;
        if(p->x<= midH){
            if(p->y <= midV){
                return southWest;

            }else{
                return northWest;
            }
        }else{
            if(p->y <= midV){
                return southEast;
            }else{
                return northEast;
            }

        }
    }

    bool insert(Point *pt)
    {
        //qDebug() << "comenzo" ;

        QuadTree *pQT = nullptr;

        if(find(pt,&pQT)){
            //qDebug() << "ya esta " ;
            return false;
        }

        if(pQT==nullptr) return false;

        if( pQT->stopCond() ){

            pQT->leaf = false;

            float midH = (pQT->topLeft.x + pQT->botRight.x)/2,midV = (pQT->topLeft.y + pQT->botRight.y)/2;

            pQT->northWest = new QuadTree(pQT->topLeft,Point(midH,midV));
            pQT->southWest = new QuadTree(Point((pQT->topLeft).x,midV),Point(midH,(pQT->botRight).y));
            pQT->northEast = new QuadTree(Point(midH,(pQT->topLeft).y),Point((pQT->botRight).x,midV));
            pQT->southEast = new QuadTree(Point(midH,midV),pQT->botRight );
            for(int i = 0;i<int(pQT->data.size());i++){
                (pQT->ubic(pQT->data[i]))->insert(pQT->data[i]);
                //(ubic(data[i])->data).push_back(data[i]);
            }
            (pQT->ubic(pt))->insert(pt);

            (pQT->data).clear();

        }else{
            (pQT->data).push_back(pt);
        }


        return true;
    }

    void printQ(){
        qDebug() << data.size() << " tamanio";
    }

    bool leaf;
    Point topLeft;
    Point botRight;
    vector<Point*> data;

private:

    bool stopCond(){
        if(data.size() >= 50){
            return true;
        }
        return false;

    }
};



#endif // QUADTREE_H
