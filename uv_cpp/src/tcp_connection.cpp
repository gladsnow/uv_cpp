#include <iostream>
#include "tcp_connection.h"
#include "MemoryPool.h"
#include "tcp_callbacks.h"

TcpConnection::TcpConnection(TcpCallback* callback_handle)
{
	connection_handle_.data = this;
	uv_tcp_init(uv_default_loop(), &connection_handle_);
	work_request_ = new uv_work_t;
	callback_handle_ = callback_handle;
	write_req_.data = this;
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
		CloseSocket((uv_handle_t*)&connection_handle_, CloseCallback);
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
	if (callback_handle_)
	{
		char *real_buf = NULL;
		size_t real_size = suggested_size;
		callback_handle_->OnAllocBuffer(shared_from_this(), real_size, &real_buf);
		memset(real_buf,0, real_size);
		buf->base = real_buf;
		buf->len = (ULONG)real_size;
	}
}

void TcpConnection::ReadCallback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	TcpConnection* ptr_this = (TcpConnection*)stream->data;

	if (ptr_this)
	{
		ptr_this->DoRead(stream,nread,buf);
	}
	std::cout << "ReadCallback....." << std::endl;
}

void TcpConnection::DoRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
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
		CloseSocket((uv_handle_t*)&connection_handle_, CloseCallback);
	}
}

void TcpConnection::CloseSocket(uv_handle_t* handle, uv_close_cb close_cb)
{
	uv_close(handle, close_cb);
	callback_handle_->OnClose(shared_from_this());
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
	if (close_client_cb_)
	{
		close_client_cb_(shared_from_this());
	}
}

void TcpConnection::Close(void)
{
	CloseSocket((uv_handle_t*)&connection_handle_, CloseCallback);
}

void TcpConnection::Connect(uv_connect_t& req,const struct sockaddr* addr)
{
	req.data = this;
	uv_tcp_connect(&req, &connection_handle_,addr, AfterConnectCB);
	
}

void TcpConnection::AfterConnectCB(uv_connect_t* req, int status)
{
	TcpConnection* p_client = (TcpConnection*) req->data;

	if(p_client)
	{
		p_client->DoAfterConnect(req,status);
	}
}

void TcpConnection::DoAfterConnect(uv_connect_t* req, int status)
{
	if (callback_handle_)
	{
		
		if (status == 0)
		{
			callback_handle_->OnConnectedSuccess(shared_from_this(), status);
			ReadStart((uv_stream_t*)&connection_handle_);
		}
		else
		{
			callback_handle_->OnConnectedFailed(shared_from_this(), status);
		}
	}
}

int TcpConnection::Send(const char* buf,unsigned int nbufs)
{
	uv_buf_t write_buffer;
	write_buffer.base = (char*) buf;
	write_buffer.len = nbufs;
	return uv_write(&write_req_, (uv_stream_t*)&connection_handle_, &write_buffer,1, AfterWriteCB);
}

void TcpConnection::AfterWriteCB(uv_write_t* req, int status)
{
	TcpConnection* ptr_this = (TcpConnection*)req->data;
	if (ptr_this)
	{
		ptr_this->DoAfterWrite(req, status);
	}
}

void TcpConnection::DoAfterWrite(uv_write_t* req, int status)
{
	if (callback_handle_)
	{
		callback_handle_->OnSend(shared_from_this(), status);
	}
}

void TcpConnection::SetCloseClientCB(const CloseClientCallback& cb)
{
	close_client_cb_ = cb;
}