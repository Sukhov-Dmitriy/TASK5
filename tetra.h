
#include "sphere.h"
using namespace std;
class FFigure;
class tetra : public FFigure{
public:
    tetra(const tetra &other);
    tetra();

    int numdat;
    ptr v1;
    ptr v2;
    ptr v3;
    ptr v4;
    void sortv();
    bool belong(ptr& point,ptr &cam);


};
