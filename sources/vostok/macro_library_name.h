////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MACRO_LIBRARY_NAME_H_INCLUDED
#define VOSTOK_MACRO_LIBRARY_NAME_H_INCLUDED

#include <vostok/macro_extensions.h>

#ifdef VOSTOK_LIBRARY_PREFIX
#	error please do not define VOSTOK_LIBRARY_PREFIX macro
#endif // #ifdef VOSTOK_LIBRARY_PREFIX

#ifdef _MSC_VER
#	define VOSTOK_LIBRARY_PREFIX		vostok_
#else // #ifdef _MSC_VER
#	define VOSTOK_LIBRARY_PREFIX		/../vostok_
#endif // #ifdef _MSC_VER

#ifdef VOSTOK_STATIC_LIBRARIES
#	ifdef MASTER_GOLD
#		define VOSTOK_STATIC_ID		-static-gold
#	else // #ifdef MASTER_GOLD
#		define VOSTOK_STATIC_ID		-static
#	endif // #ifdef MASTER_GOLD
#else // #ifdef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_STATIC_ID
#endif // #ifdef VOSTOK_STATIC_LIBRARIES

#ifdef DEBUG
#	define VOSTOK_DEBUG_LIBRARIES
#	define VOSTOK_CONFIGURATION_ID	-debug
#else // #ifdef DEBUG
#	define VOSTOK_CONFIGURATION_ID
#endif // #ifdef DEBUG

#ifdef _MSC_VER
#	define VOSTOK_LIBRARY_NAME(library, extension)	\
		VOSTOK_MAKE_STRING(\
			VOSTOK_STRING_CONCAT(\
				VOSTOK_LIBRARY_PREFIX,\
				VOSTOK_STRING_CONCAT(\
					library,\
					VOSTOK_STRING_CONCAT(\
						VOSTOK_STATIC_ID,\
						VOSTOK_STRING_CONCAT(\
							VOSTOK_CONFIGURATION_ID,\
							VOSTOK_STRING_CONCAT(\
								.,\
								extension\
							)\
						)\
					)\
				)\
			)\
		)
#else // #ifdef _MSC_VER
#	if defined(__SNC__)
#		define VOSTOK_COMPILER_ID	-snc
#	elif defined(__GCC__) // #if defined(__SNC__)
#		define VOSTOK_COMPILER_ID	-gcc
#	else // #elif defined(__GCC__)
#		error define your VOSTOK_COMPILER_ID here
#	endif // #if defined(__SNC__)

#	define VOSTOK_LIBRARY_NAME(library, extension)	\
		VOSTOK_MAKE_STRING(\
			VOSTOK_STRING_CONCAT(\
				VOSTOK_LIBRARY_PREFIX,\
				VOSTOK_STRING_CONCAT(\
					library,\
					VOSTOK_STRING_CONCAT(\
						VOSTOK_COMPILER_ID,\
						VOSTOK_STRING_CONCAT(\
							VOSTOK_STATIC_ID,\
							VOSTOK_CONFIGURATION_ID\
						)\
					)\
				)\
			)\
		)
#endif // #ifdef _MSC_VER

#endif // #ifndef VOSTOK_MACRO_LIBRARY_NAME_H_INCLUDED