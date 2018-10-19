#include <iostream>
#include"tcp_server.h"
#include "tcp_connection.h"
#include "tcp_callback.h"

class MyServer: public TcpCallback
{
public:
	MyServer(){};
	~MyServer() {};

	void OnAccept(const TcpConnectionPtr& conn, int status) 
	{ 
		std::cout << "OnAccept():" << status << std::endl;
	};
	void OnRead(const TcpConnectionPtr& conn , const char* buf, ssize_t nread)
	{
		if (nread > 0)
		{
			std::cout << "message:" << buf << std::endl;
		}
		delete buf;
	};
	void OnClose(const TcpConnectionPtr& handle) {
		std::cout << "OnClose()" << std::endl;
	};

};

int main()
{
	MyServer* my_callback = new MyServer();
	TcpServer server(my_callback);
	server.InitServer("127.0.0.1", 9800);
	server.StartServer();
	while (getchar() != 'q')
	{
		std::cout << "wait.."<<std::endl;
		Sleep(1000);
	}
	server.StopServer();
	return 0;
}

