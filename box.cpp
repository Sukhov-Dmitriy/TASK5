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
void box::sortv()
    {   ptr v;
        ptr t;
        for(int i = 0; i< 3; i++){
                v[i] = i+1;
            v1[i] = data[i];
            v8[i] = data[i+3];
            v2[i] = data[i];
            v3[i] = data[i];
            v5[i] = data[i];
            v6[i] = data[i+3];
            v7[i] = data[i+3];
            v4[i] = data[i+3];
            }
        v2[0] = data[3];
        v5[1] = data[4];
        v3[2] = data[5];
        v4[1] = data[1];
        v7[0] = data[0];
        v6[2] = data[2];
        if(scamultip(minusp(v2,v1),v)>0){
            t=v2;v2=v1;v1=t;
            t=v4;v4=v3;v3=t;
            t=v8;v8=v7;v7=t;
            t=v6;v6=v5;v5 =t;
        }
        if(scamultip(minusp(v5,v1),v)<0){
            t=v1;v1=v5;v5=t;
            t=v2;v2=v6;v6=t;
            t=v4;v4=v8;v8=t;
            t=v3;v3=v7;v7=t;
        }
    }

box::box(const box &other){
    this->numdat = other.numdat;
}
box::box(){
    numdat = 1777;
    }
bool box::belong(ptr &point, ptr & cam)
{

        bool pol1 = false;
        bool pol2 = false;

        if(beltriangl(cam,v4,v2,v1,point)||beltriangl(cam,v3,v4,v1,point)||beltriangl(cam,v7,v4,v3,point)||beltriangl(cam,v8,v4,v7,point)||beltriangl(cam,v7,v3,v5,point)||beltriangl(cam,v5,v3,v1,point)){
            pol1 =true;
        }
        if(beltriangl(cam,v5,v2,v1,point)||beltriangl(cam,v6,v2,v5,point)||beltriangl(cam,v7,v5,v6,point)||beltriangl(cam,v8,v7,v6,point))
            pol2 = true;
            if(pol1||pol2)
                return true;
            else
                return false;
}
