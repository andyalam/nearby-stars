#include <iostream>
using namespace std;

#include "StarNearEarth.h"
#include "FHsparseMat.h"
#include <cmath>

class SNE_Analyzer : public StarNearEarth
{
private:
    double x, y, z;

public:
    void calcCartCoords();
    double getX() { return x; }
    double getY() { return y; }
    double getZ() { return z; }
    string coordToString();
    SNE_Analyzer & operator=(const StarNearEarth &sne);
};

void SNE_Analyzer::calcCartCoords()
{
    double LY = 3.262 / this->getParallaxMean();
    double convertToRad = 3.14 / 180;


    x = LY * cos(this->getDec() * convertToRad) * cos(this->getRAsc() * convertToRad);
    y = LY * cos(this->getDec() * convertToRad) * sin(this->getRAsc() * convertToRad);
    z = LY * sin(this->getDec() * convertToRad);
}
string SNE_Analyzer::coordToString()
{
    return to_string(x) + "; " + to_string(y) + "; " + to_string(z);
}

SNE_Analyzer & SNE_Analyzer::operator=(const StarNearEarth &sne)
{
    // done in modules
    setRank(sne.getRank());
    setNameCns(sne.getNameCns());
    setNumComponents(sne.getNumComponents());
    setNameLhs(sne.getNameLhs());
    setRAsc(sne.getRAsc());
    setDec(sne.getDec());
    setPropMotionMag(sne.getPropMotionMag());
    setPropMotionDir(sne.getPropMotionDir());
    setParallaxMean(sne.getParallaxMean());
    setParallaxVariance(sne.getParallaxVariance());
    setWhiteDwarfFlag(sne.getWhiteDwarfFlag());
    setSpectralType(sne.getSpectralType());
    setMagApparent(sne.getMagApparent());
    setMagAbsolute(sne.getMagAbsolute());
    setMass(sne.getMass());
    setNotes(sne.getNotes());
    setNameCommon(sne.getNameCommon());
    calcCartCoords();

    return *this;
}