////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

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
	bt_character_controller						( world* w );
	~bt_character_controller					( );

	void					initialize			( );
	void					activate			( float4x4 const& t );
	void					update_action		( const u32 time_delta_in_ms );

	void					deactivate			( );

	memory::base_allocator& allocator			( );

	float4x4				get_transform		( );
	void					set_transform		( float4x4 const& transform );

	void					set_walk_direction	( float3 const& direction );

	bool					can_jump			( ) const;
	bool					on_ground			( ) const;
	void					jump				( );
	void					end_jump			( );
	bool					has_updates			( ) const;

			bool			adjust_foot_transform	(
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

	void					set_crouch			( bool crouch );
	// @todo Recover the implementation when a caller is restored.
	bool					can_prone			( ) const /* no source */;
	bool					can_crouch			( ) const;
	bool					can_stand			( ) const;

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
