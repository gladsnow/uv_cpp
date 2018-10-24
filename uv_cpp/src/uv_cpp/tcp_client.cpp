#include "tcp_client.h"

UVCPP_BEGIN

TcpClient::TcpClient(TcpCallback* tcp_cb)
{
	loop_ = uv_default_loop();
	tcp_callback_ = tcp_cb;
	tcp_connection_ = NULL;
	reconnect_flag_ = FALSE;
}
TcpClient::~TcpClient()
{
}

int TcpClient::InitClient(const char* ip,int port)
{
	if (!tcp_callback_)
	{
		return -1;
	}
	TcpConnectionPtr tcp_conn(new TcpConnection(tcp_callback_));
	tcp_connection_ = tcp_conn;
	uv_ip4_addr(ip, port, &addr_);
	tcp_conn->Connect(connect_req_, (const struct sockaddr*)&addr_,reconnect_flag_);
	return 0;
}

int TcpClient::StartClient()
{
	int ret = -1;
	ret = uv_thread_create(&thread_id_, &LoopThread, uv_default_loop());
	return ret;
}

void TcpClient::StopClient()
{
	tcp_connection_->Close();
	uv_thread_join(&thread_id_);

	uv_loop_close(loop_);
	if (loop_)
	{
		loop_ = NULL;
	}
}

void TcpClient::LoopThread(void* arg)
{
	uv_loop_t * uv_loop = (uv_loop_t *)arg;
	uv_run(uv_loop, UV_RUN_DEFAULT);
}

void TcpClient::SetReconnect(BOOL reconnect_flag)
{
	reconnect_flag_ = reconnect_flag;
}

UVCPP_END
