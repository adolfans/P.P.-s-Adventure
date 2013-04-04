// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define _CRTDBG_MAP_ALLOC 
#include<stdlib.h> 
#include<crtdbg.h> 

//#include "debug_new.h" 

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <cstdlib>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <stdexcept>
using std::runtime_error;

// TODO: 在此处引用程序需要的其他头文件
#include "MyGameFunctions.h"