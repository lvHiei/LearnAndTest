#include <atomic>
#include <iostream>
#include <thread>
#include <string.h>

#include <cstdio>

class Base{
public:
    Base(){printf("Base()\n");}
//    ~Base(){printf("~Base()\n");}
    virtual ~Base(){printf("~Base()\n");}

};


class Sub1: public Base{
public:
    Sub1():Base(){printf("Sub1()\n");}
    ~Sub1(){printf("~Sub1()\n");}
//    virtual ~Sub1(){printf("~Sub1()\n");}

};

class Sub2:public Sub1{
public:
    Sub2(){printf("Sub2()\n");}
    ~Sub2(){printf("~Sub2()\n");}
//    virtual ~Sub2(){printf("~Sub2()\n");}

};

int main(){
    
    printf("delete Base \n");
    Base *p0 = new Base();
    delete p0;
    printf("\n");

    printf("delete Base Sub1 \n");
    Base *p1 = new Sub1();
    delete p1;
    printf("\n");
    
    printf("delete Sub1 \n");
    Sub1 *p2 = new Sub1();
    delete p2;
    printf("\n");

    printf("delete Base Sub2 \n");
    Base *p3 = new Sub2();
    delete p3;
    printf("\n");

    printf("delete Sub2 \n");
    Sub2 *p4 = new Sub2();
    delete p4;
    printf("\n");
    return 0;
}
