#include <assert.h>
#include <functional>
#include "uv.h"
#include "tcp_server.h"
#include <iostream>

TcpServer::TcpServer(TcpCallback* tcp_cb)
{
	loop_ = NULL;
	server_handle_.data = this;
	tcp_callback_ = tcp_cb;
	is_init_ = FALSE;
}

TcpServer::~TcpServer()
{
}

int TcpServer::InitServer(const char* ip,int port)
{
	int ret = -1;
	struct sockaddr_in addr;

	loop_ = uv_default_loop();

	ret = uv_ip4_addr(ip, port, &addr);
	assert(ret == 0);

	ret = uv_tcp_init(loop_, &server_handle_);
	assert(ret == 0);

	ret = uv_tcp_bind(&server_handle_, (const struct sockaddr*) &addr, 0);
	assert(ret == 0);

	uv_listen((uv_stream_t*)&server_handle_,1024, ConnectionCB);
	assert(ret == 0);
	is_init_ = TRUE;
	return ret;
}

void TcpServer::LoopThread(void* arg)
{
	uv_loop_t * uv_loop = (uv_loop_t *) arg;
	uv_run(uv_loop,UV_RUN_DEFAULT);
}

int TcpServer::StartServer()
{
	int ret = -1;
	ret = uv_thread_create(&thread_id_, &LoopThread, loop_);
	return ret;
}

void TcpServer::StopServer(void)
{
	if(!is_init_)
	{
		return;
	}

	uv_close((uv_handle_t*)&server_handle_,NULL);
	if(uv_loop_alive(loop_))
	{
		uv_stop(loop_);
	}
	uv_thread_join(&thread_id_);
	uv_loop_close(loop_);
	if(loop_)
	{
		loop_ = NULL;
	}
}

void TcpServer::ConnectionCB(uv_stream_t* server, int status)
{
	TcpServer* tcp_server = (TcpServer*)server->data;
	if(tcp_server)
	{
		tcp_server->DoConnection(server,status);
	}
}

void TcpServer::DoConnection(uv_stream_t* server, int status)
{
	TcpConnectionSmartPtr conn(new TcpConnection(server,tcp_callback_));
	connections_set_.insert(conn);
	conn->Accept(server,status);
}

void TcpServer::setConnectionCallback(const ConnectionCallback& cb)
{
	//connection_callback_ = cb;
}
void TcpServer::setCloseConnCallback(const CloseConnCallback& cb)
{
	//close_conn_callback_ = cb;
}

