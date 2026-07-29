////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_COOK_H_INCLUDED
#define RENDER_EFFECT_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_cook;
class vostok::const_buffer;

/* FORWARD REFS */
class vostok::render::effect_compile_data;
class vostok::render::res_effect;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

class effect_cook : public resources::unmanaged_cook {
public:
								effect_cook				( );

	virtual	mutable_buffer		allocate_resource		(
									resources::query_result_for_cook&		in_query,
									const_buffer							raw_file_data,
									bool									file_exist
								) override;
	virtual	void				deallocate_resource		( void* buffer ) override;
	virtual	void				create_resource			(
									resources::query_result_for_cook&		in_out_query,
									const_buffer							raw_file_data,
									mutable_buffer							in_out_unmanaged_resource_buffer
								) override;
	virtual	void				destroy_resource		( resources::unmanaged_resource* resource_to_destroy ) override;

	inline	void				on_all_tasks_finished	(
									res_effect*								arg_0,
									resources::query_result_for_cook*		arg_1,
									effect_compile_data*					arg_2
								) { /* no source */ }
			void				on_binary_shaders		(
									resources::query_result_for_cook*		in_out_query,
									res_effect*								effect_resource,
									effect_compile_data*					compile_data,
									resources::queries_result&				data
								);

	virtual						~effect_cook			( ) { /* no source */ }
}; // class effect_cook

STATIC_SIZE_ASSERT(effect_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_COOK_H_INCLUDED
