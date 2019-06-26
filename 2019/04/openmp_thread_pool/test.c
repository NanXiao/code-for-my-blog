#include <unistd.h>
#include <stdio.h>
#include <omp.h>

int main(void){
        #pragma omp parallel for
        for(int i = 0; i < 256; i++)
        {
            sleep(1);
        }

        printf("Exit loop\n");

        while (1)
        {
            sleep(1);
        }

        return 0;
}
