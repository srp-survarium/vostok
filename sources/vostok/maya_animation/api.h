////////////////////////////////////////////////////////////////////////////
//	Created 	: 16.01.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_DEFAULT_API_H_INCLUDED
#define VOSTOK_DEFAULT_API_H_INCLUDED

#ifndef VOSTOK_DEFAULT_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_MAYA_ANIMATION_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_MAYA_ANIMATION_BUILDING
#			define VOSTOK_MAYA_ANIMATION_API			VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_MAYA_ANIMATION_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_MAYA_ANIMATION_API		VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_MAYA_ANIMATION_API		VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_MAYA_ANIMATION_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_DEFAULT_API

namespace vostok {
namespace maya_animation {

struct engine;
struct world;

typedef memory::doug_lea_allocator_type				allocator_type;

VOSTOK_MAYA_ANIMATION_API	world*	create_world		( engine& engine );
VOSTOK_MAYA_ANIMATION_API	void	destroy_world		( world*& world );
VOSTOK_MAYA_ANIMATION_API	void	set_memory_allocator( allocator_type& allocator );

} // namespace maya_animation
} // namespace vostok

#endif // #ifndef VOSTOK_DEFAULT_API_H_INCLUDED