#ifndef UVCPP_GLOBAL_DEFINE_H_
#define UVCPP_GLOBAL_DEFINE_H_

#define UVCPP_BEGIN namespace uvcpp { 
#define UVCPP_END }


#if (defined(_WIN32) || defined(__CYGWIN__))
#if defined(LIB_UVCPP_EXPORTS)
#define LIB_UVCPP_API __declspec(dllexport)
#else
#define LIB_UVCPP_API //__declspec(dllimport)
#endif
#endif

#ifndef LIB_UVCPP_API
#define LIB_UVCPP_API
#endif

#endif	//UVCPP_GLOBAL_DEFINE_H_