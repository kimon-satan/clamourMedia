#ifndef ZONERENDERER_H
#define ZONERENDERER_H

#include "zone.h"


class zoneRenderer
{
public:
    zoneRenderer();

    void update();
    void draw(map<string, ofPtr<zone> > z);
    void drawDebug(ofPtr<zone> z, baseData &bd);
    void simpleButton(ofPtr<zone> z, baseData &bd);


    void stopSymbol(float s);
    void pauseSymbol(float s, float w);
    void playSymbol(float s);
    void revPlaySymbol(float s);
    void ffwdSymbol(float s, float w);
    void rrwdSymbol(float s, float w);
    void ejectSymbol(float s, float w);

    //getters and setters

    virtual ~zoneRenderer();


protected:
private:


};

#endif // ZONERENDERER_H
