////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKELETON_MODEL_INSTANCE_COOK_H_INCLUDED
#define RENDER_SKELETON_MODEL_INSTANCE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::render::skeleton_model_instance_cook_data;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace vostok {
namespace render {

struct skeleton_model_instance_cook : public resources::translate_query_cook {
					skeleton_model_instance_cook	( );

	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource					( resources::resource_base* resource ) override;

private:
			void	on_render_model_loaded			( resources::queries_result& result, skeleton_model_instance_cook_data* cook_data );
			void	on_skeleton_config_loaded		( resources::queries_result& result, skeleton_model_instance_cook_data* cook_data );
			void	on_skeleton_loaded				( resources::queries_result& result, skeleton_model_instance_cook_data* cook_data );
			void	on_all_subresources_ready		( skeleton_model_instance_cook_data* cook_data );

public:
	virtual			~skeleton_model_instance_cook	( ) { /* no source */ }
}; // struct skeleton_model_instance_cook

STATIC_SIZE_ASSERT(skeleton_model_instance_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKELETON_MODEL_INSTANCE_COOK_H_INCLUDED
