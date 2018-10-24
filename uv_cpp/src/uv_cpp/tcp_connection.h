#ifndef UVCPP_TCP_CONNECTION_H_
#define UVCPP_TCP_CONNECTION_H_

#include <memory>
#include <functional>
#include "uv.h"
#include "global_defines.h"

UVCPP_BEGIN

class TcpCallback;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void(const TcpConnectionPtr&)> CloseClientCallback;

class LIB_UVCPP_API TcpConnection :public std::enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(TcpCallback* callback_handle);
	~TcpConnection();

	int ReadStart(uv_stream_t* server);
	int Accept(uv_stream_t* server, int status);
	void Connect(uv_connect_t& req,const struct sockaddr* addr, BOOL reconnect_flag = FALSE);
	void Close(void);
	int Send(const char* buf, unsigned int nbufs);
	void SetCloseClientCB(const CloseClientCallback& cb);

private:
	static void AfterWriteCB(uv_write_t* req, int status);
	void DoAfterWrite(uv_write_t* req, int status);

	static void AllocReadBufferCallback(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
	void DoAllocReadBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

	static void ReadCallback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	void DoRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);

	void CloseSocket(uv_handle_t* handle, uv_close_cb close_cb);
	static void CloseCallback(uv_handle_t* handle);
	void DoClose(uv_handle_t* handle);

	static void AfterConnectCB(uv_connect_t* req, int status);
	void DoAfterConnect(uv_connect_t* req, int status);

	BOOL StartReconnTimer(void);
	BOOL StopReconnTimer(void);
	static void AfterTimerReconnCB(uv_timer_t* handle);
	void DoTimerReconnCB(uv_timer_t* handle);

private:
	uv_tcp_t connection_handle_;
	uv_work_t* work_request_;
	uv_stream_t* server_client_handle_;
	TcpCallback* callback_handle_;
	CloseClientCallback close_client_cb_;
	uv_write_t write_req_;
	uv_connect_t* conn_req_;
	struct sockaddr* socket_addr_;
	uv_timer_t timer_handle_;
	UINT64 timer_timeout_;
	UINT64 timer_repeat_;
	BOOL be_connected_;
	BOOL reconnect_flag_;
	BOOL is_reconnecting_;
};

UVCPP_END
#endif //UVCPP_TCP_CONNECTION_H_