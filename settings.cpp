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

settings::settings(ifstream &input){
        string c;
        input>>c;
        double qa;
        for(int i =0; i<2; i++){
            input>>qa;
            cam[i] = qa;
        }
        input>>qa;
        cam[2] = -qa;
        input>>c;
        for(int i =0; i<3; i++){
            input>>qa;
            target[i] = qa;
        }
        input>>c;
        for(int i =0; i<3; i++){
            input>>qa;
            up[i] = qa;
        }
        input>>c;
        input>>screen;
        screen = screen/1000;
        input>>c;
        input>>limit;
        limit = limit/1000;
        input>>c;
        input>>alpha;
        input>>c;
        input>>wight;
        input>>c;
        input>>height;
    }
