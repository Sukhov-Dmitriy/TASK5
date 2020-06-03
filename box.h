#include"Figure.h"
using namespace std;
class FFigure;
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
    box(const box &other);
    box();

    void sortv();
    int numdat;
    bool belong(ptr &point, ptr & cam);
};
