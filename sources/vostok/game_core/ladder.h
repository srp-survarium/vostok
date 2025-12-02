////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef LADDER_H_INCLUDED
#define LADDER_H_INCLUDED

#include <vostok/game_core/usable_object.h>
#include <vostok/math_plane.h>

namespace vostok {
namespace physics {
	class world;
}
}

namespace survarium {

class landing_point;
class base_project;

typedef intrusive_list< landing_point, landing_point *, 0, threading::single_threading_policy, size_policy, no_debug_policy >	landing_point_list;

class ladder : public resources::unmanaged_resource , public usable_object {
public:
												ladder					( resources::managed_resource_ptr const& main_animation, math::plane const& p );
	virtual										~ladder					( );

	virtual	bool								use_initialize			( usable_object_user_data* user ) override;
	virtual	bool								use_execute				( usable_object_user_data* user ) override;
	virtual	bool								use_finalize			( usable_object_user_data* user ) override;
	virtual	pcstr								use_info				( usable_object_user_data* user ) override;

	virtual	void								load					( configs::binary_config_value const& cfg_val ) override;
	virtual	void								resolve_links			( base_project* p, configs::binary_config_value cfg ) override;

	virtual	void								activate				( physics::world* world );
	virtual	void								deactivate				( );

			void								add_landing_point		( landing_point* new_point );
			landing_point*						pop_landing_point		( );

	inline	landing_point const*				select_landing_point	( float3 const& arg_0, bool arg_1 ) const { /* no source */ }
	inline	landing_point const*				select_attachment_point	( float3 const& arg_0 ) const { /* no source */ }

	inline	resources::managed_resource_ptr		get_animation			( ) const { /* no source */ }

private:
	class ladder_occluder : public usable_object {
	public:
		virtual	bool		use_initialize				( usable_object_user_data* user ) override;
		virtual	bool		use_execute					( usable_object_user_data* user ) override { /* no source */ }
		virtual	bool		use_finalize				( usable_object_user_data* user ) override { /* no source */ }
		virtual	pcstr		use_info					( usable_object_user_data* user ) override { /* no source */ }
	}; // class ladder_occluder

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* usable_object */
	/* 0x0128 */	landing_point_list					m_landing_points;
	/* 0x0138 */	resources::managed_resource_ptr		m_main_animation;
	/* 0x013c */	math::plane							m_plane;
	/* 0x014c */	ladder::ladder_occluder*			m_occluder;
}; // class ladder

STATIC_SIZE_ASSERT(ladder, 0x150);

} // namespace survarium

#endif // #ifndef LADDER_H_INCLUDED
