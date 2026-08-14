#ifndef VOSTOK_RENDER_ENGINE_GEOMETRY_BATCH_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_GEOMETRY_BATCH_H_INCLUDED

#include <vostok/math_aabb.h>

#include "material_effects_instance.h"
#include "res_geometry.h"

namespace vostok {
namespace render {

struct geometry_batch {
	geometry_batch( ) { }

	geometry_batch(
		math::aabb const&				in_bbox,
		res_geometry_ptr const&			in_geometry,
		u32 const						in_num_indices,
		material_effects_instance_ptr	in_material
	) :
		bbox		( in_bbox ),
		mtl			( in_material ),
		geometry	( in_geometry ),
		num_indices	( in_num_indices )
	{
	}

	math::aabb						bbox;
	material_effects_instance_ptr	mtl;
	res_geometry_ptr				geometry;
	u32								num_indices;
};

STATIC_SIZE_ASSERT( geometry_batch, 0x24 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_GEOMETRY_BATCH_H_INCLUDED
