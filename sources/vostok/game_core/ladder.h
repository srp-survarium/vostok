// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef LADDER_H_INCLUDED
#define LADDER_H_INCLUDED

#include <vostok/game_core/usable_object.h>
#include <vostok/game_core/landing_point.h>
#include <vostok/math_plane.h>

namespace vostok {
namespace physics {
	struct world;
}
}

namespace survarium {

class base_project;

class ladder : public resources::unmanaged_resource , public usable_object {
public:
	typedef usable_object super;

												ladder					( resources::managed_resource_ptr const& main_animation, math::plane const& p );
	virtual										~ladder					( );

	virtual	bool								use_initialize			( usable_object_user_data* user ) override;
	virtual	bool								use_execute				( usable_object_user_data* user ) override;
	virtual	bool								use_finalize			( usable_object_user_data* user ) override;
	virtual	pcstr								use_info				( usable_object_user_data* user ) override { return "st_use_ladder"; }

	virtual	void								load					( configs::binary_config_value const& cfg_val ) override;
	virtual	void								resolve_links			( base_project* p, configs::binary_config_value cfg ) override;

	virtual	void								activate				( physics::world* world );
	virtual	void								deactivate				( );

			void								add_landing_point		( landing_point* const new_point );
			landing_point*						pop_landing_point		( );

	inline	landing_point const*				select_landing_point	( float3 const& arg_0, const bool arg_1 ) const { /* no source */ }
	inline	landing_point const*				select_attachment_point	( float3 const& arg_0 ) const { /* no source */ }

	inline	resources::managed_resource_ptr		get_animation			( ) const { return m_main_animation; }

	typedef intrusive_list< landing_point, landing_point*, &landing_point::next, threading::single_threading_policy, size_policy, no_debug_policy > landing_points_type;

private:
	class ladder_occluder : public usable_object {
	private:
		virtual	bool		use_initialize				( usable_object_user_data* user ) override { VOSTOK_UNREFERENCED_PARAMETER( user ); return false; }
		virtual	bool		use_execute					( usable_object_user_data* user ) override { VOSTOK_UNREFERENCED_PARAMETER( user ); return false; }
		virtual	bool		use_finalize				( usable_object_user_data* user ) override { VOSTOK_UNREFERENCED_PARAMETER( user ); return false; }
		virtual	pcstr		use_info					( usable_object_user_data* user ) override { VOSTOK_UNREFERENCED_PARAMETER( user ); return "st_do_not_use_ladder_from_this_side"; }
	}; // class ladder_occluder

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* usable_object */
	/* 0x0128 */	landing_points_type					m_landing_points;
	/* 0x0138 */	resources::managed_resource_ptr		m_main_animation;
	/* 0x013c */	math::plane							m_plane;
	/* 0x014c */	ladder::ladder_occluder*			m_occluder;
}; // class ladder

STATIC_SIZE_ASSERT(ladder, 0x150);

} // namespace survarium

#endif // #ifndef LADDER_H_INCLUDED
