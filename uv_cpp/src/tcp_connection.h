#ifndef UVCPP_TCP_CONNECTION_H_
#define UVCPP_TCP_CONNECTION_H_

#include <memory>
#include <functional>
#include "uv.h"
#include "callbacks.h"

typedef struct thread_para
{
	uv_stream_t* stream;
	ssize_t nread;
	uv_buf_t* buf;
}thread_para_;


class TcpCallback;

class TcpConnection :public std::enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(TcpCallback* callback_handle);
	~TcpConnection();

	int ReadStart(uv_stream_t* server);
	void Accept(uv_stream_t* server, int status);
	void Connect(uv_connect_t* req,uv_tcp_t* client,const struct sockaddr* addr);
	void Close(uv_handle_t* handle, uv_close_cb close_cb);

	static void AllocReadBufferCallback(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
	void DoAllocReadBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

	static void ReadCallback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	void DoRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);

	//void DoAcceptWork(uv_work_t* Accept_);

	static void CloseCallback(uv_handle_t* handle);
	void DoClose(uv_handle_t* handle);

	static void ConnectCallback(uv_connect_t* req, int status);
	void DoConnect(uv_connect_t* req, int status);

	void SetCloseClientCB(const CloseClientCallback& cb);

private:
	uv_tcp_t connection_handle_;
	uv_work_t* work_request_;
	uv_stream_t* server_client_handle_;
	TcpCallback* callback_handle_;
	CloseClientCallback close_client_cb_;
};
#endif //UVCPP_TCP_CONNECTION_H_