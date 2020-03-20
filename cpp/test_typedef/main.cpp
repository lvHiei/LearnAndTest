#include <memory>
#include <stdio.h>

#define LOGI(...) printf(__VA_ARGS__);printf("\n")

#define BW_INFO_MORE(LOGFMT, ARG1, ...) do{ \
    char fmt[1024] = {0};\
    sprintf(fmt, "%s,%s", "(%s,%d):", LOGFMT); \
    LOGI(fmt, __PRETTY_FUNCTION__, __LINE__, ARG1, ##__VA_ARGS__);    \
}while(0)

#define BW_INFO() LOGI("(%s,%d)", __PRETTY_FUNCTION__, __LINE__);

class Base{
public:
    Base(){BW_INFO();}
   virtual ~Base(){BW_INFO();}

   virtual void func(){BW_INFO();}
};

class Child:public Base{
public:
   Child(){BW_INFO();func();}
   virtual ~Child(){BW_INFO();}

   virtual void func(){BW_INFO();}
};

int main(int argc, char** argv){
    BW_INFO_MORE("argc=%d,%p", argc, argv);
    std::shared_ptr<Base> sp(new Child);
    return 0;
}
