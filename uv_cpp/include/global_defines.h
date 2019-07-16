#ifndef LIBUVCPP_GLOBAL_DEFINE_H_
#define LIBUVCPP_GLOBAL_DEFINE_H_

#define UVCPP_BEGIN namespace uvcpp { 
#define UVCPP_END }

#if (defined(_WIN32) || defined(__CYGWIN__))
#if defined(LIBUV_CPP_EXPORTS)
#define LIB_UVCPP_API __declspec(dllexport)
#else
#define LIB_UVCPP_API //__declspec(dllimport)
#endif
#endif


#endif	//LIBUVCPP_GLOBAL_DEFINE_H_