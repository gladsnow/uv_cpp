#ifndef UVCPP_TCP_SERVER_H_
#define UVCPP_TCP_SERVER_H_

#include<set>
#include "uv.h"
#include "tcp_connection.h"
#include "callbacks.h"
class TcpServer
{
public:
	TcpServer(TcpCallback* tcp_cb);
	~TcpServer();

public:
	int InitServer(const char* ip,int port);
	int StartServer();
	static void ConnectionCB(uv_stream_t* server, int status);
	void DoConnection(uv_stream_t* server, int status);
	void StopServer(void);

	static void LoopThread(void* arg);

	void setConnectionCallback(const ConnectionCallback& cb);
	void setCloseConnCallback(const CloseConnCallback& cb);



public:
typedef std::set<TcpConnectionSmartPtr> TcpConnectionSet;
	uv_tcp_t server_handle_;
	uv_loop_t* loop_;
	TcpConnectionSet connections_set_;
	TcpCallback* tcp_callback_;
	uv_thread_t thread_id_;
	BOOL is_init_;
	//ConnectionCallback connection_callback_;
	//CloseConnCallback close_conn_callback_;
};



#endif //UVCPP_TCP_SERVER_H_