////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_DECAL_H_INCLUDED
#define OBJECT_DECAL_H_INCLUDED

#include "game_object_static.h"

namespace vostok {
namespace render {
	class material_effects_instance_cook_data;
} // namespace render
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class object_decal : public game_object_static {
	typedef game_object_static super;

public:
			explicit	object_decal	( base_game_scene& w );
	virtual				~object_decal	( );

	virtual	void		load			(
							configs::binary_config_value const&		t,
							pcstr									__formal,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert			( ) override;
	virtual	void		remove			( ) override;

	inline	void		requery_material( ) { /* no source */ }

			void		material_ready	(
							resources::queries_result&		data,
							render::material_effects_instance_cook_data*	cook_data,
							boost::function< void( game_object_& ) >&	cb
						);

public:
	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	u32										m_decal_id;
	/* 0x0154 */	float									m_alpha_angle;
	/* 0x0158 */	float									m_clip_angle;
	/* 0x015c */	bool									m_projection_on_static_geometry;
	/* 0x015d */	bool									m_projection_on_skeleton_geometry;
	/* 0x015e */	bool									m_projection_on_terrain_geometry;
	/* 0x015f */	bool									m_projection_on_speedtree_geometry;
	/* 0x0160 */	bool									m_projection_on_particle_geometry;
	/* 0x0164 */	float									m_decal_far_distance;
	/* 0x0168 */	float									m_decal_width;
	/* 0x016c */	float									m_decal_height;
	/* 0x0170 */	float									m_draw_priority;
	/* 0x0174 */	resources::unmanaged_resource_ptr		m_material;
}; // class object_decal

STATIC_SIZE_ASSERT(object_decal, 0x178);

} // namespace survarium

#endif // #ifndef OBJECT_DECAL_H_INCLUDED
