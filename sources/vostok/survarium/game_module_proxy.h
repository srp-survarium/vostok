////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_MODULE_PROXY_H_INCLUDED
#define GAME_MODULE_PROXY_H_INCLUDED

#include <vostok/engine/engine_user.h>

namespace survarium {

class game_module_proxy : public vostok::engine_user::module_proxy {
private:
	virtual	vostok::engine_user::world*	create_world				(
											vostok::engine_user::engine& engine, 
											vostok::render::world& render_world, 
											vostok::sound::world& sound,
											vostok::network::world& network
										);
	virtual	void						destroy_world				( vostok::engine_user::world*& world );
	virtual	void						register_memory_allocators	( );
	virtual	vostok::memory::doug_lea_allocator_type& allocator		( );
}; // struct game_proxy

} // namespace survarium

#endif // #ifndef GAME_MODULE_PROXY_H_INCLUDED