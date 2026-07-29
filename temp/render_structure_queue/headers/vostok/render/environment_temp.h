////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENVIRONMENT_TEMP_H_INCLUDED
#define RENDER_ENVIRONMENT_TEMP_H_INCLUDED

/* INCLUDES */
struct vostok::render::cloud_key_parameters;

namespace vostok {
namespace render {

struct environment_temp {
									environment_temp	( );
									~environment_temp	( );

	inline	void					tick				( float arg_0 ) { /* no source */ }

			cloud_key_parameters	get_interp_key		( float time );
			cloud_key_parameters	get_next_key		( const u32 index );
	inline	float3					get_sun_direction	( float arg_0 ) { /* no source */ }

	/* 0x0000 */	cloud_key_parameters*		keys;
	/* 0x0004 */	float						key_time_step;
	/* 0x0008 */	u32							num_keys;
	/* 0x000c */	float4						sky_light[5];
	/* 0x005c */	float						time;
}; // struct environment_temp

STATIC_SIZE_ASSERT(environment_temp, 0x60);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENVIRONMENT_TEMP_H_INCLUDED
