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
ptr scapower(ptr &A, double a)
{
    ptr res;
    for(int i = 0; i < 3; i++)
    {
        res[i] = A[i]*a;
    }
    return res;
}


ptr minusp(ptr &A, ptr &B)
{
    ptr res;
    for(int i = 0; i < 3; i++)
    {
        res[i] = (A[i]-B[i]);
    }
    return res;
}
ptr plusp(ptr &A, ptr &B)
{
    ptr res;
    for(int i = 0; i < 3; i++)
    {
        res[i] = (A[i]+B[i]);
    }
    return res;
}
ptr vecmultip(ptr A,ptr B)
{
    ptr res;
    res[0] = (A[1]*B[2]-B[1]*A[2]);
    res[1] = (A[2]*B[0]-B[2]*A[0]);
    res[2] = (A[0]*B[1]-B[0]*A[1]);
    return res;
}

double scamultip(ptr A,ptr B)
{
    return (A[0]*B[0]+A[1]*B[1]+A[2]*B[2]);
}
double norma(ptr A)
{
    return sqrt(A[0]*A[0] + A[1]*A[1] + A[2]*A[2]);
}
bool beltriangl(ptr& v1,ptr &v2,ptr &v3,ptr &v4, ptr &point) {
        double q1;
        double q2;
        double q3;
        double q4;
        q1 = scamultip(vecmultip(minusp(v3,v1),minusp(v2,v1)),minusp(point,v1));
        q2 = scamultip(vecmultip(minusp(v4,v1),minusp(v3,v1)),minusp(point,v1));
        q3 = scamultip(vecmultip(minusp(v2,v1),minusp(v4,v1)),minusp(point,v1));
        q4 = scamultip(vecmultip(minusp(v2,v4),minusp(v3,v4)),minusp(point,v4));
        if(q1 > 0 && q2 > 0 && q3 > 0 && q4 < 0)
        {
            return true;
        }
        else
            return false;
}

FFigure *CreateData(ifstream &input, map<string,FigureFactory*>  f){
    string I;
    FFigure *fig = NULL;
    input >> I;
    fig = f[I]->create_dat();
    if(I == "tetra"){
        for(int j = 0; j < 12; j++){
            input >> fig->data[j];
        }
        fig->numtype = 3;
        fig->sortv();
    }
    if(I == "box"){
        for(int j = 0; j < 6; j++){
            input >>fig->data[j];
        }
        fig->numtype = 2;
        fig->sortv();
    }
    if(I == "sphere"){
        for(int j = 0; j < 4; j++){
            input >>fig->data[j];
        }
        fig->numtype = 1;
    }
    fig->type = I;
    return fig;
}

int check_f(ifstream &fl){
    if (!fl.is_open()) // если файл не открыт
    {
        cout << "File isn't open!\n";
        return -1;
    }
    return 0;
}

float colorfig(ptr &targ, vector<FFigure*> &arr, settings *sett)
{
    int i = 0;
    int j =0;
    float grad;
    double lima;
    bool toto = false;
    double al = 3.1415*(sett->alpha/360);
    double kof = 250*(((sin(al)/cos(al))*(sett->screen))/sett->wight);
    double ntarg = norma(scapower(targ,1.0/norma(targ)));
    lima = sett->limit/cos(al);
    while((ntarg*kof*i)<lima)
    {
        for(j = 0; j < arr.size(); j++)
        {
            ptr sca;
            ptr pl;
            sca = scapower(targ,ntarg*kof*i);
            pl = plusp(sett->cam,sca);
            toto = arr[j]->belong(pl,sett->cam);
            if(toto)
               {
                   grad = ((ntarg*kof*i)/(sett->limit/cos(al))-0.01);
                   grad = round(grad*100)/100;
                   break;
               }
        }
        if(toto)break;
        i++;
    }
    if(toto)
    return (arr[j]->numtype+grad);
    return 0;
}

void render(settings *sett, vector<FFigure*> &arr)
{
    ptr gorisont;
    ptr Ngorisont;
    ptr Nup;
    ptr maintarget;
    ptr targ;
    double al = 3.1415*(sett->alpha/360);
    float grad;
    float* adad = new float(0);
    float color[3] = {250, 250, 250};
    cimg_library::CImg<unsigned char> img(sett->wight,sett->height,1,3);
    float colorP;
    targ = minusp(sett->target,sett->cam);
    sett->screen = norma(targ);
    gorisont = vecmultip(sett->up, targ);
    gorisont = scapower(gorisont,1/norma(gorisont));
    gorisont = scapower(gorisont,(sin(al)/cos(al))*(sett->screen));
    sett->up = scapower(sett->up, (1/ norma(sett->up)));
    sett->up = scapower(sett->up,(sett->height/sett->wight)*(sin(al)/cos(al))*(sett->screen));
    #pragma omp parallel for private(Ngorisont, Nup, maintarget,colorP,grad,color)
    for(int i = -(int)(sett->wight/2); i <= (int)(sett->wight/2); i++)
    {
        Ngorisont = scapower(gorisont,(-i)/(sett->wight/2));

        for(int j = (int)(-(sett->height/2)); j <= (int)(sett->height/2); j++)
        {
                Nup = scapower(sett->up,(-j)/(sett->height/2));
                maintarget = plusp(targ,Ngorisont);
                maintarget = plusp(maintarget,Nup);
                colorP = colorfig(maintarget,arr,sett);
                grad = modf(colorP,adad);
                grad = grad*250;
                if((int)colorP != 0){
                color[0] =0;
                color[1] =0;
                color[2] =0;
                color[(int)colorP-1] = 255 - (int)grad;
                }
                img.draw_point(i+(sett->wight/2), j+(sett->height/2), color);
                color[0] =250;
                color[1] =250;
                color[2] = 250;
        }
    }
    img.display("Zdraste");
}
