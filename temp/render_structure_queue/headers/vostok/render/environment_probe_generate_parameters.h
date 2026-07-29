////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENVIRONMENT_PROBE_GENERATE_PARAMETERS_H_INCLUDED
#define RENDER_ENVIRONMENT_PROBE_GENERATE_PARAMETERS_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<260>;

namespace vostok {
namespace render {

struct environment_probe_generate_parameters {
	inline		environment_probe_generate_parameters( ) { /* no source */ }

	/* 0x0000 */	fixed_string< 260 >		filename;
	/* 0x0110 */	float3					location;
	/* 0x011c */	u32						resolution;
	/* 0x0120 */	float					ambient_multiplier;
	/* 0x0124 */	bool					depth_texture;
}; // struct environment_probe_generate_parameters

STATIC_SIZE_ASSERT(environment_probe_generate_parameters, 0x128);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENVIRONMENT_PROBE_GENERATE_PARAMETERS_H_INCLUDED
