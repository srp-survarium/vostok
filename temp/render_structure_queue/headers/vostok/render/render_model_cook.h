////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_MODEL_COOK_H_INCLUDED
#define RENDER_RENDER_MODEL_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;
enum vostok::resources::class_id_enum;

/* FORWARD REFS */
class vostok::render::cook_intermediate_data;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;
class vostok::vfs::vfs_iterator;
class vostok::vfs::vfs_locked_iterator;

namespace vostok {
namespace render {

struct render_model_cook : public resources::translate_query_cook {
			explicit	render_model_cook				( resources::class_id_enum model_type );

	virtual	void		translate_query					( resources::query_result_for_cook& parent ) override;

	virtual	void		delete_resource					( resources::resource_base* resource ) override;

protected:
			void		on_subresources_loaded			( resources::queries_result& data, cook_intermediate_data* cook_data );
			void		on_materials_loaded				( resources::queries_result& data, cook_intermediate_data* cook_data );
			void		on_model_settings_loaded		( resources::queries_result& data, cook_intermediate_data* cook_data );

			void		finish_model_creation			(
							resources::queries_result&		data_material_effects,
							cook_intermediate_data*			cook_data
						);

			void		query_materail_effects			( cook_intermediate_data* cook_data );

			void		on_fs_iterator_ready_submeshes	( cook_intermediate_data* cook_data, vfs::vfs_locked_iterator const& fs_it );

	inline	u32			count_models					( vfs::vfs_iterator const& arg_0 ) { /* no source */ }

public:
	virtual				~render_model_cook				( ) { /* no source */ }
}; // struct render_model_cook

STATIC_SIZE_ASSERT(render_model_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_MODEL_COOK_H_INCLUDED
