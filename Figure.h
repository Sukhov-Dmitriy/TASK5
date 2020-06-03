#include"functions.h"
using namespace std;
class FFigure {
public:
    string type;
    float numtype;
    double data[12];
    virtual void sortv()=0;
    virtual bool belong(ptr &point, ptr &cam) = 0;
};



