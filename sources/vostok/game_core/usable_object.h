// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef USABLE_OBJECT_H_INCLUDED
#define USABLE_OBJECT_H_INCLUDED

#include <vostok/game_core/collision_geometry_subscriber.h>
#include <vostok/game_core/link_resolver.h>
#include <vostok/game_core/usable_object_user_data.h>

namespace vostok {
namespace physics {
	struct world;
}
}

namespace survarium {

class base_project;
class collision_geometry;

class usable_object : public collision_geometry_subscriber , public link_resolver {
public:
			explicit			usable_object				( );
	virtual						~usable_object				( );

	virtual	usable_object*		cast_to_usable				( ) override { return this; }

	virtual	void				load						( configs::binary_config_value const& cfg );
	virtual	void				resolve_links				( base_project* p, configs::binary_config_value cfg ) override;

	virtual	bool				use_initialize				( usable_object_user_data* user ) = 0;
	virtual	bool				use_execute					( usable_object_user_data* user ) = 0;
	virtual	bool				use_finalize				( usable_object_user_data* user ) = 0;
	virtual	pcstr				use_info					( usable_object_user_data* user ) = 0;

			float4x4			get_transform				( );

			void				insert						( physics::world* world );
			void				remove						( );

	typedef intrusive_list<
		usable_object_user_data,
		usable_object_user_data*,
		&usable_object_user_data::next,
		threading::single_threading_policy,
		size_policy,
		no_debug_policy
	> usable_object_users;

protected:
	/* 0x0000 */	/* collision_geometry_subscriber */
	/* 0x0004 */	/* link_resolver */
	/* 0x0008 */	usable_object_users			m_usable_object_users;
	/* 0x0018 */	collision_geometry**			m_collision_geometries;
	/* 0x001c */	u32								m_collision_geometries_count;
}; // class usable_object

STATIC_SIZE_ASSERT(usable_object, 0x20);

} // namespace survarium

#endif // #ifndef USABLE_OBJECT_H_INCLUDED
