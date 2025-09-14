////////////////////////////////////////////////////////////////////////////
//	Created		: 28.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MACRO_COMPILER_H_INCLUDED
#define VOSTOK_MACRO_COMPILER_H_INCLUDED

#include <vostok/macro_platform.h>

#if defined(_MSC_VER)
#	if _MSC_VER >= 1100
#		define VOSTOK_NOVTABLE									__declspec( novtable )
#	else // #if _MSC_VER >= 1100
#		define VOSTOK_NOVTABLE
#	endif // #if _MSC_VER >= 1100

#	define VOSTOK_ALIGN( alignment )								__declspec( align(alignment) )
#	define VOSTOK_DLL_IMPORT										__declspec( dllimport )
#	define VOSTOK_DLL_EXPORT										__declspec( dllexport )
//#	define VOSTOK_ASSUME( expression )							__assume( expression )
#	define VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( type )			virtual	~type	( ) = 0 { }
#	define VOSTOK_ALLOCA_IMPL										_alloca
#elif defined(__GNUC__) // #if defined(_MSC_VER)
#	define VOSTOK_NOVTABLE
#	define VOSTOK_ALIGN( alignment )								__attribute__( (aligned(alignment)) )
#	define VOSTOK_DLL_IMPORT										__attribute__( (dllimport) )
#	define VOSTOK_DLL_EXPORT										__attribute__( (dllexport) )
#	define VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( type )			virtual	~type	( ) {}

#	include <alloca.h>
#	define VOSTOK_ALLOCA_IMPL										alloca

#	define __FUNCSIG__											"<unknown function signature>"
#else // #elif defined(__GNUC__)
#	error please define macros for your compiler here
#endif // #if VOSTOK_PLATFORM_WINDOWS | VOSTOK_PLATFORM_XBOX_360

#endif // #ifndef VOSTOK_MACRO_COMPILER_H_INCLUDED