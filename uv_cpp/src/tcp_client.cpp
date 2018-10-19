#include "tcp_client.h"
#include <assert.h>

TcpClient::TcpClient(TcpCallback* tcp_cb)
{
	loop_ = NULL;
	connect_req_.data = tcp_connection_;
	tcp_callback_ = tcp_cb;
	tcp_connection_ = NULL;
}
TcpClient::~TcpClient()
{

}

void TcpClient::InitClient(const char* ip,int port)
{
	struct sockaddr_in addr;
	
	loop_ = uv_default_loop();
	uv_ip4_addr(ip, port, &addr);

	uv_tcp_init(loop_,&client_handle_);
	//tcp_connection_ = new TcpConnection();
	//connect_req_.data = tcp_connection_;
	//tcp_connection_->Connect(&connect_req_,&client_handle_,(const struct sockaddr*)&addr);
}

void TcpClient::StartClient()
{
	int ret = -1;
	assert(loop_ != NULL);
	
	ret = uv_run(loop_,UV_RUN_DEFAULT);
	assert(ret == 0);
}





