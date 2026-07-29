#ifndef VOSTOK_RENDER_ENGINE_RENDER_SURFACE_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_SURFACE_INSTANCE_H_INCLUDED

namespace vostok {
namespace render {

class render_model_instance_impl;
class render_surface;

struct render_surface_instance {
	render_surface_instance( ) { }

	void set_constants( );
	bool is_occluded( ) const;
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_SURFACE_INSTANCE_H_INCLUDED
