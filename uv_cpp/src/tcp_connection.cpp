#include <iostream>
#include "tcp_connection.h"
#include "MemoryPool.h"
#include "tcp_callback.h"

TcpConnection::TcpConnection(TcpCallback* callback_handle)
{
	connection_handle_.data = this;
	uv_tcp_init(uv_default_loop(), &connection_handle_);
	work_request_ = new uv_work_t;
	callback_handle_ = callback_handle;
}

TcpConnection::~TcpConnection()
{
	std::cout << "~TcpConnection()" << std::endl;
}

void TcpConnection::Accept(uv_stream_t* server, int status)
{
	int ret = -1;
	ret = uv_accept(server,(uv_stream_t*)&connection_handle_);

	if (callback_handle_)
	{
		callback_handle_->OnAccept(shared_from_this(), status);
	}

	if (ret < 0)
	{
		std::cout << "TcpConnection::Accept()-->>Accept failed." << std::endl;
		return;
	}
	if (ReadStart((uv_stream_t*)&connection_handle_) < 0)
	{
		Close((uv_handle_t*)&connection_handle_, CloseCallback);
	}
}

int TcpConnection::ReadStart(uv_stream_t* connect_handle)
{
	return uv_read_start(connect_handle, AllocReadBufferCallback, ReadCallback);
}

void TcpConnection::AllocReadBufferCallback(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	TcpConnection* ptr_this = (TcpConnection*)handle->data;
	if(ptr_this)
	{
		ptr_this->DoAllocReadBuffer(handle,suggested_size,buf);
	}
}

void TcpConnection::DoAllocReadBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	char *real_buf = NULL;
	//MemoryPool<char> pool;
	//real_buf = pool.newElement();
	suggested_size = 4096;
	//memset(real_buf,0, suggested_size);
	buf->base = real_buf;
	buf->len = (ULONG)suggested_size;
}

void TcpConnection::ReadCallback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	TcpConnection* ptr_this = (TcpConnection*)stream->data;
	//uv_work_t read_req;

	if (ptr_this)
	{
		//thread_para_* thread_parameters = (thread_para_*)malloc(sizeof(thread_para_));
		//thread_parameters->stream = stream;
		//thread_parameters->nread = nread;
		//thread_parameters->buf = (uv_buf_t*)buf;
		//read_req.data = thread_parameters;
		//uv_queue_work(uv_default_loop(), &read_req, DoWork, AfterWork);
		ptr_this->DoRead(stream,nread,buf);
	}
	std::cout << "ReadCallback....." << std::endl;
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




void TcpConnection::DoRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	std::cout << "TcpConnection::DoRead:" << buf->base << ",len:" << nread << std::endl;

		//callback_handle_->OnRead(buf->base,nread);
		//uv_work_t* read_req = new uv_work_t;
		//uv_queue_work(uv_default_loop(),read_req,DoReadWork,AfterDoReadWork);
	if (nread > 0)
	{
		callback_handle_->OnRead(shared_from_this(),buf->base, nread);
	}
	else if (nread == 0)
	{
		std::cout << "nread == 0" << std::endl;
	}
	else if (nread < 0)
	{
		std::cout << "nread=" << nread << std::endl;
		Close((uv_handle_t*)&connection_handle_, CloseCallback);
	}
}

void TcpConnection::Close(uv_handle_t* handle, uv_close_cb close_cb)
{
	uv_close(handle, close_cb);
}

void TcpConnection::CloseCallback(uv_handle_t* handle)
{
	TcpConnection* ptr_this = (TcpConnection*)(handle->data);
	if(ptr_this)
	{
		ptr_this->DoClose(handle);
	}
}

void TcpConnection::DoClose(uv_handle_t* handle)
{
	close_client_cb_(shared_from_this());
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
	//callback_handle_->OnConnect(status);
	if(status == 0)
	{
		ReadStart(server_client_handle_);
	}
}


void TcpConnection::SetCloseClientCB(const CloseClientCallback& cb)
{
	close_client_cb_ = cb;
}