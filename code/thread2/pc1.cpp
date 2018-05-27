#include <stdio.h>
#include <pthread.h>

char buffer1[4];
char buffer2[4];
int num1 = 0;		//buffer1的个数 
int num2 = 0;		//buffer2的个数

pthread_mutex_t mutex1; 
pthread_mutex_t mutex2; 
pthread_cond_t wait_buffer1_push;		//buffer1有数据进入信号量 
pthread_cond_t wait_buffer1_pop;		//buffer1有数据出信号量 

pthread_cond_t wait_buffer2_push;		//buffer2有数据进入信号量 
pthread_cond_t wait_buffer2_pop;		//buffer2有数据出信号量 

bool buffer1_empty(){
	return num1 == 0;
}

bool buffer1_full(){
	return num1 == 4;
}

bool buffer2_empty(){
	return num2 == 0;
}

bool buffer2_full(){
	return num2 == 4;
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
	for(i=0;i<8;i++)
	{
		pthread_mutex_lock(&mutex2);
		while(buffer2_empty())
			pthread_cond_wait(&wait_buffer2_push,&mutex2);
		char c = pop2();
		printf("\t\tconsume:%c\n",c);
		pthread_cond_signal(&wait_buffer2_pop);				//发出信号量：buffer2有位置空出来了
		pthread_mutex_unlock(&mutex2);
	}
}

void *compute(void *arg){
	int i;
	for(i=0;i<8;i++)
	{
		pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex2);

		while(buffer1_empty())
			pthread_cond_wait(&wait_buffer1_push,&mutex1);
		while(buffer2_full())
			pthread_cond_wait(&wait_buffer2_pop,&mutex2);

		char c = pop1();
		c = c + 'A' - 'a';
		push2(c);
		printf("\tcompute:%c\n",c);
		pthread_cond_signal(&wait_buffer1_pop);				//发出信号量：buffer1有位置空出来了
		pthread_cond_signal(&wait_buffer2_push);				//发出信号量：buffer2有新成员可以取 
		pthread_mutex_unlock(&mutex1);
		pthread_mutex_unlock(&mutex2);
	}
}

void *produce(){
	int i;
	for(i=0;i<8;i++)
	{		
		pthread_mutex_lock(&mutex1);	
		while(buffer1_full())
			pthread_cond_wait(&wait_buffer1_pop,&mutex1);
		char c = i + 'a';
		push1(c);
		
		printf("produce:%c\n",c);
		pthread_cond_signal(&wait_buffer1_push);				//发出信号量：buffer1有新成员可以取 
		pthread_mutex_unlock(&mutex1);
	}
}
 
int main(){
	pthread_t consumer,computer;
	pthread_mutex_init(&mutex1,NULL);
	pthread_mutex_init(&mutex2,NULL);
	pthread_cond_init(&wait_buffer1_pop,NULL);
	pthread_cond_init(&wait_buffer2_pop,NULL);
	pthread_cond_init(&wait_buffer1_push,NULL);
	pthread_cond_init(&wait_buffer2_push,NULL);
	pthread_create(&consumer,NULL,consume,NULL);
	pthread_create(&computer,NULL,compute,NULL);
	produce();
	pthread_join(consumer,NULL);
	pthread_join(computer,NULL);
} 
