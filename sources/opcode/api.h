////////////////////////////////////////////////////////////////////////////
//	Created 	: 04.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef OPCODE_API_H_INCLUDED
#define OPCODE_API_H_INCLUDED

#ifndef ICE_NO_DLL
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define	ICE_NO_DLL
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#else // #ifndef ICE_NO_DLL
#	ifdef VOSTOK_STATIC_LIBRARIES
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef ICE_NO_DLL

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#include <boost/noncopyable.hpp>
#include <vostok/macro_extensions.h>
#include <vostok/optimization_extensions.h>
#include <vostok/type_extensions.h>
#include <vostok/debug/extensions.h>
#include <vostok/detail_noncopyable.h>
#include <vostok/memory_extensions.h>

#undef BOOL
#undef ASSERT
#include "opcode.h"

#endif // #ifndef OPCODE_API_H_INCLUDED