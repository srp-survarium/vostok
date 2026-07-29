#ifndef VOSTOK_RENDER_ENGINE_RENDER_GEOMETRY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_GEOMETRY_H_INCLUDED

#include <vostok/fixed_string.h>
#include "res_geometry.h"

namespace vostok {
namespace render {

struct render_geometry {
	render_geometry( ) { }
	~render_geometry( ) { }

	res_geometry_ptr		geom;
	res_geometry_ptr		shadow_pass_geom;
	res_geometry_ptr		lpv_pass_geom;
	u32					vertex_count;
	u32					index_count;
	u32					primitive_count;
	fixed_string< 64 >	shading_group_name;
};

STATIC_SIZE_ASSERT( render_geometry, 0x64 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_GEOMETRY_H_INCLUDED
