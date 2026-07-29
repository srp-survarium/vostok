////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_BINARY_SOURCE_COOK_H_INCLUDED
#define RENDER_SHADER_BINARY_SOURCE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_cook;
class vostok::tasks::task_type;
class vostok::const_buffer;

/* FORWARD REFS */
class vostok::render::conveted_shader_loaded_data;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

struct shader_binary_source_cook : public resources::unmanaged_cook {
								shader_binary_source_cook	( );
	virtual						~shader_binary_source_cook	( );

	virtual	mutable_buffer		allocate_resource			(
									resources::query_result_for_cook&		in_query,
									const_buffer							raw_file_data,
									bool									file_exist
								) override;
	virtual	void				deallocate_resource			( void* buffer ) override;
	virtual	void				create_resource				(
									resources::query_result_for_cook&		in_out_query,
									const_buffer							raw_file_data,
									mutable_buffer							in_out_unmanaged_resource_buffer
								) override;
	virtual	void				destroy_resource			( resources::unmanaged_resource* resource_to_destroy ) override;

			void				converted_shader_loaded		( conveted_shader_loaded_data* data, resources::queries_result& result );

	/* 0x0000 */	/* resources::unmanaged_cook */
private:
	/* 0x0020 */	tasks::task_type*		m_tasks_type;
	/* 0x0024 */	volatile long			m_blob_creation_counter;
}; // struct shader_binary_source_cook

STATIC_SIZE_ASSERT(shader_binary_source_cook, 0x28);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_BINARY_SOURCE_COOK_H_INCLUDED
