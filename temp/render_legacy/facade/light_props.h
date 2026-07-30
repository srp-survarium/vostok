////////////////////////////////////////////////////////////////////////////
//	Created		: 23.02.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef LIGHT_PROPS_H_INCLUDED
#define LIGHT_PROPS_H_INCLUDED

#include <vostok/math_curve.h>

// claude@NOTE: the light_type enum + light_props struct (0xF0 layout) are
// superseded by the canonical sources/vostok/render/facade/light_props.h
// (same members/order/size; canonical adds the light_props()/~light_props()/
// load_light_props(cfg) declarations) and were removed from this file.
// The remainder below is the un-harvested queue.

namespace vostok {
namespace render {

struct light_props;

// reads a light_props out of a config value; out-of-line in
// render/facade/sources/scene_renderer.cpp (the cook entry called by
// object_light::load). Templated over the config-value type.
// No real legacy body exists (only the volatile-sink link stub in the legacy
// scene_renderer.cpp) - the real ~1664-byte body is matcher-phase work, as is
// the canonical light_props::light_props()/load_light_props(cfg) pair.
template < typename config_t >
void load_props_impl( light_props& props, config_t const& cfg );

} // namespace render
} // namespace vostok

#endif // #ifndef LIGHT_PROPS_H_INCLUDED
