// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HIT_RECEIVER_H_INCLUDED
#define HIT_RECEIVER_H_INCLUDED

#include <vostok/collision/game_object.h>
#include <vostok/loose_ptr_base.h>
#include <vostok/game_core/damage_zone_core.h>

namespace vostok {
namespace collision {
	class bone_collision_data; // sushi@TODO: Might be better to include API?
}
namespace physics {
	class base_physics_object;
}
}

namespace survarium {

class bullet;
struct hit_initiator;
class player_actions_subscriber;

struct hit_receiver : public collision::game_object , public loose_ptr_base {
public:
	virtual						~hit_receiver				( ) {}

	virtual	void				hit							(
									hit_initiator const* const	initiator,
									u32	const					bone_index,
									pcstr						damage_type,
									float const					amount,
									float const					armor_piercing,
									bullet* const				bullet
								) = 0;

	virtual	void				hit							(
									hit_initiator const* const				initiator,
									collision::bone_collision_data const&	bone_data,
									pcstr									damage_type,
									float const								amount,
									float const								armor_piercing,
									bullet*	const							bullet
								) = 0;

	virtual	float				get_speed					( ) const = 0;

	virtual	hit_receiver*		cast_to_hit_receiver		( ) override { return this; }

	virtual	void				subscribe_on_actions		( player_actions_subscriber* arg_0 ) { /* no source */ } // sushi@TODO
	virtual	void				unsubscribe_from_actions	( player_actions_subscriber* arg_0 ) { /* no source */ } // sushi@TODO

	inline	void				on_enter_damage_zone		( damage_zone_core_ptr const& arg_0 ) { /* no source */ }
	inline	void				on_leave_damage_zone		( damage_zone_core_ptr const& arg_0 ) { /* no source */ }
}; // struct hit_receiver

STATIC_SIZE_ASSERT(hit_receiver, 0x8);


// sushi@NOTE: Definitions damage_zone_core.cpp
struct hit_receiver_info {
				hit_receiver_info	( hit_receiver* receiver, physics::base_physics_object* rigid_body );

		bool	operator==			( hit_receiver_info const& rhs ) const;
		bool	operator==			( hit_receiver const* const rhs ) const { /* no source */ }

public:
	/* 0x0000 */	hit_receiver*						m_receiver;
	/* 0x0004 */	physics::base_physics_object*		m_rigid_body;
	/* 0x0008 */	bool								m_was_hit;
}; // struct hit_receiver_info

struct erase_null_ptrs : private boost::noncopyable {
	inline bool operator()( hit_receiver_info const& info ) const
	{
		return info.m_receiver == NULL;
	}
};

STATIC_SIZE_ASSERT(erase_null_ptrs, 0x1);

struct erase_old_receivers : public boost::noncopyable {
	inline explicit erase_old_receivers( damage_zone_core_ptr const& damage_zone ) :
		m_damage_zone( damage_zone )
	{
	}

	inline bool operator()( hit_receiver_info const& info ) const;

private:
	/* 0x0000 */ damage_zone_core_ptr const&	m_damage_zone;
};

STATIC_SIZE_ASSERT(erase_old_receivers, 0x4);

struct remove_left_receivers_predicate {
	inline explicit remove_left_receivers_predicate( buffer_vector< hit_receiver_info > const& receivers ) :
		m_receivers( &receivers )
	{
	}

	inline bool operator()( hit_receiver_info const& info ) const;

private:
	/* 0x0000 */ buffer_vector< hit_receiver_info > const*	m_receivers;
};

STATIC_SIZE_ASSERT(remove_left_receivers_predicate, 0x4);


} // namespace survarium

#endif // #ifndef HIT_RECEIVER_H_INCLUDED
