// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_RENDER_TARGET_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_TARGET_H_INCLUDED
// Compatibility redirect to the engine-wide core type.
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
