////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_NETWORK_CORE_API_H_INCLUDED
#define VOSTOK_NETWORK_CORE_API_H_INCLUDED

#ifndef VOSTOK_NETWORK_CORE_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_NETWORK_CORE_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_NETWORK_CORE_BUILDING
#			define VOSTOK_NETWORK_CORE_API		VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_NETWORK_CORE_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_NETWORK_CORE_API	VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_NETWORK_CORE_API	VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_NETWORK_CORE_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_NETWORK_CORE_API

namespace vostok {
namespace network_core {

/*
typedef memory::doug_lea_allocator_type		allocator_type;

struct world;
struct engine;

VOSTOK_NETWORK_API	world*	create_world	( engine& engine, memory::base_allocator& orders_allocator );
VOSTOK_NETWORK_API	void	destroy_world	( world*& world );
VOSTOK_NETWORK_API	void	memory_allocator( allocator_type& allocator );
*/

} // namespace network_core
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_CORE_API_H_INCLUDED