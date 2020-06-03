class ptr
{   public:
     ptr(){
    }
    double data[3];
    ptr(const ptr &other){
        for(int i = 0; i < 3; i++){
        this->data[i] = other.data[i];
        }
    }
    double & operator[] (int index){
    return data[index];
    }
    void operator= (const ptr& other)
    {
        for(int i = 0; i < 3; i++){
        this->data[i] = other.data[i];
        }
    }
};
