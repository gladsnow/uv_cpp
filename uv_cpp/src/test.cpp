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
		std::cout << "OnAllocBuffer" << std::endl;
		suggested_size = 10;
		*buf = new char[suggested_size];
	};

	void OnAccept(const TcpConnectionPtr& conn, int status) 
	{ 
		std::cout << "OnAccept():" << status << std::endl;
		const char* write_buffer = "hello client";
		int ret = conn->Send(write_buffer, strlen(write_buffer));
	};
	void OnRead(const TcpConnectionPtr& conn , const char* buf, ssize_t nread)
	{
		if (nread > 0)
		{
			std::cout << "Server message:" << buf <<",buf size:" << strlen(buf)<< std::endl;
		}
		delete buf;
		std::cout << "close a client" << std::endl;
		conn->Close();
	};

	void OnClose(const TcpConnectionPtr& handle) {
		std::cout << "Server OnClose()" << std::endl;
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
		std::cout << "OnConnectedSuccess" << std::endl;
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
			//std::cout << "close a client" << std::endl;
		//conn->Close();
	};

	void OnClose(const TcpConnectionPtr& handle) {
		std::cout << "Client OnClose()" << std::endl;
	};
	void OnSend(const TcpConnectionPtr& handle, int status)
	{
		std::cout << "Client OnSend" << std::endl;
	};
};

int main()
{
	//MyServer* server_callback = new MyServer();
	//TcpServer server(server_callback);
	//server.InitServer("127.0.0.1", 9800);
	//server.StartServer();
	///*while (getchar() != 'q')
	//{
	//	std::cout << "wait.."<<std::endl;
	//	Sleep(1000);
	//}*/
	//Sleep(2000);

	MyClient* client_callback = new MyClient();
	TcpClient tcp_client(client_callback);
	tcp_client.InitClient("127.0.0.1", 9800);
	tcp_client.StartClient();

	while (getchar() != 'q')
	{
		std::cout << "wait.." << std::endl;
		Sleep(1000);
	}
	//server.StopServer();
	tcp_client.StopClient();
	return 0;
}
