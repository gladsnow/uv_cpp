#include <assert.h>
#include <functional>
#include "tcp_server.h"
#include <iostream>

UVCPP_BEGIN

TcpServer::TcpServer(TcpCallback * callback_handle)
{
	loop_ = NULL;
	server_handle_.data = this; //保存类指针，调用成员函数，实现回调函数
	tcp_callback_handle_ = callback_handle;
	is_init_ = FALSE;
}

TcpServer::~TcpServer()
{
}

int TcpServer::InitServer(const std::string& ip,int port)
{
	int ret = -1;
	struct sockaddr_in addr;

	loop_ = uv_default_loop();

	ret = uv_ip4_addr(ip.c_str(), port, &addr);
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
	TcpConnectionSet::iterator iter = connection_handles_set_.begin();
	for (;iter != connection_handles_set_.end(); iter++)
	{
		(*iter)->Close();
	}
	uv_close((uv_handle_t*)&server_handle_,NULL);
	uv_thread_join(&thread_id_);
	
	uv_loop_close(loop_);
	if(loop_)
	{
		loop_ = NULL;
	}
}

void TcpServer::ConnectionCB(uv_stream_t* server, int status)
{
	TcpServer* ptr_this = (TcpServer*)server->data;
	if(ptr_this)
	{
		ptr_this->DoConnection(server,status);
	}
}

void TcpServer::DoConnection(uv_stream_t* server, int status)
{
	TcpConnectionPtr conn(new TcpConnection(tcp_callback_handle_));
	connection_handles_set_.insert(conn);
	conn->SetCloseClientCB(std::bind(&TcpServer::RemoveConnectionHandle, this, std::placeholders::_1));
	conn->Accept(server,status);
}

void  TcpServer::RemoveConnectionHandle(const TcpConnectionPtr& conn)
{
	//std::cout << "TcpServer::RemoveConnectionHandle" << std::endl;
	connection_handles_set_.erase(conn);
}

UVCPP_END