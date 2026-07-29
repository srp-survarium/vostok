////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_LAYER_DESC_H_INCLUDED
#define RENDER_GRASS_LAYER_DESC_H_INCLUDED

/* INCLUDES */
class vostok::fixed_vector<vostok::render::grass_layer_desc::model_desc,16>;

namespace vostok {
namespace render {

struct grass_layer_desc {
	inline		grass_layer_desc	( ) { /* no source */ }
	inline		~grass_layer_desc	( ) { /* no source */ }

	/* 0x0000 */	u8			id;
	/* 0x0004 */	float		max_slope_ang;
	/* 0x0008 */	bool		use_face_normal;
	/* 0x0009 */	bool		random_orient;
	/* 0x000c */	float		random_scale;
	/* 0x0010 */	float		wind_factor;
	/* 0x0014 */	fixed_vector< grass_layer_desc::model_desc, 16 >	models_list;
}; // struct grass_layer_desc

STATIC_SIZE_ASSERT(grass_layer_desc, 0x119C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_LAYER_DESC_H_INCLUDED
