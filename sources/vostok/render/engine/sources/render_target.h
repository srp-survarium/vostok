#ifndef VOSTOK_RENDER_ENGINE_RENDER_TARGET_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_TARGET_H_INCLUDED

// The engine carcass initially carried a placeholder render_target here; the
// real class (one type engine-wide) lives in the core module.
#include <vostok/render/core/dx11/render_target.h>

namespace vostok {
namespace render {

typedef intrusive_ptr<
	render_target,
	resource_intrusive_base,
	threading::single_threading_policy
> render_target_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_TARGET_H_INCLUDED
