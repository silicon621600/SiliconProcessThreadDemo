# 说明
测试thread条件变量的demo
经典的生产者消费者问题
## producer_and_consumer.c
两个生产者两个消费者
```
 gcc producer_and_consumer.c -lpthread
 ./a.out > res
 cat res | grep producer1
 cat res | grep producer2
 cat res | grep consumer1
 cat res | grep consumer2
```
结果按顺序说明正确,producer1生产正数,producer2生产负数
但是 consumer要么全是正数要么全是负数...
##p_and_c1.c 是producer_and_consumer.c改写测试区
pthread_cond_signal 换成pthread_cond_broadcast 结果和上一样
增加if (id==2) usleep(20);消费者中得到的数有正有负
## p_and_c2.c 在producer_and_consumer.c上去掉条件变量,只用互斥
存在操作失败的可能,但实际运行并没有,结果和producer_and_consumer.c一样
改成一个生产者, 结果生产者会出现操作失败的情况

## p_and_c3.c 把条件变量notempty和 notfull 改为isempty和isfull
程序阻塞,必须把put,get改成一次放满和一次取空才行
