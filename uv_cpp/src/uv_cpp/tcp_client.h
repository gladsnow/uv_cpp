#ifndef UVCPP_TCP_CLIENT_H_
#define UVCPP_TCP_CLIENT_H_

#include "uv.h"
#include "tcp_callbacks.h"
#include "tcp_connection.h"

UVCPP_BEGIN

class LIB_UVCPP_API TcpClient
{
public:
	TcpClient(TcpCallback* tcp_cb);
	~TcpClient();

public:
	int InitClient(const char* ip,int port);
	int StartClient();
	void StopClient();
	void SetReconnect(BOOL reconnect_flag);


private:
	static void LoopThread(void* arg);

private:
	uv_loop_t* loop_;
	TcpCallback* tcp_callback_;
	TcpConnectionPtr tcp_connection_;
	uv_thread_t thread_id_;
	uv_tcp_t client_handle_;
	uv_connect_t connect_req_;
	struct sockaddr_in addr_;
	BOOL reconnect_flag_;
};

UVCPP_END
#endif	//UVCPP_TCP_CLIENT_H_
