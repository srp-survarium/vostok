////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MATERIAL_EFFECTS_INSTANCE_COOK_H_INCLUDED
#define RENDER_MATERIAL_EFFECTS_INSTANCE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_options_descriptor;
class vostok::render::material_effects_instance_cook_data;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;
class vostok::variant<32>;

namespace vostok {
namespace render {

class material_effects_instance_cook : public resources::translate_query_cook {
public:
					material_effects_instance_cook	( );

	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource					( resources::resource_base* resource ) override;

			void	gather_request_user_data		(
						variant< 32 >*						user_data,
						custom_config_value const*			root_config,
						effect_options_descriptor const*	additional_parameters
					);

			void	on_material_ready				( resources::queries_result& data, material_effects_instance_cook_data* cook_data );

			void	query_effects					(
						resources::query_result_for_cook&		parent,
						material_effects_instance_cook_data*	cook_data
					);

			void	on_effect_ready					( resources::queries_result& data, material_effects_instance_cook_data* cook_data );

	virtual			~material_effects_instance_cook	( ) { /* no source */ }
}; // class material_effects_instance_cook

STATIC_SIZE_ASSERT(material_effects_instance_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MATERIAL_EFFECTS_INSTANCE_COOK_H_INCLUDED
