////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_ZONE_H_INCLUDED
#define DAMAGE_ZONE_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_resource;
class survarium::damage_zone_core;
class survarium::game_world;
typedef survarium::vector<vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> >
	survarium::vector<vostok::resources::unmanaged_resource_ptr >;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::physics::world;
class survarium::scheduler;
class survarium::vector<vostok::render::light_props>;
class survarium::zone_group;

namespace survarium {

class damage_zone : public resources::unmanaged_resource , public damage_zone_core {
public:
			explicit	damage_zone		( game_world& game_world );
	virtual				~damage_zone	( );

	virtual	void		activate		( zone_group* owner, physics::world* p_world, scheduler& scheduler ) override;

	virtual	void		deactivate		( ) override;

			void		load			(
							configs::binary_config_value const&		cfg_val,
							vector< resources::unmanaged_resource_ptr >&	resources,
							vector< render::light_props >&			__formal,
							u32										__formal
						);

			void		play_particles	( vector< resources::unmanaged_resource_ptr > const& particles ) const;
			void		stop_particles	( vector< resources::unmanaged_resource_ptr > const& particles ) const;

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* damage_zone_core */
	/* 0x0218 */	vector< resources::unmanaged_resource_ptr >	m_particles;
	/* 0x0224 */	game_world&		m_game_world;
}; // class damage_zone

STATIC_SIZE_ASSERT(damage_zone, 0x228);

} // namespace survarium

#endif // #ifndef DAMAGE_ZONE_H_INCLUDED
