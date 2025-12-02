////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef HIT_RECEIVER_H_INCLUDED
#define HIT_RECEIVER_H_INCLUDED

#include <vostok/collision/game_object.h>
#include <vostok/loose_ptr_base.h>

namespace survarium {

class bullet;

struct hit_receiver : public collision::game_object , public loose_ptr_base {
public:
	virtual						~hit_receiver				( ) { /* no source */ }

	virtual	void				hit							(
									hit_initiator const*					arg_0,
									collision::bone_collision_data const&	arg_1,
									pcstr									arg_2,
									float									arg_3,
									float									arg_4,
									bullet*									arg_5
								) = 0;

	virtual	void				hit							(
									hit_initiator const*	arg_0,
									u32						arg_1,
									pcstr					arg_2,
									float					arg_3,
									float					arg_4,
									bullet*					arg_5
								) = 0;

	virtual	float				get_speed					( ) const = 0;

	virtual	hit_receiver*		cast_to_hit_receiver		( ) override;

	virtual	void				subscribe_on_actions		( player_actions_subscriber* arg_0 ) { /* no source */ }
	virtual	void				unsubscribe_from_actions	( player_actions_subscriber* arg_0 ) { /* no source */ }

	inline	void				on_enter_damage_zone		( resources::resource_ptr<damage_zone_core,resources::unmanaged_intrusive_base> const& arg_0 ) { /* no source */ }
	inline	void				on_leave_damage_zone		( resources::resource_ptr<damage_zone_core,resources::unmanaged_intrusive_base> const& arg_0 ) { /* no source */ }

public:
	/* 0x0000 */	/* collision::game_object */
	/* 0x0004 */	/* loose_ptr_base */
}; // struct hit_receiver

STATIC_SIZE_ASSERT(hit_receiver, 0x8);


// sushi@TODO: Definitions damage_zone_core.cpp
struct hit_receiver_info {
				hit_receiver_info	( hit_receiver* receiver, physics::base_physics_object* rigid_body );

		bool	operator==			( hit_receiver const* rhs ) const { /* no source */ }
		bool	operator==			( hit_receiver_info const& rhs ) const;

public:
	/* 0x0000 */	hit_receiver*						m_receiver;
	/* 0x0004 */	physics::base_physics_object*		m_rigid_body;
	/* 0x0008 */	bool								m_was_hit;
}; // struct hit_receiver_info


} // namespace survarium

#endif // #ifndef HIT_RECEIVER_H_INCLUDED
