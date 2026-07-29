////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BACKEND_RENDER_DIRTY_TARGETS_H_INCLUDED
#define RENDER_BACKEND_RENDER_DIRTY_TARGETS_H_INCLUDED

namespace vostok {
namespace render {

struct backend::render_dirty_targets {
	inline			render_dirty_targets( ) { /* no source */ }

	inline	void	reset				( ) { /* no source */ }

	inline	bool	any					( ) { /* no source */ }

	/* 0x0000 */	bool	render_targets[4];
	/* 0x0004 */	bool	depth_stencil;
}; // struct backend::render_dirty_targets

STATIC_SIZE_ASSERT(backend::render_dirty_targets, 0x5);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BACKEND_RENDER_DIRTY_TARGETS_H_INCLUDED
