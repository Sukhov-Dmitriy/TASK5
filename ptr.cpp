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
ptr::ptr(){
    }
ptr::ptr(const ptr &other){
        for(int i = 0; i < 3; i++){
        this->data[i] = other.data[i];
        }
    }
    double & ptr::operator[] (int index){
    return data[index];
    }
    void ptr::operator= (const ptr& other)
    {
        for(int i = 0; i < 3; i++){
        this->data[i] = other.data[i];
        }
    }
