////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BATCHED_GEOMETRY_INTERFACE_H_INCLUDED
#define RENDER_BATCHED_GEOMETRY_INTERFACE_H_INCLUDED

/* INCLUDES */
class vostok::render::material_effects_instance;
class vostok::render::render_model_instance_impl;

/* FORWARD REFS */
class vostok::render::batched_vertex_source;

namespace vostok {
namespace render {

struct batched_geometry_interface {
	virtual	void	add_data					(
						batched_vertex_source const*	arg_0,
						const u32						arg_1,
						u16 const*						arg_2,
						const u32						arg_3,
						float4x4 const&					arg_4,
						material_effects_instance_ptr const&	arg_5
					) = 0;

	virtual	void	build						( vector< render_model_instance_impl_ptr >& arg_0 ) = 0;

	inline			batched_geometry_interface	( ) { /* no source */ }
}; // struct batched_geometry_interface

STATIC_SIZE_ASSERT(batched_geometry_interface, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BATCHED_GEOMETRY_INTERFACE_H_INCLUDED
