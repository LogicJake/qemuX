#include <stdio.h>
#include <pthread.h>

#define CAPACITY 4
#define ITEM_COUNT (CAPACITY * 2)

char buffer1[CAPACITY];
char buffer2[CAPACITY];

int num1 = 0;		//buffer1的个数 
int num2 = 0;		//buffer2的个数

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

sema_t mutex1; 
sema_t mutex2; 

sema_t empty_buffer1_sema;		//buffer1空位置 
sema_t full_buffer1_sema;		//buffer1满位置 

sema_t empty_buffer2_sema;		//buffer2空位置
sema_t full_buffer2_sema;		//buffer2满位置

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

void push1(char c){
	buffer1[num1] = c;
	num1++;
}

void push2(char c){
	buffer2[num2] = c;
	num2++;
}

char pop1(){
	char c = buffer1[0];
	num1--;
	int i;
	for(i=0;i<num1;i++)			//前移 
		buffer1[i] = buffer1[i+1];
	return c;
}

char pop2(){
	char c = buffer2[0];
	num2--;
	int i;
	for(i=0;i<num2;i++)			//前移 
		buffer2[i] = buffer2[i+1];
	return c;
}
 
void *consume(void *arg){
	int i;
	for(i=0;i<ITEM_COUNT;i++)
	{
		sema_wait(&full_buffer2_sema);
		sema_wait(&mutex2);
		
		char c = pop2();
		
		sema_signal(&mutex2);
		sema_signal(&empty_buffer2_sema);
		
		printf("\t\tconsume:%c\n",c);
	}
}

void *compute(void *arg){
	int i;
	for(i=0;i<ITEM_COUNT;i++)
	{
		sema_wait(&full_buffer1_sema);
		sema_wait(&mutex1);
		char c = pop1();
		c = c + 'A' - 'a';
		
		sema_wait(&empty_buffer2_sema);
		sema_wait(&mutex2);

		push2(c);
	
		sema_signal(&mutex1);
		sema_signal(&empty_buffer1_sema);	
		
		sema_signal(&mutex2);		
		sema_signal(&full_buffer2_sema);
		
		printf("\tcompute:%c\n",c);
	}
}

void *produce(){
	int i;
	for(i=0;i<ITEM_COUNT;i++)
	{	
		sema_wait(&empty_buffer1_sema);
		sema_wait(&mutex1);

		char c = i + 'a';
		push1(c);

		sema_signal(&mutex1);
		sema_signal(&full_buffer1_sema);
		
		printf("produce:%c\n",c);
	}
}
 
int main(){
	pthread_t consumer,computer;
	
	sema_init(&mutex1,1); 		//互斥，只能被一个线程访问 
	sema_init(&mutex2,1); 
	sema_init(&empty_buffer1_sema,CAPACITY); 
	sema_init(&full_buffer1_sema,0); 
	sema_init(&empty_buffer2_sema,CAPACITY); 
	sema_init(&full_buffer2_sema,0); 

	pthread_create(&consumer,NULL,consume,NULL);
	pthread_create(&computer,NULL,compute,NULL);
	
	produce();
	
	pthread_join(consumer,NULL);
	pthread_join(computer,NULL);
} 
