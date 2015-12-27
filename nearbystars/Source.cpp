#include <iostream>
#include <cmath>
using namespace std;

#include "StarNearEarth.h"
#include "FHsparseMat.h"
#include "SNE_Analyzer.h"

// --------------- main ---------------
int main()
{

    int k, arraySize, row, col;
    double maxX, minX, maxY, minY, maxZ, minZ,
        xRange, yRange, zRange,
        xConversion, yConversion, zConversion;
    StarNearEarthReader starInput("nearest_stars.txt");
    const int NUM_COLS = 70;
    const int NUM_ROWS = 35;

    if (starInput.readError())
    {
        cout << "couldn't open " << starInput.getFileName() << " for input.\n";
        exit(1);
    }

    cout << starInput.getFileName() << endl;
    cout << starInput.getNumStars() << endl;

    // create an array of objects for our own use:
    arraySize = starInput.getNumStars();
    SNE_Analyzer *starArray = new SNE_Analyzer[arraySize];
    for (k = 0; k < arraySize; k++)
        starArray[k] = starInput[k];

    // display cartesian coords
    for (int k = 0; k < arraySize; k++)
        cout << starArray[k].getNameCommon() << " "
        << starArray[k].coordToString() << endl;
    



    /**********************************************************************/


    // get max and min coords for scaling
    maxX = minX = maxY = minY = maxZ = minZ = 0;
    
    //better safe than sorry, initialize all vars to first (X,Y,Z) 
    minX = starArray[0].getX();
    minY = starArray[0].getY();
    minZ = starArray[0].getZ();
    maxX = starArray[0].getX();
    maxY = starArray[0].getY();
    maxZ = starArray[0].getZ();

    int currentX, currentY, currentZ;

    for (int k = 0; k < arraySize; k++)
    {
        currentX = starArray[k].getX();
        currentY = starArray[k].getY();
        currentZ = starArray[k].getZ();

        if (currentX > maxX)
            maxX = currentX;

        if (currentX < minX)
            minX = currentX;

        if (currentY > maxY)
            maxY = currentY;

        if (currentY < minY)
            minY = currentY;

        if (currentZ > maxZ)
            maxZ = currentZ;

        if (currentZ < minZ)
            minZ = currentZ;
    }
    xRange = maxX - minX;
    yRange = maxY - minY;
    zRange = maxZ - minZ;

    cout << "MaxX:" << xRange << " MaxY:" << yRange << " MaxZ:" << zRange << endl;

    xConversion = xRange / (double)NUM_ROWS;
    yConversion = yRange / (double)NUM_COLS;

    SparseMat<char> starMap(NUM_ROWS, NUM_COLS, ' ');
    char convertRank;

    //calculate XY plain
    for (k = 0; k < arraySize; k++)
    {
        row = (starArray[k].getX() -minX) / xConversion;
        col = (starArray[k].getY() -minY) / yConversion;

        if (starArray[k].getRank() < 10)
            starMap.set(row, col, to_string(starArray[k].getRank())[0]);
        else
            starMap.set(row, col, '*');
    }

    //these are for the sun
    row = -minX / xConversion;
    col = -minY / yConversion;
    starMap.set(row, col, 'S');
    
    //show x/y plain
    cout << "\n\n\nXY Projection: \n";
    for (int r = 0; r < NUM_ROWS; r++) {
        for (int c = 0; c < NUM_COLS; c++)
            cout << starMap.get(r, c);
        cout << endl;
    }



    //ZY calc/display below
    /***********************************************************/


    starMap.clear();
    //these are for the sun
    row = round(-minX / xConversion);
    col = round(-minY / yConversion);
    starMap.set(row, col, 'S');

    //new conversion
    yConversion = yRange / (double)NUM_ROWS;
    zConversion = zRange / (double)NUM_COLS;

    //calculate YZ plain
    for (k = 0; k < arraySize; k++)
    {
        row = (starArray[k].getY() - minY) / yConversion;
        col = (starArray[k].getZ() - minZ) / zConversion;

        if (starArray[k].getRank() < 10)
            starMap.set(row, col, to_string(starArray[k].getRank())[0]);
        else
            starMap.set(row, col, '*');
    }

    //show YZ plain
    cout << "\n\n\nYZ Projection: \n";
    for (int r = 0; r < NUM_ROWS; r++) {
        for (int c = 0; c < NUM_COLS; c++)
            cout << starMap.get(r, c);
        cout << endl;
    }

    delete[] starArray;
    system("pause");
    return 0;
}