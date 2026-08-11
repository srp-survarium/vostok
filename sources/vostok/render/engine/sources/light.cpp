#include "pch.h"
#include "light.h"
#include "help_math.h"

#include <vostok/collision/api.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/render/core/options.h>

namespace vostok {
namespace render {

light::light( collision::space_partitioning_tree* tree ) :
	intensity						( 0.0f ),
	spot_umbra_angle				( 0.0f ),
	spot_penumbra_angle				( 0.0f ),
	spot_falloff					( 0.0f ),
	attenuation_power				( 0.0f ),
	range							( 0.0f ),
	shadow_transparency				( 0.1f ),
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
	sun_shadow_map_size				( 2048 ),
	num_sun_cascades				( 4 ),
	shadow_z_bias					( 0.001f ),
	shadow_map_size					( 1024 ),
	shadow_map_size_index			( 0 ),
	old_shadow_map_size_index		( 0 ),
	lighting_model					( 1 ),
	flags							( ),
	occluded						( false ),
	m_aabb							( math::create_zero_aabb( ) )
{
	// FUNCTION BODY[0x601350]
	flags.type				= render::light_type_point;
	flags.is_static			= false;
	flags.does_cast_shadows	= false;
	flags.is_hud_mode		= false;
	position				= float3( 0.f , -1000.f, 0.f );
	direction.set			( 0.f, 0.f, 1.f );
	right.set				( 1.f, 0.f, 0.f );
	range					= 8.f;
	color.set				( 1.f, 1.f, 1.f );

	for (u32 side=0; side<6; side++)
		shadow_distribution_sides[0] = true;
}

light::~light( )
{
	// FUNCTION BODY[0x6012d0]
	if ( m_collision_tree && m_collision_object )
		m_collision_tree->erase	( m_collision_object );

	collision::delete_object			( g_allocator, m_collision_object );
	collision::delete_geometry_instance	( g_allocator, m_collision_geometry);
}

// claude@NOTE: the target defines its own file-local `static float frac( float f )` here
// (lines 69-71, 0x5ff670) with the same body as help_math.h's - not a COMDAT copy of it.
// Not restored yet: no light.cpp function calls frac in our source, so an added static would
// be dead. Restore it together with whichever body regains the call.

void light::tick_color_animation( float const time_delta )
{
	if ( !m_is_light_animated )
		return;

	m_current_animation_time	+= time_delta / math::max( m_light_animation_length, math::epsilon_3 );

	if ( m_current_animation_time > 1.f )
		m_current_animation_time	= math::abs( m_current_animation_time ) - math::abs( static_cast< int >( m_current_animation_time ) );

	color						= m_color_curve.evaluate( m_current_animation_time, float4( 0.f, 0.f, 0.f, 0.f ) ).xyz( );
}

void light::remove_collision( )
{
	// FUNCTION BODY[0x5ff510]
	if ( m_collision_tree && m_collision_object )
		m_collision_tree->erase	( m_collision_object );

	collision::delete_object			( g_allocator, m_collision_object );
	collision::delete_geometry_instance	( g_allocator, m_collision_geometry);
}

bool light::is_occluded( ) const
{
	// FUNCTION BODY[0x5ff650]
	return options::ref( ).current.m_use_hiz_occlusion_culling && m_occluded;
}

bool light::is_cast_shadows( ) const
{
	// FUNCTION BODY[0x5ff4b0]
	if (!flags.does_cast_shadows)
	{
		return false;
	}
	else
	{
		if (get_type()==light_type_point)
		{
			return is_cast_shadows_in(cast_to_x_side) || is_cast_shadows_in(cast_to_neg_x_side) ||
				   is_cast_shadows_in(cast_to_y_side) || is_cast_shadows_in(cast_to_neg_y_side) ||
				   is_cast_shadows_in(cast_to_z_side) || is_cast_shadows_in(cast_to_neg_z_side);
		}
		else
		{
			return true;
		}
	}
}

void light::destroy_impl( ) const
{
	// FUNCTION BODY[0x601540]
	light const* this_ptr	= this;
	DELETE					( this_ptr );
}

void light::set_position( float3 const& value )
{
	// FUNCTION BODY[0x5ff6a0]
	float	eps					=	math::epsilon_7;	//_max	(range*0.001f,EPS_L);
	if (position.is_similar(value,eps))return	;
	position = (value);
}

void light::set_color( math::color const& c, float value )
{
	// FUNCTION BODY[0x5ff5c0]
	color.x			= c.r;
	color.y			= c.g;
	color.z			= c.b;
	color			/= 255.0f;
	intensity		= value;
}

void light::set_range( float value )
{
	// FUNCTION BODY[0x5ff4a0]
//	float	eps					=	std::max	(range*0.1f, math::epsilon_7);
//	if (math::is_similar(range,R,eps))	return	;
	range						= value	;
}

void light::set_orientation(
	float3 const& direction,
	float3 const& right
)
{
	// FUNCTION BODY[0x601230]
	this->direction				= normalize_safe( direction, float3( 1.f, 1.f, 1.f ) );
	this->right					= normalize_safe( right, float3( 1.f, 1.f, 1.f ) );
}

void light::on_properties_changed( )
{
	// FUNCTION BODY[0x6001c0]
	m_xform_frame				= u32(-1);
	xform_calc					( );

	if ( m_collision_tree && m_collision_object )
		m_collision_tree->erase	( m_collision_object );

	collision::delete_object			( g_allocator, m_collision_object );
	collision::delete_geometry_instance	( g_allocator, m_collision_geometry);

	float4x4 transform;
	m_aabb.zero					( );
	m_aabb						+= 1.f;

	switch ( flags.type ) {
		case light_type_parallel:

		case render::light_type_point : {
			m_collision_object	=
				&*collision::new_collision_object	(
					g_allocator,
					1,
					// sushi@TODO: new_sphere_geometry_instance no longer accepts a radius
					// m_collision_geometry = &*collision::new_sphere_geometry_instance( g_allocator, math::create_translation( position ), range ),
					m_collision_geometry = &*collision::new_sphere_geometry_instance( g_allocator, math::create_translation( position ) ),
					this
				);
			transform			= math::create_scale( float3(range, range, range) ) * math::create_translation( position );
			break;
		}
		case render::light_type_spot : {
			transform			= m_xform;
			float const radius	= range*math::tan( spot_penumbra_angle/2.f );
			float3 const box_half_length	= float3(
				radius,
				radius,
				range/2.f
			);
			transform			=
				math::create_scale( box_half_length ) *
				math::create_rotation( m_xform.get_angles_xyz() ) *
				math::create_translation( position + direction * range/2.f );

			m_collision_object	=
				&*collision::new_collision_object(
					g_allocator,
					1,
					m_collision_geometry = &*collision::new_box_geometry_instance( g_allocator, transform ),
					this
			);

			break;
		}
		case render::light_type_obb : {
			float3 const box_half_length	= scale + range;
			transform			=
				math::create_scale( box_half_length ) *
				math::create_rotation( m_xform.get_angles_xyz() ) *
				math::create_translation( position );

			m_collision_object	=
				&*collision::new_collision_object(
					g_allocator,
					1,
					m_collision_geometry = &*collision::new_box_geometry_instance( g_allocator, transform ),
					this
				);

			break;
		}
		case render::light_type_capsule : {
			float const scale_x_plus_range	= scale.x + range;
			float3 const box_half_length	= float3(
				scale_x_plus_range,
				scale_x_plus_range,
				scale.z + range
			);
			transform			=
				math::create_scale( box_half_length ) *
				math::create_rotation( m_xform.get_angles_xyz() ) *
				math::create_translation( position );

			m_collision_object	=
				&*collision::new_collision_object(
					g_allocator,
					1,
					m_collision_geometry = &*collision::new_box_geometry_instance( g_allocator, transform ),
					this
				);
			break;
		}
		case render::light_type_sphere : {
			m_collision_object	=
				&*collision::new_collision_object(
					g_allocator,
					1,
					// sushi@TODO: new_sphere_geometry_instance no longer accepts a radius
					// m_collision_geometry = &*collision::new_sphere_geometry_instance( g_allocator, math::create_translation( position ), range + scale.x ),
					m_collision_geometry = &*collision::new_sphere_geometry_instance( g_allocator, math::create_translation( position ) ),
					this
				);
			transform			=
				math::create_scale( float3(range, range, range) ) *
				math::create_translation( position );
			break;
		}
		case render::light_type_plane_spot : {
			float const range_X_tan_penumbra_angle_div_2 = range*math::tan(spot_penumbra_angle/2.f);
			float3 const& box_half_length	= float3( range_X_tan_penumbra_angle_div_2 + scale.x, range/2.f, range_X_tan_penumbra_angle_div_2 + scale.z );
			transform			=
				math::create_scale( box_half_length ) *
				math::create_rotation( m_xform.get_angles_xyz() ) *
				math::create_translation( position - math::normalize(math::cross_product(direction, right)) * range / 2.f );
			m_collision_object	=
				&*collision::new_collision_object(
					g_allocator,
					1,
					m_collision_geometry = &*collision::new_box_geometry_instance( g_allocator, transform ),
					this
				);

			break;
		}
		default : NODEFAULT( );
	}

	m_collision_tree->insert	( m_collision_object, transform );
	m_aabb.modify				( transform );
}

void light::xform_calc( )
{
	// FUNCTION BODY[0x5ff6d0]
//	if	(renderer::ref().frame_id() == m_xform_frame)	return;
//
//	m_xform_frame = renderer::ref().frame_id();

	// build final rotation / translation
	float3						L_dir,L_up,L_right;

	// dir
	L_dir = direction;
	float const direction_length = L_dir.length();
	R_ASSERT					( math::is_similar( direction_length, 1.f) );

	// R&N
	if (right.squared_length()>math::epsilon_5)
	{
		// use specified 'up' and 'right', just enshure ortho-normalization
		L_right = right; L_right.normalize();
		L_up = math::cross_product(L_dir,L_right); L_up.normalize();
		L_right = math::cross_product(L_up,L_dir);	L_right.normalize();
	} else {
		// auto find 'up' and 'right' vectors
		L_up.set(0,1,0); if (abs(math::dot_product(L_up, L_dir))>.99f)	L_up.set(0,0,1);
		L_right=math::cross_product(L_up,L_dir); L_right.normalize();
		L_up=math::cross_product(L_dir,L_right); L_up.normalize();
	}

	// matrix
	float4x4 rotation_X_translation;
	rotation_X_translation.i.xyz() = L_right;	rotation_X_translation.e03 = 0.f;
	rotation_X_translation.j.xyz() = L_up;		rotation_X_translation.e13 = 0.f;
	rotation_X_translation.k.xyz() = L_dir;		rotation_X_translation.e23 = 0.f;
	rotation_X_translation.c.xyz() = position;	rotation_X_translation.e33 = 1.f;

	switch ( flags.type ) {
		case render::light_type_point : {
			float const range		= this->range*1.05f;
			float4x4 const& scale	= math::create_scale( float3( range, range, range ) );
			m_xform					= scale * rotation_X_translation;
			break;
		}
		case render::light_type_spot : {
			float const range_X_tan_penumbra_angle_div_2 = range*math::tan(spot_penumbra_angle/2.f);
			float4x4 const& scale	= math::create_scale( float3( range_X_tan_penumbra_angle_div_2, range_X_tan_penumbra_angle_div_2, range ) );
			m_xform					= scale * rotation_X_translation;
			break;
		}
		case render::light_type_obb : {
			float4x4 const& scale	= math::create_scale( this->scale + range );
			m_xform					= scale * rotation_X_translation;
			break;
		}
		case render::light_type_capsule : {
			float4x4 const& scale	= math::create_scale( float3( this->scale.x, this->scale.x, this->scale.z ) + range );
			m_xform					= scale * rotation_X_translation;
			break;
		}
		case render::light_type_sphere : {
			float const range		= (this->range + scale.x)*1.05f;
			float4x4 const& scale	= math::create_scale( float3( range, range, range ) );
			m_xform					= scale * rotation_X_translation;
			break;
			break;
		}
		case render::light_type_plane_spot : {
			float const range_X_tan_penumbra_angle_div_2 = range*math::tan(spot_penumbra_angle/2.f);
			m_plane_spot_xform		= create_scale( scale ) * rotation_X_translation;
			float4x4 const& scale	= math::create_scale( float3( range_X_tan_penumbra_angle_div_2 + this->scale.x, range/2.f, range_X_tan_penumbra_angle_div_2 + this->scale.z ) );

			float4x4 rotation;
			rotation.i.xyz()		= L_right;	rotation.e03 = 0.f;
			rotation.j.xyz()		= L_up;		rotation.e13 = 0.f;
			rotation.k.xyz()		= L_dir;	rotation.e23 = 0.f;
			rotation.c.set			( 0.f, 0.f, 0.f, 1.f );

			m_xform					= scale * rotation * create_translation( position - L_up*this->range/2.f );
			break;
		}
		default : {
			m_xform.identity	();
			break;
		}
	}
}

void light::set_attenuation_power( float value )
{
	// FUNCTION BODY[0x5ff490]
	attenuation_power	= value;
}

void light::set_scale( float3 const& value )
{
	// FUNCTION BODY[0x5ff470]
	scale			= value;
}

} // namespace render
} // namespace vostok
