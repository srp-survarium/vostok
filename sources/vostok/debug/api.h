////////////////////////////////////////////////////////////////////////////
//	Created		: 15.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_DEBUG_API_H_INCLUDED
#define VOSTOK_DEBUG_API_H_INCLUDED

#ifndef VOSTOK_DEBUG_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_DEBUG_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_DEBUG_BUILDING
#			define VOSTOK_DEBUG_API		VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_DEBUG_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_DEBUG_API	VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_DEBUG_API	VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_DEBUG_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_DEBUG_API

#endif // #ifndef VOSTOK_DEBUG_API_H_INCLUDED