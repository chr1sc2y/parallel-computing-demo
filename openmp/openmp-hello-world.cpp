#include <omp.h>
#include <iostream>
 
int main()
{
   #pragma omp parallel
   {
       int tid{ omp_get_thread_num() };
       printf("Hello world from thread %d\n", tid);

       int thread_num{ omp_get_num_threads() };
       if (tid == thread_num - 1)
       {
           printf("tid: %d, thread_num: %d\n", tid, thread_num);
       }
   }
    return 0;
}