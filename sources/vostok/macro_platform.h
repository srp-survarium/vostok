////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MACRO_PLATFORM_H_INCLUDED
#define VOSTOK_MACRO_PLATFORM_H_INCLUDED

////////////////////////////////////////////////////////////////////////////
// VOSTOK_PLATFORM_WINDOWS_32
////////////////////////////////////////////////////////////////////////////
#ifdef VOSTOK_PLATFORM_WINDOWS_32
#	error do not define VOSTOK_PLATFORM_WINDOWS_32 macro
#endif // #ifdef VOSTOK_PLATFORM_WINDOWS_32

#ifdef WIN32
#	define VOSTOK_PLATFORM_WINDOWS_32	1
#else // #ifdef WIN32
#	define VOSTOK_PLATFORM_WINDOWS_32	0
#endif // #ifdef WIN32

////////////////////////////////////////////////////////////////////////////
// VOSTOK_PLATFORM_WINDOWS_64
////////////////////////////////////////////////////////////////////////////
#ifdef VOSTOK_PLATFORM_WINDOWS_64
#	error do not define VOSTOK_PLATFORM_WINDOWS_64 macro
#endif // #ifdef VOSTOK_PLATFORM_WINDOWS_64

#ifdef WIN64
#	define VOSTOK_PLATFORM_WINDOWS_64	1
#else // #ifdef WIN32
#	define VOSTOK_PLATFORM_WINDOWS_64	0
#endif // #ifdef WIN32

////////////////////////////////////////////////////////////////////////////
// VOSTOK_PLATFORM_WINDOWS
////////////////////////////////////////////////////////////////////////////
#ifdef VOSTOK_PLATFORM_WINDOWS
#	error do not define VOSTOK_PLATFORM_WINDOWS macro
#endif // #ifdef VOSTOK_PLATFORM_WINDOWS

#if VOSTOK_PLATFORM_WINDOWS_32 | VOSTOK_PLATFORM_WINDOWS_64
#	define VOSTOK_PLATFORM_WINDOWS	1
#else // #if VOSTOK_PLATFORM_WINDOWS_32 | VOSTOK_PLATFORM_WINDOWS_64
#	define VOSTOK_PLATFORM_WINDOWS	0
#endif // #if VOSTOK_PLATFORM_WINDOWS_32 | VOSTOK_PLATFORM_WINDOWS_64

////////////////////////////////////////////////////////////////////////////
// VOSTOK_PLATFORM_XBOX_360
////////////////////////////////////////////////////////////////////////////
#ifdef VOSTOK_PLATFORM_XBOX_360
#	error do not define VOSTOK_PLATFORM_XBOX_360 macro
#endif // #ifdef VOSTOK_PLATFORM_XBOX_360

#ifdef _XBOX
#	if _XBOX_VER < 200 
#		error engine doesn't support XBox versions earlier to XBox 360
#	endif // #if _XBOX_VER < 200 
#	define VOSTOK_PLATFORM_XBOX_360	1
#	define VOSTOK_STATIC_LIBRARIES	1
#else // #ifdef _XBOX
#	define VOSTOK_PLATFORM_XBOX_360	0
#endif // #ifdef _XBOX

////////////////////////////////////////////////////////////////////////////
// VOSTOK_PLATFORM_PS3
////////////////////////////////////////////////////////////////////////////
#ifdef VOSTOK_PLATFORM_PS3
#	error do not define VOSTOK_PLATFORM_PS3 macro
#endif // #ifdef VOSTOK_PLATFORM_PS3

#ifdef SN_TARGET_PS3
#	define VOSTOK_PLATFORM_PS3		1
#else // #ifdef SN_TARGET_PS3
#	define VOSTOK_PLATFORM_PS3		0
#endif // #ifdef SN_TARGET_PS3

////////////////////////////////////////////////////////////////////////////
// VOSTOK_LITTLE_ENDIAN / VOSTOK_BIG_ENDIAN
////////////////////////////////////////////////////////////////////////////
#ifdef VOSTOK_LITTLE_ENDIAN
#	error do not define VOSTOK_LITTLE_ENDIAN macro
#endif // #ifdef VOSTOK_LITTLE_ENDIAN

#ifdef VOSTOK_BIG_ENDIAN
#	error do not define VOSTOK_BIG_ENDIAN macro
#endif // #ifdef VOSTOK_BIG_ENDIAN

#if VOSTOK_PLATFORM_WINDOWS 
#	define	VOSTOK_LITTLE_ENDIAN		1
#	define	VOSTOK_BIG_ENDIAN			0
#elif VOSTOK_PLATFORM_XBOX_360		// #if VOSTOK_PLATFORM_WINDOWS 
#	define	VOSTOK_LITTLE_ENDIAN		0
#	define	VOSTOK_BIG_ENDIAN			1
#elif VOSTOK_PLATFORM_PS3				// #elif VOSTOK_PLATFORM_XBOX_360
#	define	VOSTOK_LITTLE_ENDIAN		0
#	define	VOSTOK_BIG_ENDIAN			1
#endif								// #elif VOSTOK_PLATFORM_PS3

#if VOSTOK_LITTLE_ENDIAN & VOSTOK_BIG_ENDIAN
#	error invalid platform capabilities specified
#endif // #if VOSTOK_LITTLE_ENDIAN & VOSTOK_BIG_ENDIAN

////////////////////////////////////////////////////////////////////////////
// VOSTOK_PLATFORM_32_BIT / VOSTOK_PLATFORM_64_BIT
////////////////////////////////////////////////////////////////////////////
#ifdef VOSTOK_PLATFORM_32_BIT
#	error do not define VOSTOK_PLATFORM_32_BIT macro
#endif // #ifdef VOSTOK_LITTLE_ENDIAN

#ifdef VOSTOK_PLATFORM_64_BIT
#	error do not define VOSTOK_PLATFORM_64_BIT macro
#endif // #ifdef VOSTOK_BIG_ENDIAN

#if VOSTOK_PLATFORM_WINDOWS_64
#	define	VOSTOK_PLATFORM_32_BIT	0
#	define	VOSTOK_PLATFORM_64_BIT	1
#elif VOSTOK_PLATFORM_WINDOWS_32		// #if VOSTOK_PLATFORM_WINDOWS_64
#	define	VOSTOK_PLATFORM_32_BIT	1
#	define	VOSTOK_PLATFORM_64_BIT	0
#elif VOSTOK_PLATFORM_XBOX_360		// #if VOSTOK_PLATFORM_WINDOWS 
#	define	VOSTOK_PLATFORM_32_BIT	1
#	define	VOSTOK_PLATFORM_64_BIT	0
#elif VOSTOK_PLATFORM_PS3				// #elif VOSTOK_PLATFORM_XBOX_360
#	define	VOSTOK_PLATFORM_32_BIT	1
#	define	VOSTOK_PLATFORM_64_BIT	0
#endif								// #elif VOSTOK_PLATFORM_PS3

#if VOSTOK_PLATFORM_32_BIT & VOSTOK_PLATFORM_64_BIT
#	error invalid platform capabilities specified
#endif // #if VOSTOK_LITTLE_ENDIAN & VOSTOK_BIG_ENDIAN

////////////////////////////////////////////////////////////////////////////
// check macros
////////////////////////////////////////////////////////////////////////////
#if VOSTOK_PLATFORM_WINDOWS_32
#	if VOSTOK_PLATFORM_WINDOWS_64 | VOSTOK_PLATFORM_XBOX_360 | VOSTOK_PLATFORM_PS3
#		error you cannot specify several target platforms
#	endif // #if VOSTOK_PLATFORM_WINDOWS_64 | VOSTOK_PLATFORM_XBOX_360 | VOSTOK_PLATFORM_PS3
#elif VOSTOK_PLATFORM_WINDOWS_64		// #if VOSTOK_PLATFORM_WINDOWS_32
#	if VOSTOK_PLATFORM_WINDOWS_32 | VOSTOK_PLATFORM_XBOX_360 | VOSTOK_PLATFORM_PS3
#		error you cannot specify several target platforms
#	endif // #if VOSTOK_PLATFORM_WINDOWS_32 | VOSTOK_PLATFORM_XBOX_360 | VOSTOK_PLATFORM_PS3
#elif VOSTOK_PLATFORM_XBOX_360			// #elif VOSTOK_PLATFORM_WINDOWS_64
#	if VOSTOK_PLATFORM_WINDOWS_32 | VOSTOK_PLATFORM_WINDOWS_64 | VOSTOK_PLATFORM_PS3
#		error you cannot specify several target platforms
#	endif // #if VOSTOK_PLATFORM_WINDOWS_32 | VOSTOK_PLATFORM_WINDOWS_64 | VOSTOK_PLATFORM_PS3
#elif VOSTOK_PLATFORM_PS3				// #elif VOSTOK_PLATFORM_XBOX_360
#	if VOSTOK_PLATFORM_WINDOWS_32 | VOSTOK_PLATFORM_XBOX_360 | VOSTOK_PLATFORM_XBOX_360
#		error you cannot specify several target platforms
#	endif // #if VOSTOK_PLATFORM_WINDOWS_32 | VOSTOK_PLATFORM_XBOX_360 | VOSTOK_PLATFORM_XBOX_360
#else								// #elif VOSTOK_PLATFORM_PS3
#		error unknown target platform!
#endif // #if VOSTOK_PLATFORM_WINDOWS_32

#endif // #ifndef VOSTOK_MACRO_PLATFORM_H_INCLUDED