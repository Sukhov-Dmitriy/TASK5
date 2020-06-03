#include"box.h"
using namespace std;
class FFigure;
class sphere : public FFigure{
public:
    sphere(const sphere &other);
    sphere();
    void sortv();

    int numdat;
    bool belong(ptr &point, ptr &cam);

};
