#include"tetra.h"
using namespace std;
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
