////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LPV_RENDER_SURFACE_H_INCLUDED
#define RENDER_LPV_RENDER_SURFACE_H_INCLUDED

/* INCLUDES */
struct vostok::render::render_surface_instance;
typedef vostok::resources::resource_ptr<vostok::render::render_model_instance_impl,vostok::resources::unmanaged_intrusive_base>
	vostok::render::render_model_instance_impl_ptr;
class vostok::render::render_model_instance_impl;

namespace vostok {
namespace render {

struct lpv_render_surface {
	inline		lpv_render_surface	( ) { /* no source */ }
	inline		~lpv_render_surface	( ) { /* no source */ }

	/* 0x0000 */	render_surface_instance*			surface;
	/* 0x0004 */	render_model_instance_impl_ptr		model;
}; // struct lpv_render_surface

STATIC_SIZE_ASSERT(lpv_render_surface, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LPV_RENDER_SURFACE_H_INCLUDED
