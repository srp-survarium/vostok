////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_LOADING_DATA_H_INCLUDED
#define RENDER_GRASS_LOADING_DATA_H_INCLUDED

/* INCLUDES */
class vostok::configs::binary_config_value;
class vostok::fixed_string<260>;

namespace vostok {
namespace render {

struct grass_loading_data {
	inline		grass_loading_data	( ) { /* no source */ }

	/* 0x0000 */	configs::binary_config_value const*		t_current;
	/* 0x0004 */	fixed_string< 260 >						project_resources_path;
}; // struct grass_loading_data

STATIC_SIZE_ASSERT(grass_loading_data, 0x114);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_LOADING_DATA_H_INCLUDED
