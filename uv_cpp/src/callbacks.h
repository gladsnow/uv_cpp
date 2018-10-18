#ifndef UVCPP_CALLBACKS_H_
#define UVCPP_CALLBACKS_H_

#include <functional>

typedef std::function<void (void)> ConnectionCallback;
typedef std::function<void (int)> CloseConnCallback;


#endif	//UVCPP_CALLBACK_H_