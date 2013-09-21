#ifndef ZONERENDERER_H
#define ZONERENDERER_H

#include "zone.h"


class zoneRenderer
{
public:
    zoneRenderer();

    void update();
    void draw(map<string, ofPtr<zone> > z);

    //getters and setters

    void setScreenDims(int width, int height);

    virtual ~zoneRenderer();


protected:
private:


};

#endif // ZONERENDERER_H
