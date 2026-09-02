// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef COLLISION_USER_H_INCLUDED
#define COLLISION_USER_H_INCLUDED

#include <vostok/game_core/usable_object_user_data.h>

namespace survarium {

inline usable_object_user_data::usable_object_user_data( ) :
		owner								( 0 ),
		current_object						( 0 ),
		start_using_time_ms					( 0 ),
		current_time_ms						( 0 ),
		current_progress					( u32(-1) ),
		booster_artcont_time_factor			( 1.0f ),
		booster_engineer_use_time_factor	( 1.0f ),
		next								( 0 )
{
}

class ladder;
class victory_item_core;
class victory_items_container_core;
class inventory_holder;

struct base_player;

class collision_user : private boost::noncopyable {
public:
	virtual								~collision_user				( ) { }

	virtual	float4x4 const&				get_transform				( ) const = 0;
	virtual	void						use_ladder					( ladder* arg_0 ) = 0;

	virtual	void						use_victory_item			( victory_item_core* __formal )				{ }
	virtual	void						use_victory_items_container	( victory_items_container_core* __formal )	{ }

	inline	usable_object_user_data*	usable_object_user_data		( ) { return &m_usable_object_user_data; }

	virtual	inventory_holder*			cast_to_inventory_holder	( ) { return NULL; }
	virtual	inventory_holder const*		cast_to_inventory_holder	( ) const { return NULL; }

	virtual	base_player*				cast_to_base_player			( ) { return NULL; }
	virtual	base_player const*			cast_to_base_player			( ) const { return NULL; }

	inline	void						set_artcontainer_time_factor( float factor )	{ m_usable_object_user_data.booster_artcont_time_factor = factor; }
	inline	float						get_artcontainer_time_factor( ) const			{ return  m_usable_object_user_data.booster_artcont_time_factor; }

	inline	void						set_engineer_use_time_factor( float factor )	{ m_usable_object_user_data.booster_engineer_use_time_factor = factor; }
	inline	float						get_engineer_use_time_factor( ) const			{ return m_usable_object_user_data.booster_engineer_use_time_factor; }

protected:
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	survarium::usable_object_user_data		m_usable_object_user_data;
}; // class collision_user

STATIC_SIZE_ASSERT(collision_user, 0x24);

} // namespace survarium

#endif // #ifndef COLLISION_USER_H_INCLUDED
