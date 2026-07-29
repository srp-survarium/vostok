////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_USER_RENDER_SURFACE_EDITABLE_H_INCLUDED
#define RENDER_USER_RENDER_SURFACE_EDITABLE_H_INCLUDED

/* INCLUDES */
class vostok::render::untyped_buffer;
class vostok::render::user_render_surface;

/* FORWARD REFS */
class vostok::memory::chunk_reader;

namespace vostok {
namespace render {

class user_render_surface_editable : public user_render_surface {
public:
	virtual	void	load_from_chunk_data			( memory::chunk_reader& chunk ) override;

	inline			user_render_surface_editable	( ) { /* no source */ }
	virtual			~user_render_surface_editable	( ) { /* no source */ }

	/* 0x0000 */	/* user_render_surface */
	/* 0x009c */	untyped_buffer*		m_vb;
}; // class user_render_surface_editable

STATIC_SIZE_ASSERT(user_render_surface_editable, 0xA0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_USER_RENDER_SURFACE_EDITABLE_H_INCLUDED
