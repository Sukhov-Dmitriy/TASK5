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


    vector<FFigure*> arr;
    for(int i = 0; i < n; i++){// получение всех данных из файла
        arr.push_back(CreateData(FileD, f));
    }
    FileD.close();
    settings sett(FileS);
    FileS.close();

    render(&sett,arr);//  RENDER

    //закрытие/очищение
    for(int i = 0; i < n; i++){
        delete arr[i];
    }


    return 0;
}
