#include "pch.h"
#include "material_effects_instance_cook.h"

#include <vostok/render/core/custom_config.h>
#include <vostok/render/facade/cull_mode.h>
#include <vostok/render/facade/material_effects_instance_cook_data.h>

#include "material_effects_instance.h"

namespace vostok {
namespace render {

static void replace_value_data(
	custom_config_value const&,
	custom_config_value
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64d310]
}

static void replace_values( custom_config_value const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x64d4d0]
}

static void merge_configs(
	custom_config_value const*&,
	custom_config_value const*&,
	custom_config_value const&,
	custom_config_value const*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64d380]
}

static u32 get_num_fields( custom_config_value const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x64d330]
	return 0;
}

static custom_config_ptr merge_effect_options(
	custom_config_value const&,
	effect_options_descriptor const&,
	u32& out_crc
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64d690]
	out_crc = 0;
	return custom_config_ptr( );
}

static D3D11_CULL_MODE base_cull_mode_to_d3d_cull_mode(
	enum_cull_mode cull_mode
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64d2f0]
	switch ( cull_mode ) {
	case cull_mode_front:
		return D3D11_CULL_FRONT;
	case cull_mode_back:
		return D3D11_CULL_BACK;
	default:
		return D3D11_CULL_NONE;
	}
}

material_effects_instance_cook::material_effects_instance_cook( ) :
	resources::translate_query_cook(
		resources::material_effects_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64d5f0]
}

void material_effects_instance_cook::on_material_ready(
	resources::queries_result&,
	material_effects_instance_cook_data*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64edd0]
}

void material_effects_instance_cook::query_effects(
	resources::query_result_for_cook&,
	material_effects_instance_cook_data*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64e6b0]
}

void material_effects_instance_cook::translate_query(
	resources::query_result_for_cook&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64efa0]
}

void material_effects_instance_cook::on_effect_ready(
	resources::queries_result&,
	material_effects_instance_cook_data*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64db60]
}

void material_effects_instance_cook::gather_request_user_data(
	variant< 32 >*,
	custom_config_value const*,
	effect_options_descriptor const*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64d7a0]
}

void material_effects_instance_cook::delete_resource(
	resources::resource_base* resource
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x64d650]
	material_effects_instance* instance =
		static_cast< material_effects_instance* >( resource );
	DELETE( instance );
}

} // namespace render
} // namespace vostok
