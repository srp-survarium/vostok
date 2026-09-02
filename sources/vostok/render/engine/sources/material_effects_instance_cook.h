// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_INSTANCE_COOK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_INSTANCE_COOK_H_INCLUDED
#include <vostok/resources_cook_classes.h>
#include <vostok/type_variant.h>

namespace vostok {
namespace render {

class custom_config_value;
struct effect_options_descriptor;
struct material_effects_instance_cook_data;

class material_effects_instance_cook : public resources::translate_query_cook {
public:
	material_effects_instance_cook( );

	virtual void translate_query(
		resources::query_result_for_cook& parent
	) override;

	virtual void delete_resource(
		resources::resource_base* resource
	) override;

	void gather_request_user_data(
		variant< 32 >*						user_data,
		custom_config_value const*			root_config,
		effect_options_descriptor const*	additional_parameters
	);

	void on_material_ready(
		resources::queries_result&			data,
		material_effects_instance_cook_data*	cook_data
	);

	void query_effects(
		resources::query_result_for_cook&		parent,
		material_effects_instance_cook_data*	cook_data
	);

	void on_effect_ready(
		resources::queries_result&			data,
		material_effects_instance_cook_data*	cook_data
	);
};

STATIC_SIZE_ASSERT( material_effects_instance_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_INSTANCE_COOK_H_INCLUDED
