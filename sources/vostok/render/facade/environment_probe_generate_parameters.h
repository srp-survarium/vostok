#ifndef VOSTOK_RENDER_FACADE_ENVIRONMENT_PROBE_GENERATE_PARAMETERS_H_INCLUDED
#define VOSTOK_RENDER_FACADE_ENVIRONMENT_PROBE_GENERATE_PARAMETERS_H_INCLUDED

#include <vostok/fixed_string.h>
#include <vostok/math_float3.h>

namespace vostok {
namespace render {

struct environment_probe_generate_parameters {
	fixed_string<260>	filename;
	float3				location;
	u32					resolution;
	float				ambient_multiplier;
	bool				depth_texture;
};

STATIC_SIZE_ASSERT( environment_probe_generate_parameters, 0x128 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_ENVIRONMENT_PROBE_GENERATE_PARAMETERS_H_INCLUDED
