/*******************************************************************
 * File automatically generated by rebuild_wrappers.py (v2.0.0.10) *
 *******************************************************************/
#ifndef __wrappedlibvkd3dTYPES_H_
#define __wrappedlibvkd3dTYPES_H_

#ifndef LIBNAME
#error You should only #include this file inside a wrapped*.c file
#endif
#ifndef ADDED_FUNCTIONS
#define ADDED_FUNCTIONS() 
#endif

typedef int32_t (*iFpp_t)(void*, void*);

#define SUPER() ADDED_FUNCTIONS() \
	GO(vkd3d_create_instance, iFpp_t)

#endif // __wrappedlibvkd3dTYPES_H_
