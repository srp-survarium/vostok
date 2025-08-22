////////////////////////////////////////////////////////////////////////////
//	Created		: 22.11.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_API_H_INCLUDED
#define VOSTOK_AI_API_H_INCLUDED

#ifndef VOSTOK_AI_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_AI_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_AI_BUILDING
#			define VOSTOK_AI_API		VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_AI_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_AI_API	VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_AI_API	VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_AI_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_AI_API

namespace vostok {
namespace ai {

typedef memory::doug_lea_allocator_type	allocator_type;

struct world;
struct engine;

VOSTOK_AI_API	world*	create_world		( engine& engine );
VOSTOK_AI_API	void	destroy_world		( world*& world );
VOSTOK_AI_API	void	set_memory_allocator( allocator_type& allocator );

} // namespace ai
} // namespace vostok

#endif // #ifndef VOSTOK_AI_API_H_INCLUDED