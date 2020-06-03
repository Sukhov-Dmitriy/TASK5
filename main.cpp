#define EPS 0.000000000001
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <omp.h>
#include "CImg.h"
#include<windows.h>

using namespace std;

#include "ptr.h"

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
class settings{
public:
    settings(ifstream &input){
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
        input>>c;
        input>>limit;
        input>>c;
        input>>alpha;
        input>>c;
        input>>wight;
        input>>c;
        input>>height;
    }
    ptr cam;
    ptr target;
    ptr up;
    double screen;
    double limit;
    double alpha;
    double wight;
    double height;
};

class FFigure {
public:
    string type;
    float numtype;
    double data[12];
    virtual void sortv()=0;
    virtual bool belong(ptr &point, ptr &cam) = 0;
};

class sphere : public FFigure{
public:
    sphere(const sphere &other){
        this->numdat = other.numdat;
    }
    sphere(){
    numdat = 1777;
    }
    void sortv() override
    {numdat = 1777;}
    int numdat;
    bool belong(ptr &point, ptr &cam) override
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
};
class box : public FFigure{
public:
    ptr v1;
    ptr v2;
    ptr v3;
    ptr v4;
    ptr v5;
    ptr v6;
    ptr v7;
    ptr v8;
    box(const box &other){
        this->numdat = other.numdat;
    }
    box(){
    numdat = 1777;
    }
    void sortv() override
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
    int numdat;
    bool belong(ptr &point, ptr & cam) override
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

};
class tetra : public FFigure{
public:
    tetra(const tetra &other){
        this->numdat = other.numdat;
    }
    tetra()
    {
        numdat = 12;
    }
    int numdat;
    ptr v1;
    ptr v2;
    ptr v3;
    ptr v4;
    void sortv() override
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
    bool belong(ptr& point,ptr &cam) override
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

};
class FigureFactory {
public:
    virtual FFigure* create_dat()=0;
    virtual ~FigureFactory(){}
};
class FigureFactoryS : public FigureFactory{
public:
    FFigure* create_dat()
    {
        return new sphere();
    }
};
class FigureFactoryB : public FigureFactory{
public:
    FFigure* create_dat()
    {
        return new box();
    }
};
class FigureFactoryT : public FigureFactory{
public:
    FFigure* create_dat()
    {
        return new tetra();
    }
};
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
    gorisont = vecmultip(sett->up, targ);
    gorisont = scapower(gorisont,1/norma(gorisont));
    gorisont = scapower(gorisont,(sin(al)/cos(al))*(sett->screen));
    sett->up = scapower(sett->up, (1/ norma(sett->up)));
    sett->up = scapower(sett->up,(sett->height/sett->wight)*(sin(al)/cos(al))*(sett->screen));
    #pragma omp parallel for
    for(int i = -(sett->wight/2); i < sett->wight/2; i++)
    {
        Ngorisont = scapower(gorisont,(-i)/(sett->wight/2));

        for(int j = -(sett->height/2); j <= (sett->height/2); j++)
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
void autotest1 (){
    tetra fid;
    cout<<"autotest1 ...\n";
    ptr point;
    ptr com;
    com[0] = 2.0;
    com[1] = 0.0;
    com[2] = 0.0;
    point[0] = 0.2;
    point[1] = 0.0;
    point[2] = 0.3;
    fid.data[0] = 1.0;
    fid.data[1] = 0.0;
    fid.data[2] = 0.0;
    fid.data[3] = 0.0;
    fid.data[4] = 1.0;
    fid.data[5] = 0.0;
    fid.data[6] = 0.0;
    fid.data[7] = -1.0;
    fid.data[8] = 0.0;
    fid.data[9] = 0.0;
    fid.data[10] = 0.0;
    fid.data[11] = 1.0;
    fid.numtype = 3;
    fid.sortv();
    if(fid.belong(point,com))
    {
        cout<<"autotest1 passed succesfuly\n";
    }

}
int main (){
    char c;
    int n = 1;
    autotest1();


    string filedat("figures.txt");//передачи названия файла
    string fileset("settings.txt");
    ifstream FileD(filedat.c_str());//c_str //Открытие файла
    ifstream FileS(fileset.c_str());
    if(check_f(FileD)==-1){
        return -1;
    }
    if(check_f(FileS)==-1){
        return -1;
    }
    while (FileD.get(c)){ // определеие количества данных
        if (c=='\n')
            n++;
    }
    cout<<"number of figures equal to "<< n<<endl;
    FileD.clear();
    FileD.seekg(0);// флаг переведен на начало файла

    FigureFactory* fabS = new FigureFactoryS;//  фабрики
    FigureFactory* fabB = new FigureFactoryB;
    FigureFactory* fabT = new FigureFactoryT;
    map<string,FigureFactory*> f = {{"sphere",fabS},{"box",fabB},{"tetra",fabT}};
    vector<FFigure*> arr;// вектор для фигур
    for(int i = 0; i < n; i++){// получение всех данных из файла
        arr.push_back(CreateData(FileD, f));
    }
    FileD.close();

    settings sett(FileS);
    FileS.close();

   render(&sett,arr);

    //закрытие/очищение
    for(int i = 0; i < n; i++){
        delete arr[i];
    }


    return 0;
}
