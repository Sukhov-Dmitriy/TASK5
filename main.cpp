#define EPS 0.000000000001
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <omp.h>
//#include "CImg.h"
using namespace std;
vector<double> scapower(vector<double> &A, double a)
{
    vector<double> res;
    for(int i = 0; i < 3; i++)
    {
        res.push_back(A[i]*a);
    }
    return res;
}

vector<double> minusp(vector<double> &A, vector<double> &B)
{
    vector<double> res;
    for(int i = 0; i < 3; i++)
    {
        res.push_back(A[i]-B[i]);
    }
    return res;
}
vector<double> plusp(vector<double> &A, vector<double> &B)
{
    vector<double> res;
    for(int i = 0; i < 3; i++)
    {
        res.push_back(A[i]+B[i]);
    }
    return res;
}
vector<double> vecmultip(const vector<double> &A,const vector<double> &B)
{
    vector<double> res;
    res.push_back(A[1]*B[2]-B[1]*A[2]);
    res.push_back(A[2]*B[0]-B[2]*A[0]);
    res.push_back(A[0]*B[1]-B[0]*A[1]);
    return res;
}
double scamultip(const vector<double> &A,const vector<double> &B)
{
    return A[0]*B[0]+A[1]*B[1]+A[2]*B[2];
}
double norma(const vector<double> &A)
{
    return sqrt(A[0]*A[0] + A[1]*A[1] + A[2]*A[2]);
}

class settings{
public:
    settings(ifstream &input){
        string c;
        input>>c;
        for(int i =0; i<3; i++){
            input>>cam[i];
        }
        input>>c;
        for(int i =0; i<3; i++){
            input>>target[i];
        }
        input>>c;
        for(int i =0; i<3; i++){
            input>>up[i];
        }
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
    double cam[3];
    double target[3];
    double up[3];
    double screen;
    double limit;
    double alpha;
    double wight;
    double height;
};
class Figure {
public:
    string type;
    double data[12];
    virtual void sortv();
};

class sphere : public Figure{
public:
    int numdat = 4;
    int numtype = 1;
    bool belong(vector<double> &point){
    if(pow((point[0]-data[0]),2)+pow((point[1]-data[1]),2)+pow((point[2]-data[2]),2)-pow(data[4],2)<EPS)
    {
        return true;
    }
    else
    {
        return false;
    }
    }
};
class box : public Figure{
public:
    int numdat = 6;
    int numtype = 2;
    bool belong(vector<double> &point){
        if((data[0]+point[0] < data[3]) && (data[1]+point[1] < data[4]) && (data[2]+point[2] < data[5])){
            return true;
        }
        else
        {
            return false;
        }
    }

};
class tetra : public Figure{
public:
    int numdat = 12;
    int numtype = 3;
    vector<double> v1;
    vector<double> v2;
    vector<double> v3;
    vector<double> v4;
    void sortv()
    {
        for(int i = 0;i < 3; i++)
        {
            v1.push_back(data[i]);
        }
         for(int i = 0;i < 3; i++)
        {
            v2.push_back(data[i+3]);
        }
         for(int i = 0;i < 3; i++)
        {
            v3.push_back(data[i+6]);
        }
        for(int i = 0;i < 3; i++)
        {
            v4.push_back(data[i+9]);
        }
        if(scamultip(minusp(v4,v1),vecmultip(minusp(v3,v1),minusp(v2,v1)))<0)
        {
            vector<double> t;
            t = v3;
            v3 = v1;
            v1 = t;
        }
    }
    bool belong(vector<double>& point){
        double q1;
        double q2;
        double q3;
        double q4;
        q1 = scamultip(vecmultip(minusp(v3,v1),minusp(v2,v1)),minusp(point,v1));
        q2 = scamultip(vecmultip(minusp(v4,v1),minusp(v3,v1)),minusp(point,v1));
        q3 = scamultip(vecmultip(minusp(v2,v1),minusp(v4,v1)),minusp(point,v1));
        q4 = scamultip(vecmultip(minusp(v2,v4),minusp(v3,v4)),minusp(point,v4));
        if(q1 > 0 && q2 > 0 && q3 > 0 && q4 > 0)
            return true;
        else
            return false;
    }

};
class FigureFactory {
public:
    virtual Figure* create_dat()=0;
    virtual ~FigureFactory(){}
};
class FigureFactoryS : public FigureFactory{
public:
    Figure* create_dat()
    {
        return new sphere();
    }
};
class FigureFactoryB : public FigureFactory{
public:
    Figure* create_dat()
    {
        return new box();
    }
};
class FigureFactoryT : public FigureFactory{
public:
    Figure* create_dat()
    {
        return new tetra();
    }
};
Figure *CreateData(ifstream &input, map<string,FigureFactory*>  f){
    string I;
    Figure *fig = NULL;
    input >> I;
    fig = f[I]->create_dat();
    if(I == "tetra"){
        for(int j = 0; j < 12; j++){
            input >> fig->data[j];
        }
        fig->sortv();
    }
    if(I == "box"){
        for(int j = 0; j < 6; j++){
            input >>fig->data[j];
        }
    }
    if(I == "sphere"){
        for(int j = 0; j < 4; j++){
            input >>fig->data[j];
        }
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

int colorfig(vector<double> &targ, vector<*Figure> arr, settings *sett)
{
    int i = 0;
    double kof = (((sin((sett->alpha)/2)/cos((sett->alpha)/2))*sett->screen)/sett->wight)/2;
    while(norma(scapower(targ,kof*i))<(sett->limit/cos(sett->alpha/2)))
    {
        for(int j  = 0; j < arr.size(); j++)
        {
            if(arr[j].belong(plusp(sett->cam,scapower(targ,kof*i))))
               {
                   return arr[j].numtype;
               }
        }
        i++;
    }
    return 0;
}
int main (){
    char c;
    int n = 1;

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
    vector<Figure*> arr;// вектор для фигур
    for(int i = 0; i < n; i++){// получение всех данных из файла
        arr.push_back(CreateData(FileD, f));
    }
    FileD.close();

    settings sett(FileS);
    FileS.close();
    cimg_library::CImg<unsigned char> img(sett.wight,sett.height,1,3);


    //закрытие/очищение
    for(int i = 0; i < n; i++){
        delete arr[i];
    }


    return 0;
}
