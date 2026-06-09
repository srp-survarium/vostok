////////////////////////////////////////////////////////////////////////////
//	Created		: 07.06.2026
//	Copyright (C) GSC Game World
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_GAME_CORE_API_H_INCLUDED
#define VOSTOK_GAME_CORE_API_H_INCLUDED

#ifndef VOSTOK_GAME_CORE_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_GAME_CORE_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_GAME_CORE_BUILDING
#			define VOSTOK_GAME_CORE_API		VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_GAME_CORE_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_GAME_CORE_API	VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_GAME_CORE_API	VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_GAME_CORE_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_GAME_CORE_API

#endif // #ifndef VOSTOK_GAME_CORE_API_H_INCLUDED
