#include "omp.h"
#include "stdio.h"


int main(){
    int counter = 0;
    #pragma omp parallel 
    {
        printf("#########pragma block started. \n");
        int threads = omp_get_num_threads();
        int id = omp_get_thread_num();

        for(int i = id; i < 4; i+=threads){
            

            printf("start %d _________________. \n",counter);
            printf("threads %d. \n",threads);
            printf("ID %d. \n",id);
            printf("i %d. \n",i);
            printf("_________________________ \n \n");
            counter ++;
        }
        printf("#########pragma block ended. \n");
    }
    printf("counter outside loop %d. \n",counter);
return 0;
}
