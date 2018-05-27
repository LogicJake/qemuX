#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NR_CPU 10
#define NR_TOTAL 1000000			//计算 NUM_ITEM项 
#define NR_CHILD (NR_TOTAL/NR_CPU)

struct param {
    long long start;			//开始分母 
    long long end;			//结束分母 
};

struct result {
    double sum;
};

void *compute(void *arg)
{
    struct param *param;
    struct result *result;
    double sum = 0;
    int i,state;
	
    param = (struct param *)arg;
    
    if(((param->start+1)/2)%2==0)
    	state = -1;
    else
    	state = 1;
    	
    for (i = param->start; i < param->end; i+=2){
        sum += 1.0/i*state;
        state *= -1;
    }

    result = (struct result *)malloc(sizeof(struct result));
    result->sum = sum;
    return result;
}

int main()
{
    pthread_t workers[NR_CPU];
    struct param params[NR_CPU];
    int i;

    for (i = 0; i < NR_CPU; i++) {
        struct param *param;
        param = &params[i];
        param->start = (i * NR_CHILD + 1) * 2 - 1;
        param->end = ((i + 1) * NR_CHILD + 1) * 2 - 1;
        printf("%lld:%lld\n",param->start,param->end);
        pthread_create(&workers[i], NULL, compute, param);
    }

    double sum = 0;
    for (i = 0; i < NR_CPU; i++) {
        struct result *result;
        pthread_join(workers[i], (void **)&result);
        sum += result->sum;
        free(result);
    }
    printf("sum = %.16lf\n", sum*4);
    return 0;
}
