////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADOW_BATCHED_GEOMETRY_H_INCLUDED
#define RENDER_SHADOW_BATCHED_GEOMETRY_H_INCLUDED

/* INCLUDES */
struct vostok::render::batched_geometry<vostok::render::shadow_vertex>;
class vostok::render::render_model_instance_impl;

/* FORWARD REFS */
class vostok::render::batched_vertex_source;

namespace vostok {
namespace render {

struct shadow_batched_geometry : public batched_geometry< shadow_vertex > {
					shadow_batched_geometry	( );

	virtual	void	add_vertex				( batched_vertex_source const& in_vertex, float3 const& not_modified_position ) override;

	virtual	void	build					( vector< render_model_instance_impl_ptr >& model_instances ) override;

	inline			~shadow_batched_geometry( ) { /* no source */ }
}; // struct shadow_batched_geometry

STATIC_SIZE_ASSERT(shadow_batched_geometry, 0xCC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADOW_BATCHED_GEOMETRY_H_INCLUDED
