////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_REMOVE_LPV_INAPPROPRIATE_MODELS_H_INCLUDED
#define RENDER_REMOVE_LPV_INAPPROPRIATE_MODELS_H_INCLUDED

/* FORWARD REFS */
class vostok::render::lpv_render_surface;

namespace vostok {
namespace render {

struct remove_lpv_inappropriate_models {
	inline			remove_lpv_inappropriate_models	( ) { /* no source */ }

	inline	bool	operator()						( lpv_render_surface const& arg_0 ) { /* no source */ }
}; // struct remove_lpv_inappropriate_models

STATIC_SIZE_ASSERT(remove_lpv_inappropriate_models, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_REMOVE_LPV_INAPPROPRIATE_MODELS_H_INCLUDED
