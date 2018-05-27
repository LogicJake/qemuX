#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define N 20
int *array;

struct param {
    long long start;
};

void swap(int*a,int*b)
{
    int temp;
    temp=*a;
    *a=*b;
    *b=temp;
}

void *select_sort(void *arg)
{
	struct param *param;
	param = (struct param *)arg;
	
    int i,j,min,m;
    
    for(i=param->start;i<N-1;i++)
    {
        min=i;//查找最小值
        for(j=i+1;j<N;j++)
        {
            if(array[min]>array[j])
            {
                min=j;
            }
        }
        if(min!=i)
            swap(&array[min],&array[i]);
    }
}


int main(){
	array = (int *)malloc(sizeof(int)*N);
	srand((unsigned) time(NULL));
	printf("before sort:");
	for(int i = 0; i < N; i++)
	{
		array[i] = rand() % 101;
		printf("%d ",array[i]);
	}
	printf("\n");
	pthread_t worker;
		
	struct param *param = (struct param *)malloc(sizeof(struct param));
	param->start = N/2;
	
	pthread_create(&worker, NULL, select_sort, param);		//排序后半部分 
	
    int i,j,min;
	for(i=0;i<N/2-1;i++)		//排序前半部分 
    {
        min=i;//查找最小值
        for(j=i+1;j<N/2;j++)
        {
            if(array[min]>array[j])
            {
                min=j;
            }
        }
        if(min!=i)
            swap(&array[min],&array[i]);
    }
    
    pthread_join(worker,NULL);
    
    printf("after select sort:");

    for(i = 0; i < N; i++){
		printf("%d ",array[i]);
	}
	printf("\n");

    //归并阶段
	int *tmp = (int *)malloc(sizeof(int)*N);
	
	int first = 0;
	int second = N/2;
	j = 0;
	
	while(first < N/2 && second < N)
	{
		if(array[first] < array[second]){
			tmp[j++] = array[first];
			first++; 
		}
		else{
			tmp[j++] = array[second];
			second++; 
		}
	 } 

	while(first < N/2)
	{
		tmp[j++] = array[first];
		first++; 
	}
	while(second < N)
	{	
		tmp[j++] = array[second];
		second++; 
	}
	free(array);
	array = tmp;
    //输出 
    printf("the result :");
    for(i = 0; i < N; i++){
		printf("%d ",array[i]);
	}
	free(array);
}
