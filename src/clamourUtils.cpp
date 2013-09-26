#include "clamourUtils.h"

int screenData::width = 640;
int screenData::height = 480;

int clamourUtils::rowToInt(string r){

    r = r.substr(0,1);
    int i = r[0] - 64; //convert into an integer
    return i;
}
