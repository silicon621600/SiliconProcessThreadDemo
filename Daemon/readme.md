# 说明
来自网上的两个示例程序
http://blog.csdn.net/liangxanhai/article/details/7752898
```
 gcc test.c
 ./a.out
 tail -f /tmp/dameon.txt
```

http://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux
```
 gcc daemonize.c -o firstdaemon
 ./firstdaemon
 ps -xj | grep firstdaemon
 grep firstdaemon /var/log/syslog
```
------+------+------+------+-----+-------+------+------+------+-----+
| PPID | PID  | PGID | SID  | TTY | TPGID | STAT | UID  | TIME | CMD |
+------+------+------+------+-----+-------+------+------+------+-----+
|    1 | 3387 | 3386 | 3386 | ?   |    -1 | S    | 1000 | 0:00 | ./  |
+------+------+------+------+-----+-------+------+------+------+-----+
The daemon has no controlling terminal (TTY = ?)
The parent process ID (PPID) is 1 (The init process)
The PID != SID which means that our process is NOT the session leader
(because of the second fork())
Because PID != SID our process can't take control of a TTY again


 In reality you would also want to implement a signal handler and set up the logging properly (Files, log levels...).???
