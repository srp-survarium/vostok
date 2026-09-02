// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_COOK_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_EFFECT_COOK_H_INCLUDED
#include <vostok/resources_cook_classes.h>

namespace vostok {

namespace resources {
class queries_result;
} // namespace resources

namespace render {

struct effect_compile_data;
class res_effect;

class effect_cook : public resources::unmanaged_cook {
public:
	effect_cook( );

	virtual mutable_buffer allocate_resource(
		resources::query_result_for_cook& query,
		const_buffer raw_file_data,
		bool file_exist
	);
	virtual void deallocate_resource( void* buffer );
	virtual void create_resource(
		resources::query_result_for_cook& query,
		const_buffer raw_file_data,
		mutable_buffer unmanaged_resource_buffer
	);
	virtual void destroy_resource( resources::unmanaged_resource* resource );

	void on_all_tasks_finished(
		res_effect* effect,
		resources::query_result_for_cook* query,
		effect_compile_data* compile_data
	);
	void on_binary_shaders(
		resources::query_result_for_cook* query,
		res_effect* effect,
		effect_compile_data* compile_data,
		resources::queries_result& data
	);
};

STATIC_SIZE_ASSERT( effect_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_COOK_H_INCLUDED
