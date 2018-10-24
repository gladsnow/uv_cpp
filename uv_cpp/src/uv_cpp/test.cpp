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
		}
		write_buffer = new char[nread];
		memcpy(write_buffer, buf, nread);
		int ret = conn->Send(write_buffer, nread);
		//std::cout << "close a client" << std::endl;
		//conn->Close();
		delete buf;
	};

	void OnClose(const uvcpp::TcpConnectionPtr& handle) {
		std::cout << "Server OnClose(const TcpConnectionPtr& handle)" << std::endl;
	};
	void OnSend(const uvcpp::TcpConnectionPtr& handle, int status)
	{
		std::cout << "server OnSend" << std::endl;
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

	/*MyServer* server_callback = new MyServer();
	TcpServer server(server_callback);
	server.InitServer("192.168.6.51", 9800);
	server.StartServer();
	while (getchar() != 'q')
	{
		std::cout << "wait.."<<std::endl;
		Sleep(1000);
	}
	server.StopServer();*/

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
//#include <inttypes.h> 
//int random(int m) {
//	return rand() % m;//將随机数控制在0~m-1之间
//}
//
//void on_after_work(uv_work_t* req, int status) {
//	free(req);
//}
//void on_work(uv_work_t* req) {
//	// "Work"
//	if (random(100) % 5 == 0) {
//		printf("Sleeping...\n");
//		Sleep(3000);
//		printf("sleep over,on work.\n");
//	}
//}
//void on_timer(uv_timer_t* timer) {
//	uint64_t timestamp = uv_hrtime();
//	printf("on_timer [%" PRIu64 " ms]\n", (timestamp / 1000000) % 100000);
//	uv_work_t* work_req = (uv_work_t*)malloc(sizeof(*work_req));
//	uv_queue_work(uv_default_loop(), work_req, on_work, on_after_work);
//	printf("After queue\n");
//}
//int main(int argc, const char** argv) {
//	uv_timer_t timer;
//	uv_timer_init(uv_default_loop(), &timer);
//	uv_timer_start(&timer, on_timer, 0, 1000);
//	 uv_run(uv_default_loop(), UV_RUN_DEFAULT);
//	 printf("uv_run()");
//	return 0;
//}