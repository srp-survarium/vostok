////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.09.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_OS_EXTENSIONS_H_INCLUDED
#define VOSTOK_OS_EXTENSIONS_H_INCLUDED

#include <vostok/macro_platform.h>
#include <vostok/type_extensions.h>

#if VOSTOK_PLATFORM_WINDOWS || VOSTOK_PLATFORM_XBOX_360
#	define LPVOID					pvoid
#	define HANDLE					pvoid
#	define INVALID_HANDLE_VALUE		((HANDLE)((u64)(-1)))

#	define BOOL						int
#	define FALSE					0
#	define TRUE						1

#	define DLL_PROCESS_DETACH		0
#	define DLL_PROCESS_ATTACH		1

#	define APIENTRY					__stdcall

#	define HMODULE					pvoid

	struct HWND__;
#	define	HWND					HWND__*

#endif // #if VOSTOK_PLATFORM_WINDOWS || VOSTOK_PLATFORM_XBOX_360

#include <vostok/os_functions.h>

#endif // #ifndef VOSTOK_OS_EXTENSIONS_H_INCLUDED