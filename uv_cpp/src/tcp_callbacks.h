#ifndef UVCPP_TCP_CALLBACK_H_
#define UVCPP_TCP_CALLBACK_H_

#include "tcp_connection.h"

class TcpCallback
{
public:
	TcpCallback(){};
	virtual ~TcpCallback(){};

	//Common Callbacks
	virtual void OnAllocBuffer(const TcpConnectionPtr& handle, size_t& suggested_size, char **buf)  = 0;
	virtual void OnRead(const TcpConnectionPtr& handle, const char* buf, ssize_t nread) {};
	virtual void OnSend(const TcpConnectionPtr& handle, int status) {};

	//Callbacks for a client
	virtual void OnConnectedSuccess(const TcpConnectionPtr& handle, int status){};
	virtual void OnConnectedFailed(const TcpConnectionPtr& handle, int status) {};
	virtual void OnClose(){};
	

	//Callbacks for a server
	virtual void OnAccept(const TcpConnectionPtr& handle, int status) {};
	virtual void OnClose(const TcpConnectionPtr& handle) {};
};


#endif	//UVCPP_TCP_CALLBACK_H_
