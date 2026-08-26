////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_ZONE_H_INCLUDED
#define DAMAGE_ZONE_H_INCLUDED

#include <vostok/game_core/damage_zone_core.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {
	struct light_props;
} // namespace render
} // namespace vostok

namespace survarium {

class game_world;
class zone_group;

class damage_zone : public resources::unmanaged_resource , public damage_zone_core {
	typedef damage_zone_core super;

public:
			explicit	damage_zone		( game_world& game_world );
	virtual				~damage_zone	( );

	virtual	void		activate		( zone_group* owner, physics::world* p_world, scheduler& scheduler ) override;

	virtual	void		deactivate		( ) override;

			void		load			(
							configs::binary_config_value const&		cfg_val,
							vector< resources::unmanaged_resource_ptr >&	resources,
							vector< render::light_props >&,					// PDB: __formal, genuinely unused
							u32												// PDB: __formal, genuinely unused (two collide - left unnamed)
						);

	typedef vector< resources::unmanaged_resource_ptr > particles_container;

			void		play_particles	( vector< resources::unmanaged_resource_ptr > const& particles ) const;
			void		stop_particles	( particles_container const& particles ) const;

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* damage_zone_core */
	/* 0x0218 */	particles_container	m_particles;
	/* 0x0224 */	game_world&		m_game_world;
}; // class damage_zone

STATIC_SIZE_ASSERT(damage_zone, 0x228);

} // namespace survarium

#endif // #ifndef DAMAGE_ZONE_H_INCLUDED
