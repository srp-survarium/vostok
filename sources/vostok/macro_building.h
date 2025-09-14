////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MACRO_BUILDING_H_INCLUDED
#define VOSTOK_MACRO_BUILDING_H_INCLUDED

#ifndef VOSTOK_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_CORE_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		if VOSTOK_CORE_BUILDING
#			define VOSTOK_CORE_API				VOSTOK_DLL_EXPORT
#		else // #if VOSTOK_CORE_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_CORE_API			VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_CORE_API			VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #if VOSTOK_CORE_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_API

#endif // #ifndef VOSTOK_MACRO_BUILDING_H_INCLUDED