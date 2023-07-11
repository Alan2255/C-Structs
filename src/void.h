#ifndef __VOID_H__
#define __VOID_H__

typedef void *(*FunctionCopy)(void*);
typedef void (*FunctionDestroy)(void*);
typedef void (*FunctionVisit)(void*);
typedef void (*FunctionVisitExtra)(void*, void*);
typedef int (*FunctionCompare)(void*, void*);
typedef unsigned (*FunctionHash)(void*);

#endif
