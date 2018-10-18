#ifndef UVCPP_TCP_CALLBACK_H_
#define UVCPP_TCP_CALLBACK_H_

class TcpCallback
{
public:
	TcpCallback(){};
	virtual ~TcpCallback(){};

	virtual void OnConnect(int status){};
	virtual void OnClose(){};
	virtual void OnRead(const char* buf,int n_read){};
	virtual void OnAccept(int status){};
	virtual void OnAllocBuffer(int& suggested_size, char **buf)
	{
		static char default_buffer[1024];
		suggested_size = 1024;
		*buf = default_buffer;
	};

};


#endif	//UVCPP_TCP_CALLBACK_H_
