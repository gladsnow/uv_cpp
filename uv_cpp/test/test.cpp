#include <iostream>
#include"tcp_server.h"
#include "tcp_connection.h"
#include "tcp_callbacks.h"
#include "tcp_client.h"

//#define GLOG_NO_ABBREVIATED_SEVERITIES
//#include <windows.h>  
//#include "glog/logging.h"
//
//using namespace google;
using namespace uvcpp;

class MyServer: public uvcpp::TcpCallback
{
public:
	MyServer() { write_buffer = NULL; };
	~MyServer() {};

	void OnAllocBuffer(const uvcpp::TcpConnectionPtr& handle, size_t& suggested_size, char **buf)
	{
		//std::cout << "OnAllocBuffer" << std::endl;
		suggested_size = 1024;
		*buf = new char[suggested_size];
	};

	void OnAccept(const uvcpp::TcpConnectionPtr& conn, int status)
	{ 
		std::cout << "OnAccept():" << status << std::endl;

	};
	void OnRead(const uvcpp::TcpConnectionPtr& conn , const char* buf, ssize_t nread)
	{
		if (nread > 0)
		{
			//std::cout << "Server message:" << buf <<",buf size:" << strlen(buf)<< std::endl;
			int ret = conn->Send(buf, nread);

			delete buf;
			buf = NULL;
		}

		//conn->Close();
		
	};

	void OnClose(const uvcpp::TcpConnectionPtr& handle) {
		//std::cout << "Server OnClose(const TcpConnectionPtr& handle)" << std::endl;
	};
	void OnSend(const uvcpp::TcpConnectionPtr& handle, int status)
	{
		//std::cout << "server OnSend" << std::endl;
		if (write_buffer)
		{
			delete write_buffer;
			write_buffer = NULL;
		}
	};
private:
	char* write_buffer;
};

class MyClient : public uvcpp::TcpCallback
{
public:
	MyClient() {};
	~MyClient() {};

	void OnAllocBuffer(const uvcpp::TcpConnectionPtr& handle, size_t& suggested_size, char **buf)
	{
		std::cout << "OnAllocBuffer" << std::endl;
		suggested_size = 1024;
		*buf = new char[suggested_size];
	};

	void OnConnectedSuccess(const uvcpp::TcpConnectionPtr& handle, int status)
	{
		std::cout << "OnConnectedSuccess" << std::endl;
	};
	void OnConnectedFailed(const uvcpp::TcpConnectionPtr& handle, int status)
	{
		std::cout << "OnConnectedFailed" << std::endl;
	};

	void OnRead(const uvcpp::TcpConnectionPtr& conn, const char* buf, ssize_t nread)
	{
		if (nread > 0)
		{
			std::cout << "client message:" << buf << ",buf size:" << strlen(buf) << std::endl;
		}
		delete buf;
		char* write_buffer = (char*)"hello server";
		int ret = conn->Send(write_buffer, strlen(write_buffer));
		std::cout << "close socket" << std::endl;
		conn->Close();
	};
	void OnClose() {
		std::cout << "Client OnClose()" << std::endl;
	};
	void OnSend(const uvcpp::TcpConnectionPtr& handle, int status)
	{
		std::cout << "Client OnSend" << std::endl;
	};
	void OnReconnect() 
	{
		std::cout << "OnReconnect()" << std::endl;
	};

};

int main(int argc, char* argv[])
{
	//// Start google log system:
	////FLAGS_log_dir = argv[0];
	//std::cout << argv[0] << std::endl;
	//google::InitGoogleLogging("test");
	////google::SetLogDestination(google::GLOG_INFO, "E:\\Workspace\\CppProject\\libuv-1.23.0\\x64\\Debug\\Logs\\libuvcpp_");
	//google::SetStderrLogging(google::NUM_SEVERITIES);
	//google::SetLogDestination(google::GLOG_INFO, "INFO_"); //设置 google::INFO 级别的日志存储路径和文件名前缀
	//google::SetLogDestination(google::GLOG_WARNING,"WARNING_");   //设置 google::WARNING 级别的日志存储路径和文件名前缀
	//google::SetLogDestination(google::GLOG_ERROR, "ERROR_");
	//google::SetLogFilenameExtension("log");
	//FLAGS_colorlogtostderr = true;  // Set log color
	//FLAGS_logbufsecs = 0;  // Set log output speed(s)
	//FLAGS_max_log_size = 1024;  // Set max log file size
	//FLAGS_stop_logging_if_full_disk = true;  // If disk is full
	//char str[20] = "hello log!";
	//LOG(INFO) << str;
	//std::string cStr = "hello google!";
	//LOG(INFO) << cStr;
	//LOG(INFO) << "info test" << "hello log!";  //输出一个Info日志
	//LOG(WARNING) << "warning test";  //输出一个Warning日志
	//LOG(ERROR) << "error test";  //输出一个Error日志
	//google::ShutdownGoogleLogging();

	MyServer* server_callback = new MyServer();
	TcpServer server(server_callback);
	server.InitServer("192.168.6.92", 8900);
	server.StartServer();
	while (getchar() != 'q')
	{
		std::cout << "wait.."<<std::endl;
		Sleep(1000);
	}
	server.StopServer();

	//MyClient* client_callback = new MyClient();
	//TcpClient tcp_client(client_callback);
	//tcp_client.InitClient("192.168.6.51", 9800);
	//tcp_client.StartClient();

	//while (getchar() != 'q')
	//{
	//	std::cout << "wait.." << std::endl;
	//	Sleep(1000);
	//}
	//
	//tcp_client.StopClient();
	return 0;
}


//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//
//#include "uv.h"
//
///*
//libuv工作队列：
//1.复杂耗时的工作内容放在工作队列，让线程池来处理;
//2.线程池处理完后会调传入的回调函数，回到主线程继续处理;
//*/
//
//static void uv_work_callback(uv_work_t* req)
//{
//	//另外一个线程中执行
//	printf("work id: %d\n", (int)uv_thread_self());
//	printf("work data: %d\n", (int)req->data);
//}
//
//static void uv_after_work_callback(uv_work_t* req, int status)
//{
//	//主线程中执行
//	printf("after work id: %d\n", (int)uv_thread_self());
//}
//
////每个函数中打印一下线程id
////mian: id = 0;
////uv_work_callback: id = 156;
////uv_after_work_callback: id = 0;
////说明uv_work_callback在另一个线程中执行;
//int main(int argc, char** argv)
//{
//	printf("main id: %d\n", (int)uv_thread_self());
//
//	uv_loop_t *uv_loop = uv_default_loop();
//
//	//创建工作数据结构,自定义数据结构添加在data中
//	uv_work_t uv_work;
//	uv_work.data = (void*)2018;
//
//	//添加工作任务到libuv的队列中，libuv的线程池的线程获得锁后即可处理调用uv_work_callback函数，处理完毕后调用uv_after_work_callback函数
//	uv_queue_work(uv_loop, &uv_work, uv_work_callback, uv_after_work_callback);
//
//	uv_run(uv_loop, UV_RUN_DEFAULT);
//
//	system("pause");
//	return 0;
//}