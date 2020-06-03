#include"FigureFactory.h"
using namespace std;
FFigure *CreateData(ifstream &input, map<string,FigureFactory*>  f);

int check_f(ifstream &fl);

float colorfig(ptr &targ, vector<FFigure*> &arr, settings *sett);

void render(settings *sett, vector<FFigure*> &arr);
