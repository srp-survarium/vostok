// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HITTABLE_OBJECT_H_INCLUDED
#define HITTABLE_OBJECT_H_INCLUDED

namespace vostok {
namespace physics {
	struct world;
	class bt_static_rigid_body;
}
}

#include <vostok/game_core/hit_receiver.h>

namespace survarium {

class hittable_object : public hit_receiver {
public:
	explicit			hittable_object				( );
	virtual				~hittable_object			( );

			void		load						( configs::binary_config_value const& cfg_val );

			void		set_transform				( float4x4 const& transform );
			float4x4	get_transform				( );

			void		insert						( physics::world* world );
			void		remove						( );

private:
	/* 0x0000 */	/* hit_receiver */
	/* 0x0008 */	physics::bt_static_rigid_body*		m_rigid_body;
	/* 0x000c */	physics::world*						m_physics_world;
	/* 0x0010 */	u16									m_group;
	/* 0x0012 */	u16									m_mask;
}; // class hittable_object

STATIC_SIZE_ASSERT(hittable_object, 0x14);

} // namespace survarium

#endif // #ifndef HITTABLE_OBJECT_H_INCLUDED
