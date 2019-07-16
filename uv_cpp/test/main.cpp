//#include <iostream>
//#include "tcp_callbacks.h"
//#include "tcp_server.h"
//#include "tcp_client.h"
//#include <thread>
//#include <windows.h>
//using namespace uvcpp;
//
//class MyCallback: public TcpCallback
//{
//public:
//	MyCallback(){};
//	~MyCallback(){};
//
//	void OnAccept(int status)
//	{
//		std::cout << "Server---On Accept:" << status << std::endl;
//	};
//	void OnRead(const char* buf,int n_read)
//	{
//		std::cout << "Server---On Read:" << buf << std::endl;
//		delete buf;
//		//Sleep(5000);
//	};
//	void OnAllocBuffer(const TcpConnectionPtr& handle, size_t& suggested_size, char **buf)
//	{
//		suggested_size = 1024;
//		*buf = new char[suggested_size]();
//	};
//	void OnClose()
//	{
//		std::cout << "Server---OnClose..." << std::endl;
//	}
//};
//
//class ClientCallback: public TcpCallback
//{
//public:
//	ClientCallback(){};
//	~ClientCallback(){};
//
//	void OnAccept(int status)
//	{
//		std::cout << "On Accept:" << status << std::endl;
//	};
//	void OnRead(const char* buf,int n_read)
//	{
//		std::cout << "nread:" << n_read << std::endl;
//		if (n_read > 0)
//		{
//			std::cout << "On Read:" << buf << std::endl;
//			delete buf;
//		}
//		
//	};
//	void OnAllocBuffer(const TcpConnectionPtr& handle, size_t& suggested_size, char **buf)
//	{
//		suggested_size = 1024;
//		*buf = new char[suggested_size]();
//	};
//	void OnClose()
//	{
//		std::cout << "Client---OnClose..." << std::endl;
//	}
//	void OnConnect(int status)
//	{
//		std::cout << "Client---OnConnect:"<< status << std::endl;
//	}
//};
//
//
//void startThread(int i)
//{
//	std::cout << "thread:" << i << std::endl;
//	TcpCallback* client_cb =(TcpCallback*) new ClientCallback();
//	TcpClient* myClient = new TcpClient(client_cb);
//	myClient->InitClient("192.168.6.92",8900);
//	myClient->StartClient();
//}
//
//int main()
//{
//	TcpCallback* myCallback =(TcpCallback*) new MyCallback();
//	//TcpCallback* client_cb =(TcpCallback*) new ClientCallback();
//	TcpServer* myServer = new TcpServer(myCallback);
//
//	myServer->InitServer("192.168.6.92",8900);
//	myServer->StartServer();
//
//	while(true)
//	{
//		std::cout << "wait.."<<std::endl;
//		Sleep(1000);
//	}
//	myServer->StopServer();
//
//	 //for(int i = 0;i < 10;i++)
//	 //{
//		// TcpClient* myClient = new TcpClient(myCallback);
//		// myClient->InitClient("127.0.0.1",8900);
//		// myClient->StartClient();
//		// client_array[i] = myClient;
//	 //}
//
//	//Sleep(2000);
//
//	//for(int i = 0;i < 10;i++)
//	//{
//	 
//	/*std::thread threads[5];
//	std::cout << "Spawning 5 threads...\n";
//	for (int i = 0; i < 5; i++) {
//		threads[i] = std::thread(startThread, i + 1);
//		Sleep(1000);
//	}
//	std::cout << "Done spawning threads! Now wait for them to join\n" << std::endl;
//	for (auto& t: threads) {
//		t.join();
//	}*/
//
//	//	TcpClient* myClient = new TcpClient(client_cb);
////		myClient->InitClient("127.0.0.1",8900);
//	//	myClient->StartClient();
//	//	client_array[i] = myClient;
//	//}
//
//	
//	std::cout << "over.."<<std::endl;
//	return 0;
//}