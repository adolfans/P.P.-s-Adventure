#pragma once
#include <DxErr.h>
#include <cstdio>
#include <string.h>
#ifdef _DEBUG
#include <iostream>
#define MyGameMessage(p) {std::cout<<p<<std::endl;}
#endif
#ifndef _DEBUG
#define MyGameMessage(p) {}
#endif

template <typename T>
inline void IRelease(T& IObject)
{
	if( IObject != 0 )
	{
		IObject->Release();
		IObject = 0;
	}
}

#if	defined(DEBUG) | defined(_DEBUG)
       #ifndef HR
       #define HR(x)                                          \
       {                                                      \
             HRESULT hr = x;                                  \
             if(FAILED(hr))                                   \
             {                                                \
			 DXTraceW(__FILE__, __LINE__, hr, NULL, TRUE); \
             }                                                \
       }
       #endif

#else
      #ifndef HR
      #define HR(x) x;
      #endif
#endif

//DXTraceW(__FILE__, __LINE__, hr, L#x, TRUE);