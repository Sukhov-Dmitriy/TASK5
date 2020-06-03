using namespace std;
class ptr
{   public:
    ptr();

    double data[3];
    ptr(const ptr &other);
    double & operator[] (int index);
    void operator= (const ptr& other);

};
