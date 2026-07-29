////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_RENDER_SURFACE_H_INCLUDED
#define RENDER_STATIC_RENDER_SURFACE_H_INCLUDED

/* INCLUDES */
class vostok::render::render_surface;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::memory::chunk_reader;
class vostok::render::batched_geometry_interface;

namespace vostok {
namespace render {

class static_render_surface : public render_surface {
public:
			explicit	static_render_surface			( bool colored );
	virtual				~static_render_surface			( ) { /* no source */ }

	virtual	void		load							( configs::binary_config_value const& properties, memory::chunk_reader& chunk ) override;

	virtual	bool		is_dynamic						( ) const override { /* no source */ }

	virtual	void		fill_lpv_vertex_color			( batched_geometry_interface* in_out_geometry, float4x4 const& transform ) override;

	virtual	void		add_shadow_vertices				( batched_geometry_interface* in_out_geometry, float4x4 const& transform ) override;

	inline	void		create_material_pass_geometry	( ) { /* no source */ }
			void		create_shadow_pass_geometry		( pcbyte data, const u32 num_vertices, const u32 stride );
}; // class static_render_surface

STATIC_SIZE_ASSERT(static_render_surface, 0x9C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_RENDER_SURFACE_H_INCLUDED
