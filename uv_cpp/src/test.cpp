#include <iostream>
#include"tcp_server.h"
#include "tcp_connection.h"
#include "tcp_callbacks.h"
#include "tcp_client.h"

class MyServer: public TcpCallback
{
public:
	MyServer(){};
	~MyServer() {};

	void OnAllocBuffer(const TcpConnectionPtr& handle, size_t& suggested_size, char **buf)
	{
		//std::cout << "OnAllocBuffer" << std::endl;
		suggested_size = 1024;
		*buf = new char[suggested_size];
	};

	void OnAccept(const TcpConnectionPtr& conn, int status) 
	{ 
		std::cout << "OnAccept():" << status << std::endl;

	};
	void OnRead(const TcpConnectionPtr& conn , const char* buf, ssize_t nread)
	{
		if (nread > 0)
		{
			std::cout << "Server message:" << buf <<",buf size:" << strlen(buf)<< std::endl;
		}
		delete buf;
		char* write_buffer = (char*)"hello client";
		int ret = conn->Send(write_buffer, strlen(write_buffer));
		std::cout << "close a client" << std::endl;
		conn->Close();
	};

	void OnClose(const TcpConnectionPtr& handle) {
		std::cout << "Server OnClose(const TcpConnectionPtr& handle)" << std::endl;
	};
};

class MyClient : public TcpCallback
{
public:
	MyClient() {};
	~MyClient() {};

	void OnAllocBuffer(const TcpConnectionPtr& handle, size_t& suggested_size, char **buf)
	{
		std::cout << "OnAllocBuffer" << std::endl;
		suggested_size = 1024;
		*buf = new char[suggested_size];
	};

	void OnConnectedSuccess(const TcpConnectionPtr& handle, int status) 
	{
		std::cout << "OnConnectedSuccess" << std::endl;
	};
	void OnConnectedFailed(const TcpConnectionPtr& handle, int status) 
	{
		std::cout << "OnConnectedFailed" << std::endl;
	};

	void OnRead(const TcpConnectionPtr& conn, const char* buf, ssize_t nread)
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
	void OnSend(const TcpConnectionPtr& handle, int status)
	{
		std::cout << "Client OnSend" << std::endl;
	};
	void OnReconnect() 
	{
		std::cout << "OnReconnect()" << std::endl;
	};

};

int main()
{
	MyServer* server_callback = new MyServer();
	TcpServer server(server_callback);
	server.InitServer("127.0.0.1", 9800);
	server.StartServer();
	while (getchar() != 'q')
	{
		std::cout << "wait.."<<std::endl;
		Sleep(1000);
	}
	server.StopServer();

	//MyClient* client_callback = new MyClient();
	//TcpClient tcp_client(client_callback);
	//tcp_client.InitClient("127.0.0.1", 9800);
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