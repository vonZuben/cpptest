#include<iostream>
#include<chrono>
#include<string>
#include<cstring>
#include<algorithm>

#define MEMSIZE 100000

using namespace std;

template <typename T>
void timeit(string testname, T f){

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    f();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << testname << ": Time "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << "us.\n";
}

int main(int argc, char **argv){

    cout << "MEMSIZE (num int): " << MEMSIZE << "\n";

    int hello;

    timeit("Heap alloc", [&](){

            int *p = new int[MEMSIZE];

            hello = 99;

            delete[]p;

    });

    timeit("memcopy", [](){

            int p[MEMSIZE] = {0};
            int d[MEMSIZE];

            memcpy(d, p, MEMSIZE*sizeof(int));

    });

    timeit("std::copy", [](){

            int p[MEMSIZE] = {0};
            int d[MEMSIZE];

            copy(p, &p[MEMSIZE], d);

    });


    cout << hello << '\n';
    return 0;
}
