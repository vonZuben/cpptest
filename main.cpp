#include<iostream>
#include<chrono>
#include<string>
#include<cstring>
#include<algorithm>
#include<cstdlib>

#define MEMSIZE 16
#define NUMTESTS 5

template <typename T, typename... args>
void timeit(std::string testname, int numt, T f, args&&... a){

    for (int i = 0; i < numt; i++){
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        f(std::forward<args>(a)...);

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << testname << " " << i + 1 << " " << ": Time "
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
                  << "ns.\n";
    }
}

// this just grabs memory from the stack and heap to perform the system call and then returns it to the program
// this way the tests are not affected by system call overhead
void grabMem(){ 

    double tmp[MEMSIZE];
    double *t = new double[MEMSIZE];

    delete[]t;
}

int main(int argc, char **argv){

    std::cout << "MEMSIZE (num double): " << MEMSIZE << '\n';
    std::cout << "size of double: " << sizeof(double) << '\n';

    grabMem();

    double hello; // this is not for testing performance, but testing template stuff

    timeit("Heap alloc", NUMTESTS, [](double *hello){

            double *p = new double[MEMSIZE];

            *hello = 99;

            delete[]p;

    }, &hello);

    double buf[MEMSIZE];

    srand(3);

    for (auto &x : buf){
        x = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
    }

    timeit("memmove", NUMTESTS, [&](){

            double d[MEMSIZE];

            memmove(d, buf, MEMSIZE*sizeof(double));

    });

    timeit("memcopy", NUMTESTS, [&](){

            double d[MEMSIZE];

            memcpy(d, buf, MEMSIZE*sizeof(double));

    });

    timeit("std::copy", NUMTESTS, [&](){

            double d[MEMSIZE];

            std::copy(buf, &buf[MEMSIZE], d);

    });

    std::cout << hello << '\n';
    return 0;
}
