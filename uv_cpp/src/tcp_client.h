#ifndef UVCPP_TCP_CLIENT_H_
#define UVCPP_TCP_CLIENT_H_

#include "uv.h"
#include "tcp_callback.h"
#include "tcp_connection.h"

class TcpClient
{
public:
	TcpClient(TcpCallback* tcp_cb);
	~TcpClient();

public:
	void InitClient(const char* ip,int port);
	void StartClient();
	static void ConnectCallback(uv_connect_t* req, int status);
	void DoConnect(uv_connect_t* req, int status);

private:
	uv_tcp_t client_handle_;
	uv_connect_t connect_req_;
	uv_loop_t* loop_;
	TcpCallback* tcp_callback_;
	TcpConnection* tcp_connection_;
};


#endif	//UVCPP_TCP_CLIENT_H_
