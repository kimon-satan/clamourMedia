#ifndef PATHFACTORY_H
#define PATHFACTORY_H

#include "ofMain.h"

class pathFactory
{
    public:

        static void createPath(ofPath &p, string shapeType, float t_size);
        static void createPath(ofPath &p, string shapeType, float x_dim, float y_dim, float t_size);
        static void mousePointer(ofPath &p, float x_dim, float y_dim, float t_size);
        static void mousePointerStraight(ofPath &p, float x_dim, float y_dim, float t_size);
        static void roundedRect(ofPath &p, float x_dim, float y_dim, float t_size);

};

#endif // PATHFACTORY_H
