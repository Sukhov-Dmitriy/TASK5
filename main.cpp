#define EPS 0.000000000001
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <omp.h>
#include "CImg.h"
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
        double qa;
        for(int i =0; i<3; i++){
            input>>qa;
            cam.push_back(qa);
        }
        input>>c;
        for(int i =0; i<3; i++){
            input>>qa;
            target.push_back(qa);
        }
        input>>c;
        for(int i =0; i<3; i++){
            input>>qa;
            up.push_back(qa);
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
    vector<double> cam;
    vector<double> target;
    vector<double> up;
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
    virtual bool belong(vector<double> &point) = 0;

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
    bool belong(vector<double> &point) override
    {
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
class box : public FFigure{
public:
    box(const box &other){
        this->numdat = other.numdat;
    }
    box(){
    numdat = 1777;
    }
    void sortv() override
    {numdat = 1777;}
    int numdat;
    bool belong(vector<double> &point) override
    {
        if((data[0]+point[0] < data[3]) && (data[1]+point[1] < data[4]) && (data[2]+point[2] < data[5])){
            return true;
        }
        else
        {
            return false;
        }
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
    vector<double> v1;
    vector<double> v2;
    vector<double> v3;
    vector<double> v4;
    void sortv() override
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
    bool belong(vector<double>& point) override
    {
        double q1;
        double q2;
        double q3;
        double q4;
        q1 = scamultip(vecmultip(minusp(v3,v1),minusp(v2,v1)),minusp(point,v1));
        q2 = scamultip(vecmultip(minusp(v4,v1),minusp(v3,v1)),minusp(point,v1));
        q3 = scamultip(vecmultip(minusp(v2,v1),minusp(v4,v1)),minusp(point,v1));
        q4 = scamultip(vecmultip(minusp(v2,v4),minusp(v3,v4)),minusp(point,v4));
        if(q1 > 0 && q2 > 0 && q3 > 0 && q4 > 0)
        {
            cout<<"Belonges\n";
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

float colorfig(vector<double> &targ, vector<FFigure*> &arr, settings *sett)
{
    int i = 0;
    float grad;
    bool toto;
    double kof = 4*(((sin((sett->alpha)/2)/cos((sett->alpha)/2))*(sett->screen))/sett->wight);
    double ntarg = norma(scapower(targ,1.0/norma(targ)));
    while((ntarg*kof*i)<(sett->limit/cos(sett->alpha/2)))
    {
        for(int j  = 0; j < arr.size(); j++)
        {   cout<<"ddddd\n";
            vector<double> sca;
            vector<double> pl;
            sca = scapower(targ,ntarg*kof*i);
            pl = plusp(sett->cam,sca);
            toto = arr[j]->belong(pl);
            if(toto)
               {   cout<< "DAAAAAAAA\n";
                   grad = ((ntarg*kof*i)/(sett->limit/cos(sett->alpha/2))-0.01);
                   grad = round(grad*100)/100;
                   return (arr[j]->numtype+grad);
               }
        }
        i++;
    }
    return 0;
}
void render(settings *sett, vector<FFigure*> &arr)
{
    vector<double> gorisont;
    vector<double> Ngorisont;
    vector<double> Nup;
    vector<double> maintarget;
    vector<double> targ;
    float grad;
    float* adad = new float(0);
    float color[3] = {250, 250, 0};
    cimg_library::CImg<unsigned char> img(sett->wight,sett->height,1,3);
    cout<<"ddddddd\n";
    float colorP;
    targ = minusp(sett->target,sett->cam);

    gorisont = vecmultip(sett->up, targ);
    gorisont = scapower(gorisont,1/norma(gorisont));
    gorisont = scapower(gorisont,(sin((sett->alpha)/2)/cos((sett->alpha)/2))*(sett->screen));
    sett->up = scapower(sett->up, (1/ norma(sett->up)));
    sett->up = scapower(sett->up,(sett->height/sett->wight)*(sin((sett->alpha)/2)/cos((sett->alpha)/2))*(sett->screen));
    for(int i = (sett->wight/2); i < -(sett->wight/2); i--)
    {   cout<<"ddddddd\n";
        Ngorisont = scapower(gorisont,i/(sett->wight/2));
        for(int j = (sett->height/2); j < -(sett->height/2); j--)
        {
                Nup = scapower(sett->up,j/(sett->height/2));
                maintarget = plusp(targ,Ngorisont);
                maintarget = plusp(maintarget,Nup);
                colorP = colorfig(maintarget,arr,sett);
                grad = modf(colorP,adad);
                grad = grad*100;
                if((int)colorP != 0){
                color[0] =0;
                color[1] =0;
                color[2] = 0;
                color[(int)colorP-1] = 255 - (int)grad;
                }
                img.draw_point(i, j, color);
        }
    }
    img.display("1222");
}
void autotest1 (){
    tetra prt;
    cout<<"autotest1 ...\n";
    vector<double> point;
    point.push_back(0.2);
    point.push_back(0.0);
    point.push_back(0.3);
    prt.data;
    prt.data[0] = 1.0;
    prt.data[0] = 0.0;
    prt.data[0] = 0.0;
    prt.data[0] = 0.0;
    prt.data[0] = 1.0;
    prt.data[0] = 0.0;
    prt.data[0] = 0.0;
    prt.data[0] = -1.0;
    prt.data[0] = 0.0;
    prt.data[0] = 0.0;
    prt.data[0] = 0.0;
    prt.data[0] = 1.0;
    prt.numtype = 3;
    prt.sortv();
    if(prt.belong(point))
    {
        cout<<"autotest1 passed succesfuly\n";
    }

}
int main (){
    char c;
    int n = 1;


    string filedat("figures.txt");//передачи названия файла
    string fileset("settings.txt");
    ifstream FileD(filedat.c_str());//c_str //Открытие файла
    ifstream FileS(fileset.c_str());
    cout<<"zopa\n";
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
    autotest1();

        for(int i =0; i<3; i++){
            cout<<sett.cam[i]<<endl;
        }
        for(int i =0; i<6; i++){
            cout<<"aeeee"<<arr[0]->data[i]<<endl;
        }
        for(int i =0; i<3; i++){
            cout<<sett.target[i]<<endl;
        }
        for(int i =0; i<3; i++){
            cout<<sett.up[i]<<endl;
        }
        cout<<sett.screen<<endl;
        cout<<sett.limit<<endl;
        cout<<sett.alpha<<endl;
        cout<<sett.wight<<endl;
        cout<<sett.height<<endl;



   render(&sett,arr);

    //закрытие/очищение
    for(int i = 0; i < n; i++){
        delete arr[i];
    }


    return 0;
}
