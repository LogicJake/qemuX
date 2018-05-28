#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define CAPACITY 1

#define N 20

int buffer[N][CAPACITY];
int in[N];
int out[N];


int get_item(int i)
{
    int item;
    item = buffer[i][0];
    return item;
}

void put_item(int i,int item)
{
    buffer[i][0] = item;
}

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

void sema_init(sema_t *sema, int value)
{
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    sema->value--;
    while (sema->value < 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    ++sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex_sema[N];
sema_t empty_buffer_sema[N];
sema_t full_buffer_sema[N];

void *worker(void *arg)
{
	int *index = (int *)arg;
    int i;
    int item;
    int read = ((*index)+N-1)%N;
    int write = ((*index)+N)%N;

    sema_wait(&full_buffer_sema[read]);
    sema_wait(&mutex_sema[read]);
    item = get_item(read); 
    printf("Thread%d read from Thread%d: %d\n",*index+1,read+1,item);
    sema_signal(&mutex_sema[read]);
    sema_signal(&empty_buffer_sema[read]);
    
	if(*index != 0){
		sema_wait(&empty_buffer_sema[write]);
    	sema_wait(&mutex_sema[write]);
    
		item++;
    	put_item(write,item); 

    	sema_signal(&mutex_sema[write]);
    	sema_signal(&full_buffer_sema[write]);
    	
		printf("\tThread%d write to Thread%d: %d\n",*index+1, (write+2)%N,item);
	}
}

int main()
{ 
    pthread_t consumer_tid[N];
	int i;
	int param[N];
	
	put_item(0,1); 
	sema_init(&mutex_sema[0],1);				//T1³õÊ¼»¯Îª1 
    sema_init(&empty_buffer_sema[0],0);
    sema_init(&full_buffer_sema[0],1);
	
	for(i=1;i<N;i++)
	{
		sema_init(&mutex_sema[i],1);
    	sema_init(&empty_buffer_sema[i],1);
    	sema_init(&full_buffer_sema[i],0);
	}
	
	for(i=0;i<N;i++)
	{
		int *j;
		param[i]=i;
		j=&param[i];
    	pthread_create(&consumer_tid[i], NULL, worker, j);
	}

	for(i=0;i<N;i++)
		pthread_join(consumer_tid[i], NULL);

    return 0;
}
