#ifndef VOSTOK_RENDER_ENGINE_RENDER_TARGET_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_TARGET_H_INCLUDED

#include <vostok/render/core/resource_intrusive_base.h>

namespace vostok {
namespace render {

class render_target : public resource_intrusive_base {
public:
	void destroy_impl( ) const { }
};

typedef intrusive_ptr<
	render_target,
	resource_intrusive_base,
	threading::single_threading_policy
> render_target_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_TARGET_H_INCLUDED
