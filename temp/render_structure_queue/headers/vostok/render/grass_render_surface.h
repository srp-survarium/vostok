////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_RENDER_SURFACE_H_INCLUDED
#define RENDER_GRASS_RENDER_SURFACE_H_INCLUDED

/* INCLUDES */
class vostok::render::render_surface;
struct vostok::render::grass_source_vertex;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::memory::chunk_reader;

namespace vostok {
namespace render {

class grass_render_surface : public render_surface {
public:
					grass_render_surface	( );
	virtual			~grass_render_surface	( );

	virtual	void	load					( configs::binary_config_value const& properties, memory::chunk_reader& chunk ) override;

	virtual	bool	is_dynamic				( ) const override { /* no source */ }

	/* 0x0000 */	/* render_surface */
	/* 0x009c */	grass_source_vertex*	m_vertices;
	/* 0x00a0 */	u32						m_num_vertices;
	/* 0x00a4 */	u16*					m_indices;
	/* 0x00a8 */	u32						m_num_indices;
}; // class grass_render_surface

STATIC_SIZE_ASSERT(grass_render_surface, 0xAC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_RENDER_SURFACE_H_INCLUDED
