#ifndef UVCPP_TCP_SERVER_H_
#define UVCPP_TCP_SERVER_H_

#include<set>
#include "uv.h"
#include "tcp_callbacks.h"
#include "tcp_connection.h"

UVCPP_BEGIN

class LIB_UVCPP_API TcpServer
{
public:
	TcpServer(TcpCallback* callback_handle);
	~TcpServer();

public:
	int InitServer(const std::string& ip,int port);
	int StartServer();
	void StopServer(void);

private:
	static void ConnectionCB(uv_stream_t* server, int status);
	void DoConnection(uv_stream_t* server, int status);
	static void LoopThread(void* arg);
	void RemoveConnectionHandle(const TcpConnectionPtr& conn);

private:
	typedef std::set<TcpConnectionPtr> TcpConnectionSet;
	uv_tcp_t server_handle_;
	uv_loop_t* loop_;
	TcpConnectionSet connection_handles_set_;
	uv_thread_t thread_id_;
	BOOL is_init_;
	TcpCallback* tcp_callback_handle_;
};

UVCPP_END

#endif //UVCPP_TCP_SERVER_H_