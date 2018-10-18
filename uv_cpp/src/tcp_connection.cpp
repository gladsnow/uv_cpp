#include "tcp_connection.h"
#include <iostream>

TcpConnection::TcpConnection()
{
	uv_tcp_init(uv_default_loop(),&connection_handle_);
}

TcpConnection::TcpConnection(uv_stream_t* connection_handle,TcpCallback* callback)
{
	connection_handle_.data = this;
	uv_tcp_init(uv_default_loop(),&connection_handle_);
	work_request_ = new uv_work_t;
	callback_handle_ = callback;
	server_client_handle = connection_handle;
	server_client_handle->data=this;
}

TcpConnection::~TcpConnection()
{

}

int TcpConnection::ReadStart(uv_stream_t* connect_handle)
{
	return uv_read_start(connect_handle,AllocReadBufferCallback,ReadCallback);
}

void TcpConnection::Accept(uv_stream_t* server, int status)
{
	int ret = -1;
	ret = uv_accept(server,(uv_stream_t*)&connection_handle_);
	if(ret >= 0)
	{
		callback_handle_->OnAccept(status);
		ReadStart((uv_stream_t*)&connection_handle_);
	}
}

void TcpConnection::Close(uv_handle_t* handle, uv_close_cb close_cb)
{
	uv_close(handle, close_cb);
}

void TcpConnection::AllocReadBufferCallback(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	TcpConnection* conn = (TcpConnection*)handle->data;
	if(conn)
	{
		conn->DoAllocReadBuffer(handle,suggested_size,buf);
	}
}

void TcpConnection::DoAllocReadBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	int real_buf_size = suggested_size;
	char *real_buf = NULL;
	callback_handle_->OnAllocBuffer(real_buf_size,&real_buf);
	memset(real_buf,0,real_buf_size);
	buf->base = real_buf;
	buf->len = real_buf_size;
}


void DoWork(uv_work_t* work_req)
{
	TcpConnection* conn =(TcpConnection*) ((thread_para_*)(work_req->data))->stream->data;
	uv_stream_t* stream = ((thread_para_*)(work_req->data))->stream;
	ssize_t nread = ((thread_para_*)(work_req->data))->nread;
	uv_buf_t* buf =  ((thread_para_*)(work_req->data))->buf;
	conn->DoRead(stream,nread,buf);
}

void AfterWork(uv_work_t* req, int status)
{
	free(req->data);
}

void TcpConnection::ReadCallback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	TcpConnection* conn = (TcpConnection*)stream->data;
	uv_work_t read_req;

	if(conn)
	{

		thread_para_* thread_parameters =(thread_para_*) malloc(sizeof(thread_para_));
		thread_parameters->stream = stream;
		thread_parameters->nread = nread;
		thread_parameters->buf = (uv_buf_t*)buf;
		read_req.data = thread_parameters;
		uv_queue_work(uv_default_loop(),&read_req,DoWork,AfterWork);
		//conn->DoRead(stream,nread,buf);
	}
	std::cout << "ReadCallback....." << std::endl;
}


void TcpConnection::DoRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{

	if(nread >= 0)
	{
		
		//std::cout << "TcpConnection::DoRead:" << buf->base << ",len:"<< nread << std::endl;
		callback_handle_->OnRead(buf->base,nread);
		//uv_work_t* read_req = new uv_work_t;
		//uv_queue_work(uv_default_loop(),read_req,DoReadWork,AfterDoReadWork);
	}
	else if (nread < 0)
	{
		uv_close((uv_handle_t*)&connection_handle_, CloseCallback);
	}
}

void TcpConnection::CloseCallback(uv_handle_t* handle)
{
	TcpConnection* conn = (TcpConnection*)(handle->data);
	if(conn)
	{
		conn->DoClose(handle);
	}
}

void TcpConnection::DoClose(uv_handle_t* handle)
{
	callback_handle_->OnClose();
}

void TcpConnection::Connect(uv_connect_t* req,uv_tcp_t* client,const struct sockaddr* addr)
{
	req->data = this;
	
	uv_tcp_connect(req,client,addr,ConnectCallback);
	
}

void TcpConnection::ConnectCallback(uv_connect_t* req, int status)
{
	TcpConnection* p_client = (TcpConnection*) req->data;

	if(p_client)
	{
		p_client->DoConnect(req,status);
	}
}

void TcpConnection::DoConnect(uv_connect_t* req, int status)
{
	callback_handle_->OnConnect(status);
	if(status == 0)
	{
		ReadStart(server_client_handle);
	}
}
