
C Library Code
===========

本目录下的代码均是 C 代码，大多数代码基于 Linux 平台。


Code List
----------

- main.c   演示了带命令行参数的 main 函数的使用
- rw_queue 实现了一个基于 idx 的环形缓冲区队列,常用于“生产者-消费者”应用场景
- ring_buffer 参考 Linux 内核的 kfifo 实现了一个基于字节流的环形缓冲区
- synclock 封装了 mutex 和 condition 代码，方便在其他项目中快速集成条件同步功能
- keylist  实现了一个带有键值 (key) 的双向链表，可以通过 key 直接访问某个节点
- pipe     封装了Linux系统的无名管道（pipe），可以方便地集成到 C 语言项目中作为“队列”来使用
- syslog   演示如何构建一个“专业”的 Log 系统，相关博文：[构建专业的log系统](http://ticktick.blog.51cto.com/823160/1579785)
- broadcaster 演示了如何在 Linux 平台下收发广播包
- multicast 演示了如何在 Linux 平台收发 UDP 多播包
- common   一些常用的函数和测试代码
- thread   封装了 POSIX threads API 接口
- ffmpeg  一些关于 ffmpeg 的代码封装和示例


Thanks
----------

谢谢你的关注，欢迎提供建议和意见，联系方式：lujun.hust@gmail.com
