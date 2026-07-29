////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_USER_RENDER_SURFACE_WIRE_H_INCLUDED
#define RENDER_USER_RENDER_SURFACE_WIRE_H_INCLUDED

/* INCLUDES */
class vostok::render::user_render_surface;

/* FORWARD REFS */
class vostok::memory::chunk_reader;

namespace vostok {
namespace render {

class user_render_surface_wire : public user_render_surface {
public:
	virtual	void	load_from_chunk_data		( memory::chunk_reader& chunk ) override;

	inline			user_render_surface_wire	( ) { /* no source */ }
	virtual			~user_render_surface_wire	( ) { /* no source */ }
}; // class user_render_surface_wire

STATIC_SIZE_ASSERT(user_render_surface_wire, 0x9C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_USER_RENDER_SURFACE_WIRE_H_INCLUDED
