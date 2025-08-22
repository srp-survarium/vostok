////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_NETWORK_API_H_INCLUDED
#define VOSTOK_NETWORK_API_H_INCLUDED

#ifndef VOSTOK_NETWORK_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_NETWORK_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_NETWORK_BUILDING
#			define VOSTOK_NETWORK_API				VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_NETWORK_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_NETWORK_API			VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_NETWORK_API			VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_NETWORK_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_NETWORK_API

namespace vostok {
namespace network {

struct engine;
struct world;

typedef memory::doug_lea_allocator_type			allocator_type;

VOSTOK_NETWORK_API	world*	create_world		( engine& engine );
VOSTOK_NETWORK_API	void	destroy_world		( world*& world );
VOSTOK_NETWORK_API	void	set_memory_allocator( allocator_type& allocator );
VOSTOK_NETWORK_API	void	dispatch_callbacks	( );

} // namespace network
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_API_H_INCLUDED