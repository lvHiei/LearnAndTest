
#include <iostream>
using namespace std;
class MyClass {
public:
    MyClass() {
        a = 1; b = 2;
    }
    ~MyClass() {
        a = 3; b = 4;
    }
public:
    int a;
    int b;
};


int main(){

MyClass *pcls = (MyClass *)malloc(sizeof(MyClass));
// ??? : 以下输出 1, 2 : new (plcs) MyClass;
cout << pcls->a << ", " << pcls->b << endl;
// ???: 以下输出 3, 4 : pcls->~MyClass();
cout << pcls->a << ", " << pcls->b << endl;
free(pcls);
return 0;
}
