////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_PARTICLE_VISUAL_H_INCLUDED
#define OBJECT_PARTICLE_VISUAL_H_INCLUDED

#include "game_object_static.h"

namespace vostok {
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

// out-of-line bodies live in the object_solid_visual.cpp compiland (the
// legacy multi-class visuals TU; only this type survived into the shipped PDB)
class object_particle_visual : public game_object_static {
	typedef game_object_static super;

public:
			explicit	object_particle_visual	( base_game_scene& w );

	virtual	void		load					(
							configs::binary_config_value const&		t,
							pcstr									project_resources_path,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert					( ) override;
	virtual	void		remove					( ) override;

protected:
			void		on_visual_ready			( resources::queries_result& data, boost::function< void( game_object_& ) >& cb );

public:

protected:
	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	resources::unmanaged_resource_ptr		m_particle_system_instance_ptr;
}; // class object_particle_visual

STATIC_SIZE_ASSERT(object_particle_visual, 0x158);

} // namespace survarium

#endif // #ifndef OBJECT_PARTICLE_VISUAL_H_INCLUDED
