#ifndef UVCPP_TCP_SERVER_H_
#define UVCPP_TCP_SERVER_H_

#include<set>
#include "uv.h"
#include "tcp_connection.h"
#include "tcp_callback.h"

class TcpServer
{
public:
	TcpServer(TcpCallback* callback_handle);
	~TcpServer();

public:
	int InitServer(const std::string& ip,int port);
	int StartServer();
	static void ConnectionCB(uv_stream_t* server, int status);
	void DoConnection(uv_stream_t* server, int status);
	void StopServer(void);

	static void LoopThread(void* arg);

	void RemoveConnectionHandle(const TcpConnectionPtr& conn);

public:
typedef std::set<TcpConnectionPtr> TcpConnectionSet;
	uv_tcp_t server_handle_;
	uv_loop_t* loop_;
	TcpConnectionSet connection_handles_set_;
	uv_thread_t thread_id_;
	BOOL is_init_;
	TcpCallback* tcp_callback_handle_;
};

#endif //UVCPP_TCP_SERVER_H_