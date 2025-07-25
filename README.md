# TCP通信
## TCP通信流程图
![image](https://github.com/Lenmoncc/socket-learning/blob/main/image.png)  
## 程序结构
socket-learning/  
├── src/  
│   ├── client.c      // 客户端代码  
│   ├── server.c      // 基础服务器代码(单连接)  
│   └── server_1.c    // 多进程服务器代码(支持多客户端)  
├── LICENSE  
└── README.md 
## 功能说明
## 1.客户端 (client.c)  
- 建立 TCP 连接到指定服务器  
- 周期性发送固定消息 "client sended!"  
- 接收并打印服务器返回的响应  
## 2.基础服务器 (server.c)  
- 监听 10000 端口  
- 只能处理一个客户端连接  
- 接收客户端消息并返回 "send succeed!" 响应  
## 3.多进程服务器 (server_1.c)  
- 支持同时处理多个客户端连接  
- 通过创建子进程为每个客户端提供服务  
- 实现子进程资源自动回收机制  
- 处理信号中断问题  
## 其他
本项目是在Linux操作系统环境下编译和运行（Ubuntu 20.04版本）  
需要安装GCC编译器（`sudo apt install gcc`）  
