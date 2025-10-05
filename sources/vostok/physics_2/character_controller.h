////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_2_CHARACTER_CONTROLLER_H_INCLUDED
#define VOSTOK_PHYSICS_2_CHARACTER_CONTROLLER_H_INCLUDED

namespace vostok {
namespace physics_2 {

class bullet_physics_world;
class bullet_character_controller;

class bt_character_controller  {
public:
	bt_character_controller						( world* w );
	~bt_character_controller					( );

	void					initialize			( );
	void					activate			( float4x4 const& t );
	void					update_action		( u32 time_delta_in_ms );

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

	bool					adjust_foot_transform(
		float3 const&                      half_size,
		float3 const&                      start,
		float3 const&                      finish,
		float3&                            __unknown_1,
		float3&                            __unknown_2) const /* no source */;

	bool					adjust_foot_transform(
		float3 const&                      half_size,
		float3 const&                      start,
		float3 const&                      finish,
		float                              rotation_koef0,
		float4x4&                          transform);

	void					set_crouch			( bool crouch );
	bool					can_prone			( ) const /* no source */;
	bool					can_crouch			( ) const;
	bool					can_stand			( ) const;

private:
	/* offset 0x0000 */ bullet_character_controller*        m_bt_controller;
	/* offset 0x0004 */ bullet_physics_world*               m_bt_physics_world;
	/* offset 0x0008 */ bool                                m_active;
}; // class bt_character_controller

namespace {
	typedef char size_assert[
		sizeof(bt_character_controller) == 0xC ? 1 : -1
	];
}

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_2_CHARACTER_CONTROLLER_H_INCLUDED