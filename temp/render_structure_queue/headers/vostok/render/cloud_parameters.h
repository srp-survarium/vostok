////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CLOUD_PARAMETERS_H_INCLUDED
#define RENDER_CLOUD_PARAMETERS_H_INCLUDED

/* FORWARD REFS */
class vostok::configs::binary_config_value;

namespace vostok {
namespace render {

struct cloud_parameters {
	inline			cloud_parameters( ) { /* no source */ }

	inline	void	load			( configs::binary_config_value const& arg_0 ) { /* no source */ }

	/* 0x0000 */	u32			grid_width;
	/* 0x0004 */	u32			grid_height;
	/* 0x0008 */	float		volume_width;
	/* 0x000c */	u32			noise_resulution;
	/* 0x0010 */	float		noise_period;
}; // struct cloud_parameters

STATIC_SIZE_ASSERT(cloud_parameters, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CLOUD_PARAMETERS_H_INCLUDED
