#define EPS 0.00000000001
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

sphere::sphere(const sphere &other){
        this->numdat = other.numdat;
}
sphere::sphere(){
    numdat = 1777;
}
bool sphere::belong(ptr &point, ptr &cam)
    {
        numdat = cam[2];
       double r = pow((point[0]-data[0]),2)+pow((point[1]-data[1]),2)+pow((point[2]-data[2]),2)-pow(data[3],2);
        if(r<EPS)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void sphere::sortv()
    {
        numdat = 1777;
    }
