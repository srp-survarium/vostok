// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_PHYSICS_CHARACTER_CONTROLLER_H_INCLUDED
#define VOSTOK_PHYSICS_CHARACTER_CONTROLLER_H_INCLUDED

#include <vostok/physics/api.h>

namespace vostok {
namespace physics {

class bullet_physics_world;
class bullet_character_controller;
struct world;

class bt_character_controller  {
public:
	VOSTOK_PHYSICS_API bt_character_controller		( world* w );
	VOSTOK_PHYSICS_API ~bt_character_controller		( );

	VOSTOK_PHYSICS_API void	initialize			( );
	VOSTOK_PHYSICS_API void	activate			( float4x4 const& t );
	VOSTOK_PHYSICS_API void	update_action		( const u32 time_delta_in_ms );

	VOSTOK_PHYSICS_API void	deactivate			( );

	VOSTOK_PHYSICS_API memory::base_allocator& allocator( );

	VOSTOK_PHYSICS_API float4x4 get_transform		( );
	VOSTOK_PHYSICS_API void	set_transform		( float4x4 const& transform );

	VOSTOK_PHYSICS_API void	set_walk_direction	( float3 const& direction );

	VOSTOK_PHYSICS_API bool	can_jump			( ) const;
	VOSTOK_PHYSICS_API bool	on_ground			( ) const;
	VOSTOK_PHYSICS_API void	jump				( );
	VOSTOK_PHYSICS_API void	end_jump			( );
	VOSTOK_PHYSICS_API bool	has_updates			( ) const;

	VOSTOK_PHYSICS_API bool	adjust_foot_transform	(
								float3 const&		half_size,
								float3 const&		start,
								float3 const&		finish,
								float				rotation_koef0,
								float				__formal,
								float4x4&			transform
							);

	// @todo Recover this overload if a caller requiring it is restored.
	inline	bool			adjust_foot_transform	(
								float3 const&		arg_0,
								float3 const&		arg_1,
								float3 const&		arg_2,
								float3&				arg_3,
								float3&				arg_4
							) const { /* no source */ }

	VOSTOK_PHYSICS_API void	set_crouch			( bool crouch );
	// @todo Recover the implementation when a caller is restored.
	bool					can_prone			( ) const /* no source */;
	VOSTOK_PHYSICS_API bool	can_crouch			( ) const;
	VOSTOK_PHYSICS_API bool	can_stand			( ) const;

private:
	/* 0x0000 */	bullet_character_controller*	m_bt_controller;
	/* 0x0004 */	bullet_physics_world*			m_bt_physics_world;
	/* 0x0008 */	bool							m_active;
}; // class bt_character_controller

STATIC_SIZE_ASSERT(bt_character_controller, 0xC);

VOSTOK_PHYSICS_API bt_character_controller* create_character_controller( vostok::memory::base_allocator& allocator, world* w );

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_CHARACTER_CONTROLLER_H_INCLUDED
