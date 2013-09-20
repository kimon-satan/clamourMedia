#ifndef ZONERENDERER_H
#define ZONERENDERER_H

#include "zoneManager.h"

class zoneRenderer
{
public:
    zoneRenderer();

    void update();
    void draw(ofPtr<zone> z);

    //getters and setters

    void setScreenDims(int width, int height);

    virtual ~zoneRenderer();


protected:
private:

    int screenWidth;
    int screenHeight;

};

#endif // ZONERENDERER_H
