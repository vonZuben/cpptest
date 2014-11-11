#include<iostream>
#include<chrono>
#include<string>
#include<cstring>
#include<algorithm>
#include<cstdlib>

#define MEMSIZE 4096/4

template <typename T, typename... args>
void timeit(std::string testname, T f, args&&... a){

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    f(std::forward<args>(a)...);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << testname << ": Time "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
              << "ns.\n";
}

int main(int argc, char **argv){

    std::cout << "MEMSIZE (num double): " << MEMSIZE << '\n';
    std::cout << "size of double: " << sizeof(double) << '\n';

    double hello; // this is not for testing performance, but testing template stuff

    timeit("Heap alloc 1", [](double *hello){

            double *p = new double[MEMSIZE];

            *hello = 99;

            delete[]p;

    }, &hello);

    // this test shows that the first malloc for the data section is the most expensive because of the system call,
    // but after freeing it, if we need the memory again than it is alrady held by the internal new/delete memory allocator
    // and is obtainable with out system call. for both heap and stack memory cases

    timeit("Heap alloc 2", [](){ 

            double *p = new double[MEMSIZE];

            delete[]p;

    });


    double buf[MEMSIZE];

    srand(3);

    for (auto &x : buf){ // this isnt working p is all 0's evan after loop
        x = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
    }

    // big problem, all these tests are affected by the first one. everything after goes faster even after changeing the
    // content of buf befor each copy, I think it is because stack memory is malloc'ed for the first one and that memory is still held
    // by the program, allowing copying to be faster.
    
    timeit("memmove", [&](){

            double d[MEMSIZE];

            memmove(d, buf, MEMSIZE*sizeof(double));

    });

    for (auto &x : buf){ // this isnt working p is all 0's evan after loop
        x = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
    }

    timeit("memcopy", [&](){

            double d[MEMSIZE];

            memcpy(d, buf, MEMSIZE*sizeof(double));

    });
    
    for (auto &x : buf){ // this isnt working p is all 0's evan after loop
        x = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
    }

    timeit("std::copy", [&](){

            double d[MEMSIZE];

            std::copy(buf, &buf[MEMSIZE], d);

    });


    std::cout << hello << '\n';
    return 0;
}
