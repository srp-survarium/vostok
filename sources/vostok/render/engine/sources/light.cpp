#include "pch.h"
#include "light.h"
#include "help_math.h"

namespace vostok {
namespace render {

light::light( collision::space_partitioning_tree* tree ) :
	intensity						( 0.0f ),
	spot_umbra_angle				( 0.0f ),
	spot_penumbra_angle				( 0.0f ),
	spot_falloff					( 0.0f ),
	attenuation_power				( 0.0f ),
	range							( 0.0f ),
	shadow_transparency				( 0.0f ),
	m_xform_frame					( 0 ),
	m_occluded						( false ),
	static_shadows					( false ),
	need_refresh_static_shadows		( false ),
	m_enabled						( false ),
	m_occlusion_info_index			( 0 ),
	m_collision_tree				( tree ),
	m_collision_geometry			( 0 ),
	m_collision_object				( 0 ),
	m_is_light_animated				( false ),
	m_light_animation_length		( 0.0f ),
	m_current_animation_time		( 0.0f ),
	diffuse_influence_factor		( 0.0f ),
	specular_influence_factor		( 0.0f ),
	is_shadower						( false ),
	use_with_lpv					( false ),
	sun_shadow_map_size				( 0 ),
	num_sun_cascades				( 0 ),
	shadow_z_bias					( 0.0f ),
	shadow_map_size					( 0 ),
	shadow_map_size_index			( 0 ),
	old_shadow_map_size_index		( 0 ),
	lighting_model					( 0 ),
	flags							( ),
	occluded						( false ),
	m_aabb							( math::create_zero_aabb( ) )
{
	// STATE[STUB]
	// FUNCTION BODY[0x601350]
}

light::~light( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6012d0]
}

// frac: COMDAT copy of the help_math.h inline (FUNCTION BODY[0x5ff670])

void light::tick_color_animation( float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6000c0]
}

void light::remove_collision( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ff510]
}

bool light::is_occluded( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ff650]
	return false;
}

bool light::is_cast_shadows( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ff4b0]
	return false;
}

void light::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x601540]
}

void light::set_position( float3 const& value )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ff6a0]
	position = value;
}

void light::set_color( math::color const&, float value )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ff5c0]
	intensity = value;
}

void light::set_range( float value )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ff4a0]
	range = value;
}

void light::set_orientation(
	float3 const& in_direction,
	float3 const& in_right
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x601230]
	direction = in_direction;
	right = in_right;
}

void light::on_properties_changed( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6001c0]
}

void light::xform_calc( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ff6d0]
}

void light::set_attenuation_power( float value )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ff490]
	attenuation_power = value;
}

void light::set_scale( float3 const& value )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ff470]
	scale = value;
}

} // namespace render
} // namespace vostok
