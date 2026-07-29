////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BACKEND_RENDER_DIRTY_OBJECTS_H_INCLUDED
#define RENDER_BACKEND_RENDER_DIRTY_OBJECTS_H_INCLUDED

namespace vostok {
namespace render {

struct backend::render_dirty_objects {
	inline			render_dirty_objects( ) { /* no source */ }

	inline	void	reset				( ) { /* no source */ }

	/* 0x0000 */	bool	vertex_buffer;
	/* 0x0001 */	bool	vertex_buffer_instance_data;
	/* 0x0002 */	bool	vertex_buffer_stream_1;
	/* 0x0003 */	bool	index_buffer;
	/* 0x0004 */	bool	input_declaration;
	/* 0x0005 */	bool	input_layout;
	/* 0x0006 */	bool	rasterizer_state;
	/* 0x0007 */	bool	depth_stencil_state;
	/* 0x0008 */	bool	effect_state;
	/* 0x0009 */	bool	vertex_shader;
	/* 0x000a */	bool	vertex_constants;
	/* 0x000b */	bool	vertex_textures;
	/* 0x000c */	bool	vertex_samplers;
	/* 0x000d */	bool	geometry_shader;
	/* 0x000e */	bool	geometry_constants;
	/* 0x000f */	bool	geometry_textures;
	/* 0x0010 */	bool	geometry_samplers;
	/* 0x0011 */	bool	pixel_shader;
	/* 0x0012 */	bool	pixel_constants;
	/* 0x0013 */	bool	pixel_textures;
	/* 0x0014 */	bool	pixel_samplers;
	/* 0x0015 */	bool	bstat;
	/* 0x0016 */	bool	primitive_topology;
}; // struct backend::render_dirty_objects

STATIC_SIZE_ASSERT(backend::render_dirty_objects, 0x17);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BACKEND_RENDER_DIRTY_OBJECTS_H_INCLUDED
