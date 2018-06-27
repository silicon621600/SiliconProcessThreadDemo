#include <stdio.h>
#include <pthread.h>
#define BUFFER_SIZE 4
#define OVER (-1)
struct product_storage
{
    int buffer[BUFFER_SIZE];
    pthread_mutex_t lock;
    int readpos,writepos;
    pthread_cond_t isempty;
    pthread_cond_t isfull;
} store;

void init(struct product_storage *p )
{
    pthread_mutex_init(&p->lock,NULL);
    pthread_cond_init(&p->isempty,NULL);
    pthread_cond_init(&p->isfull,NULL);
    p->readpos = 0;
    p->writepos = 0;
}
//生产者调用
void put(struct product_storage *p,int data)
{
    pthread_mutex_lock(&p->lock);//先上锁
    //判断能否操作
    while (p->readpos!=p->writepos)
    {
        pthread_cond_wait(&p->isempty,&p->lock);
    }
    p->buffer[p->writepos++] = data;
    if (p->writepos==BUFFER_SIZE) p->writepos =0;
    if ((p->writepos+1)%BUFFER_SIZE==p->readpos)pthread_cond_signal(&p->isfull);

    pthread_mutex_unlock(&p->lock);
}
//消费者调用
int  get(struct product_storage *p)
{
    int ret;
    pthread_mutex_lock(&p->lock);
    while ( (p->writepos+1)%BUFFER_SIZE!=p->readpos)
    {
        pthread_cond_wait(&p->isfull,&p->lock);
    }
    ret = p->buffer[p->readpos++];
    if (p->readpos==BUFFER_SIZE) p->readpos=0;
    if (p->readpos==p->writepos) pthread_cond_signal(&p->isempty);
    pthread_mutex_unlock(&p->lock);
    return ret;
}

//生产者线程
void * producer(void *data)
{
    int id = *((int *)data);
    int t;
    for (int i=0;i<20;i++)
    {
        if (id==2) t=-i;
        else t=i;
        put(&store,t);
        printf("producer%d -> %d\n",id,t);
    }
    return NULL;
}
//消费者线程
void *consumer(void *data)
{
    int id = *((int*)data);
    int t;
    for (int i=0;i<20;i++)
    {
        t = get(&store);
        printf("consumer%d <- %d\n",id,t);
    }
    return NULL;
}
int main(void)
{
    pthread_t thp1,thp2,thc1,thc2;
    void * retval;
    int p1,p2,c1,c2;
    p1=c1=1;
    p2=c2=2;
    init(&store);
    pthread_create(&thp1,NULL,producer,&p1);
    pthread_create(&thp2,NULL,producer,&p2);
    pthread_create(&thc1,NULL,consumer,&c1);
    pthread_create(&thc2,NULL,consumer,&c2);
    pthread_join(thp1,&retval);
    pthread_join(thp2,&retval);
    pthread_join(thc1,&retval);
    pthread_join(thc2,&retval);
    return 0;
}
