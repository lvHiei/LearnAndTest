#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <thread>

using namespace std;

int fd;
void func1(){
    for(int i = 0; i < 10; ++i){
        pwrite(fd, &i, 1, i);
    }

    fsync(fd);
    printf("write 10 end\n");

    this_thread::sleep_for(std::chrono::microseconds(200));

    lseek(fd, 20, SEEK_SET);
    
    for(int i = 20; i < 30; ++i){
        pwrite(fd, &i, 1, i);
    } 
    fsync(fd);


    printf("write 30 end\n");
    close(fd);


    int fd2 = open("data.dat", O_RDWR);

    lseek(fd2, 10, SEEK_SET);
    
    for(int i = 10; i < 20; ++i){
        pwrite(fd2, &i, 1, i);
    } 

    printf("write 20 end\n");
    close(fd2);

}

void func2(){
    this_thread::sleep_for(std::chrono::microseconds(100));


    for(int i = 0; i < 10; ++i){
        int j = 0;
        pread(fd, &j, 1, i);
        printf("%d ", j);
    }


    this_thread::sleep_for(std::chrono::microseconds(200));

    for(int i = 10; i < 20; ++i){
        int j = 0;
        pread(fd, &j, 1, i);
        printf("%d ", j);
    }

    for(int i = 20; i < 30; ++i){
        int j = 0;
        pread(fd, &j, 1, i);
        printf("%d ", j);
    }
}

int main()
{
    fd = open("data.dat", O_RDWR|O_CREAT, 0755);
    
    thread t0(func1);
    thread t1(func2);
    t0.join();
    t1.join();

    return 0;
}
