#include<iostream>
#include<chrono>
#include<string>
#include<cstring>
#include<algorithm>
#include<cstdlib>

// the point of these tests is to detirmin if it is beter to use heap memory with the over head of malloc
// or stack memory with the over head of copying the results. the performance is important because input that is
// procesed every frame in my opengl program creates and manipulates matricies. with stack memory it mallocs
// in no time but results need to be copied. with heap it needs to be allocated and freed constantly but the result
// can use move semantics to only copy pointers.

#define MEMSIZE 16
#define NUMTESTS 10

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

    std::cout << "*****************\n";
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

    double hello; // this is not for testing performance, but testing template and lambda stuff

    timeit("Heap alloc", NUMTESTS, [](double *t){

            double *p = new double[MEMSIZE];

            *t = 99;

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

    timeit("Heap alloc again", NUMTESTS, [](){

            double *d = new double[MEMSIZE];

            delete[]d;
    });

    std::cout << hello << '\n';
    return 0;
}
