#ifndef VOSTOK_RENDER_ENGINE_LPV_RENDER_SURFACE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_LPV_RENDER_SURFACE_H_INCLUDED

#include "render_model_instance_impl.h"

namespace vostok {
namespace render {

struct render_surface_instance;

struct lpv_render_surface {
	render_surface_instance*			surface;
	render_model_instance_impl_ptr	model;
};

STATIC_SIZE_ASSERT( lpv_render_surface, 0x8 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_LPV_RENDER_SURFACE_H_INCLUDED
