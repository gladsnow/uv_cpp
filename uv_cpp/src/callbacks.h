#ifndef UVCPP_CALLBACKS_H_
#define UVCPP_CALLBACKS_H_

#include <functional>
#include "tcp_connection.h"

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void(const TcpConnectionPtr&)> CloseClientCallback;

#endif	//UVCPP_CALLBACK_H_