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
	//google::SetLogDestination(google::GLOG_INFO, "INFO_"); //���� google::INFO �������־�洢·�����ļ���ǰ׺
	//google::SetLogDestination(google::GLOG_WARNING,"WARNING_");   //���� google::WARNING �������־�洢·�����ļ���ǰ׺
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
	//LOG(INFO) << "info test" << "hello log!";  //���һ��Info��־
	//LOG(WARNING) << "warning test";  //���һ��Warning��־
	//LOG(ERROR) << "error test";  //���һ��Error��־
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
//libuv�������У�
//1.���Ӻ�ʱ�Ĺ������ݷ��ڹ������У����̳߳�������;
//2.�̳߳ش������������Ļص��������ص����̼߳�������;
//*/
//
//static void uv_work_callback(uv_work_t* req)
//{
//	//����һ���߳���ִ��
//	printf("work id: %d\n", (int)uv_thread_self());
//	printf("work data: %d\n", (int)req->data);
//}
//
//static void uv_after_work_callback(uv_work_t* req, int status)
//{
//	//���߳���ִ��
//	printf("after work id: %d\n", (int)uv_thread_self());
//}
//
////ÿ�������д�ӡһ���߳�id
////mian: id = 0;
////uv_work_callback: id = 156;
////uv_after_work_callback: id = 0;
////˵��uv_work_callback����һ���߳���ִ��;
//int main(int argc, char** argv)
//{
//	printf("main id: %d\n", (int)uv_thread_self());
//
//	uv_loop_t *uv_loop = uv_default_loop();
//
//	//�����������ݽṹ,�Զ������ݽṹ�����data��
//	uv_work_t uv_work;
//	uv_work.data = (void*)2018;
//
//	//��ӹ�������libuv�Ķ����У�libuv���̳߳ص��̻߳�����󼴿ɴ������uv_work_callback������������Ϻ����uv_after_work_callback����
//	uv_queue_work(uv_loop, &uv_work, uv_work_callback, uv_after_work_callback);
//
//	uv_run(uv_loop, UV_RUN_DEFAULT);
//
//	system("pause");
//	return 0;
//}