C Library Code
===========

本目录下的代码均是C代码，大多数代码基于Linux平台。

Code List
----------

- main.c   演示了带命令行参数的main函数的使用
- rw_queue 实现了一个基于idx的环形缓冲区队列,常用于“生产者-消费者”应用场景
- ring_buffer 参考Linux内核的kfifo实现了一个基于字节流的环形缓冲区
- synclock 封装了mutex和condition代码，方便在其他项目中快速集成条件同步功能
- keylist  实现了一个带有键值(key)的双向链表，可以通过key直接访问某个节点
- pipe     封装了Linux系统的无名管道（pipe），可以方便地集成到C语言项目中作为“队列”来使用
- syslog   演示如何构建一个“专业”的Log系统，相关博文：[构建专业的log系统](http://ticktick.blog.51cto.com/823160/1579785)
- broadcaster 演示了如何在Linux平台下收发广播包
- multicast 演示了如何在Linux平台收发UDP多播包
- common   一些常用的函数和测试代码
- thread   封装了POSIX threads API接口

Thanks
----------

谢谢你的关注，欢迎提供建议和意见，联系方式：lujun.hust@gmail.com
