#include "pch.h"
#include "grass_world.h"

namespace vostok {
namespace render {

grass_world::grass_world( ) :
	m_patches_tree						( 0 ),
	m_ambient_color						( 0 ),
	m_c_environment_skylight_upper_color	( 0 ),
	m_c_environment_skylight_lower_color	( 0 ),
	m_c_environment_skylight_parameters	( 0 ),
	m_c_sun_direction					( 0 ),
	m_c_sun_color						( 0 ),
	m_patch_parameters					( 0 ),
	m_trample_parameters					( 0 ),
	m_shadow_cascade_index_parameter		( 0 ),
	m_wind_info_parameters					( 0 ),
	m_need_populate						( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6364e0]
}

void grass_world::set_wind_parameters( float2 const&, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635d10]
}

void grass_world::set_patch_parameters( grass_patch* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635c70]
}

void grass_world::set_trample_parameters( trample_desc& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635c40]
}

void grass_world::set_shadow_parameters( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635be0]
}

grass_world::~grass_world( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6370f0]
}

u32 grass_world::add_template( grass_render_model_ptr const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x636920]
	return 0;
}

void grass_world::add_trample( trample_desc const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635f10]
}

u32 grass_world::add_instance( u32, math::color const&, float4x4 const&, u8, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x636090]
	return 0;
}

void grass_world::remove_instance( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635d90]
}

grass_template* grass_world::id_to_template( u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x635a70]
	return 0;
}

grass_template* grass_world::find_template( grass_render_model_ptr const& ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x635a20]
	return 0;
}

grass_patch* grass_world::find_patch( float3 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6359c0]
	return 0;
}

void grass_world::remove_patches( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x636ee0]
}

void grass_world::populate( float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x637b90]
}

void grass_world::merge_patches( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6370c0]
}

void grass_world::render_debug( renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x637270]
}

bool sort_grass_patch_predicate::operator()( grass_patch const*, grass_patch const* ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x70670]
	return false;
}

void grass_world::process_sorting( float3 const&, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x636130]
}

void grass_world::process_culling( renderer_context*, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6361b0]
}

void grass_world::accumulate_trample( renderer*, renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6371c0]
}

void grass_world::remove_trample( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635bb0]
}

void grass_world::render(
	renderer_context*,
	float3 const&,
	enum_render_stage_type,
	u32,
	float,
	bool,
	res_effect*,
	bool,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x638150]
}

void grass_world::add_grass_layer( grass_layer_desc*, grass_layer_data*, bool, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x637b70]
}

void grass_world::update_grass_layer(
	grass_layer_desc*,
	grass_layer_data*,
	bool,
	bool,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x637940]
}

void grass_world::clear( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x636fb0]
}

void grass_world::remove_grass_layer( u8, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635e40]
}

void grass_world::remove_instances( vector<u32> const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635e10]
}

void setup_seed_clk( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635990]
}

u8 select_model_template( float*, float, u8 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635b50]
	return 0;
}

float select_model_orientation( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635b10]
	return 0.0f;
}

float select_model_scale( float, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635ac0]
	return 0.0f;
}

void grass_world::grass_layer_resources_ready_from_cook(
	resources::queries_result&,
	grass_layer_desc*,
	grass_layer_data*,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x636da0]
}

void grass_world::grass_layer_resources_ready(
	resources::queries_result&,
	grass_layer_desc*,
	grass_layer_data*,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x636990]
}

void grass_world::remove_layer_instances( u8, float2 const&, float2 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x635f50]
}

} // namespace render
} // namespace vostok
