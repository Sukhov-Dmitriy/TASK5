#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <omp.h>
#include "CImg.h"
#include"functions1.h"
using namespace std;
tetra::tetra(const tetra &other){
        this->numdat = other.numdat;
    }
tetra::tetra()
{
    numdat = 12;
}
void tetra::sortv()
    {
        for(int i = 0;i < 3; i++)
        {
            v1[i] = data[i];
        }
         for(int i = 0;i < 3; i++)
        {
            v2[i] = data[i+3];
        }
         for(int i = 0;i < 3; i++)
        {
            v3[i] = data[i+6];
        }
        for(int i = 0;i < 3; i++)
        {
            v4[i] = data[i+9];
        }
        if(scamultip(minusp(v4,v1),vecmultip(minusp(v3,v1),minusp(v2,v1)))<0)
        {
            ptr t;
            t = v3;
            v3 = v1;
            v1 = t;
        }
    }
    bool tetra::belong(ptr& point,ptr &cam)
    {
        bool gr1;
        bool gr2;
        bool gr3;
        bool gr4;
        gr1 = beltriangl(cam, v2,v1,v4, point);
        gr2 = beltriangl(cam, v2,v3,v4, point);
        gr3 = beltriangl(cam, v3,v1,v4, point);
        gr4 = beltriangl(cam, v2,v1,v3, point);
        if(gr1 || gr2 || gr3 || gr4)
        {
            return true;
        }
        else
            return false;
    }
