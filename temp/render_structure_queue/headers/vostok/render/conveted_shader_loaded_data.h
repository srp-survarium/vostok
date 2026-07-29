////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CONVETED_SHADER_LOADED_DATA_H_INCLUDED
#define RENDER_CONVETED_SHADER_LOADED_DATA_H_INCLUDED

/* INCLUDES */
class vostok::fs_new::virtual_path_string;
class vostok::resources::query_result_for_cook;
struct vostok::render::binary_shader_cook_data;
struct vostok::render::binary_shader_source;

namespace vostok {
namespace render {

struct conveted_shader_loaded_data {
	inline		conveted_shader_loaded_data	(
					resources::query_result_for_cook*		arg_0,
					binary_shader_cook_data*				arg_1,
					binary_shader_source*					arg_2,
					fs_new::virtual_path_string				arg_3
				) { /* no source */ }

	/* 0x0000 */	resources::query_result_for_cook*		in_out_query;
	/* 0x0004 */	binary_shader_cook_data*				cook_data;
	/* 0x0008 */	binary_shader_source*					new_resource;
	/* 0x000c */	fs_new::virtual_path_string				converted_shader_path;
	/* 0x0120 */	pbyte									data_to_save;
	/* 0x0124 */	u32										save_data_size;
}; // struct conveted_shader_loaded_data

STATIC_SIZE_ASSERT(conveted_shader_loaded_data, 0x128);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CONVETED_SHADER_LOADED_DATA_H_INCLUDED
