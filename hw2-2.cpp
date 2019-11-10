#include<cstdio>
class dif
{
private:
    int a;
    int b;
public:
    dif(int _a = 1, int _b = 1):a(_a),b(_b){}
    ~dif(){}
    dif operator* (const dif & _d)
    {
        return dif(a * _d.a, b * _d.b);
    }
    bool operator< (const dif & _d)
    {
        return a * _d.b < b * _d.a;
    }
    void print() const
    {
        printf("%d/%d\n",a,b);
    }
};

const dif half(1,2);
const dif doub(2,1);
const dif form(2,3);

int main()
{
    dif init(1,1);
    for (int i = 0; i < 18; i++)
    {
        init = init * form;
        if(init < half)
        {
            printf("rise\t");
        }
        else
        {
            printf("norise\t");
        }
        
        while (init < half)
        {
            init = init * doub;
        }
        init.print();
    }
    return 0;
}