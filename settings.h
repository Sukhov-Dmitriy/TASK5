#include"ptr.h"
using namespace std;
class settings{
public:
    settings(ifstream &input);

    ptr cam;
    ptr target;
    ptr up;
    double screen;
    double limit;
    double alpha;
    double wight;
    double height;
};
