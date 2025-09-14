////////////////////////////////////////////////////////////////////////////
//	Created 	: 11.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_GAME_API_H_INCLUDED
#define VOSTOK_GAME_API_H_INCLUDED

#ifndef VOSTOK_GAME_API
#	define VOSTOK_GAME_API
#endif // #ifndef VOSTOK_GAME_API

namespace vostok {

namespace engine_user {
	struct engine;
	struct world;
} // namespace engine_user

namespace render {
namespace game {
	class renderer;
} // namespace game
} // namespace render

namespace sound {
	struct world;
} // namespace sound

namespace network {
	struct world;
} // namespace network

} // namespace vostok

namespace survarium {
namespace game_module {

VOSTOK_GAME_API	vostok::engine_user::world*	create_world				( 
												vostok::engine_user::engine& engine, 
												vostok::render::world& render_world, 
												vostok::sound::world& sound,
												vostok::network::world& network
											);

VOSTOK_GAME_API	void						destroy_world				( vostok::engine_user::world*& world );
VOSTOK_GAME_API	void						register_memory_allocators	( );
VOSTOK_GAME_API	vostok::memory::doug_lea_allocator_type&	allocator		( );

} // namespace game_module
} // namespace survarium

#endif // #ifndef VOSTOK_GAME_API_H_INCLUDED