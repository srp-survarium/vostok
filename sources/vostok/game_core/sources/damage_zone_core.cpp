// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/damage_zone_core.h>

#include <vostok/collision/bone_collision_data.h>
#include <vostok/physics/contact_test_predicate.h>
#include <vostok/physics/base_physics_object.h>
#include <vostok/collision/game_object.h>
#include <vostok/game_core/hit_receiver.h>
#include <vostok/game_core/game_scene.h>
#include <vostok/game_core/zone_group.h>
#include <vostok/game_core/generic_anomaly_core.h>

namespace survarium {

hit_receiver_info::hit_receiver_info( hit_receiver* receiver, physics::base_physics_object* rigid_body ) :
	m_receiver		( receiver ),
	m_rigid_body	( rigid_body ),
	m_was_hit		( false )
{
}

// sushi@TODO: Think about this a bit more.
bool hit_receiver_info::operator==( hit_receiver_info const& rhs ) const
{
	return m_receiver->m_pointer->m_pointer == rhs.m_receiver->m_pointer->m_pointer;
}

damage_zone_core::damage_zone_core( ) :
	hit_initiator				( u8(-1), true ),
	m_physics_world				( NULL ),
	m_owner						( NULL ),
	m_accumulated_hit_time_ms	( 0 ),
	m_standalone				( true )
{
}

damage_zone_core::~damage_zone_core( )
{
}

void damage_zone_core::load( configs::binary_config_value const& t )
{
	collision_sensor::load( t );

	m_hit_curve.load( t["hit_curve"] );
	m_motion_on_bound_curve.load( t["on_bound_motion_curve"] );
	m_motion_on_center_curve.load( t["on_center_motion_curve"] );

	m_apply_hit_type		= (apply_hit_type)(u8)t["apply_hit_type"];
	m_max_hit				= (float)t["max_hit"];
	m_min_hit				= (float)t["min_hit"];
	m_max_armor_piercing	= (float)t["max_armor_piercing"];
	m_min_armor_piercing	= (float)t["min_armor_piercing"];
	m_hit_interval_ms		= (u32)t["interval_in_msec"];
	m_damage_type			= (pcstr)t["damage_type"];

	configs::binary_config_value bone_parts_filter	= t["hit_parts_filter"];
	configs::binary_config_value const* it	= bone_parts_filter.begin( );
	configs::binary_config_value const* end	= bone_parts_filter.end( );
	for ( ; it != end ; ++it )
		m_body_parts_filter.push_back((pcstr)it);
}

static bool compare_bone_data_predicate( std::pair< collision::bone_collision_data *, float > const& lhs, std::pair< collision::bone_collision_data *, float > const& rhs )
{
	return lhs.first->skeleton_bone_index == rhs.first->skeleton_bone_index;
}

static float distance_from_sphere_center_to_point_on_shape( float radius )
{
	return radius;
}

static float distance_from_box_center_to_point_on_shape( float4x4 const& transform, float3 const& dim, float3 const& source_position )
{
	float3 dir			= source_position - transform.c.xyz( );
	float3 result		= transform.c.xyz( );
	float3 half_sides	= dim;

	for ( s32 i = 0 ; i < 3 ; ++i )
	{
		float3 axis	= transform.lines[i].xyz( );
		axis.normalize( );
		float dist = dir.dot_product( axis );

		if ( dist > half_sides[i] )
			dist = half_sides[i];

		if ( -half_sides[i] > dist )
			dist = -half_sides[i];

		result += axis * dist;
	}

	return ( result - transform.c.xyz( ) ).length( );
}

static float distance_from_capsule_center_to_point_on_shape(
	float4x4 const&		transform,
	float				half_length,
	float				radius,
	float3 const&		source_position
)
{
	float3 center					= transform.c.xyz( );
	float3 y_axis					= transform.j.xyz( );
	float3 top_surface_center		= center + y_axis * half_length;
	float3 bottom_surface_center	= center - y_axis * half_length;
	float3 height_vector			= top_surface_center - bottom_surface_center;
	const float proj_to_y_axis		= ( source_position - top_surface_center ).dot_product( height_vector ) / height_vector.dot_product( height_vector );

	if ( proj_to_y_axis > 0.0f && 1.0f > proj_to_y_axis )
	{
		float3 height_vector_proj_point	= top_surface_center + height_vector * proj_to_y_axis;
		float3 dir						= height_vector_proj_point - source_position;
		return ( center - ( height_vector_proj_point + dir.normalize( ) * radius ) ).length( );
	}

	float3 surface_center			= proj_to_y_axis < 0.0f ? top_surface_center : bottom_surface_center;
	return ( center - ( surface_center + ( surface_center - source_position ).normalize( ) * radius ) ).length( );
}

static float distance_from_cylinder_center_to_point_on_shape(
	float4x4 const&		transform,
	float				radius,
	float				half_length,
	float3 const&		source_position
)
{
	float3 center					= transform.c.xyz( );
	float3 y_axis					= transform.j.xyz( );
	float3 top_surface_center		= center + y_axis * half_length;
	float3 bottom_surface_center	= center - y_axis * half_length;
	float3 height_vector			= top_surface_center - bottom_surface_center;
	const float proj_to_y_axis		= ( source_position - top_surface_center ).dot_product( height_vector ) / height_vector.dot_product( height_vector );

	if ( proj_to_y_axis > 0.0f && 1.0f > proj_to_y_axis )
	{
		float3 height_vector_proj_point	= top_surface_center + height_vector * proj_to_y_axis;
		float3 dir						= height_vector_proj_point - source_position;
		return ( center - ( height_vector_proj_point + dir.normalize( ) * radius ) ).length( );
	}

	float3 surface_center			= proj_to_y_axis < 0.0f ? top_surface_center : bottom_surface_center;
	float3 circle_point_dir			= surface_center - source_position;
	float3 proj						= y_axis * y_axis.dot_product( circle_point_dir );
	float3 circle_proj_vec			= proj - circle_point_dir;
	return ( center - ( surface_center + circle_proj_vec ) ).length( );
}

struct compare_body_parts_predicate {
	inline explicit compare_body_parts_predicate( pcstr name ) : m_name( name ) { }

	inline bool operator()( fixed_string<16> const& body_part ) const
	{
		return strings::equal( m_name, body_part.c_str( ) );
	}
	pcstr	m_name;
};

STATIC_SIZE_ASSERT(compare_body_parts_predicate, 0x4);

struct dz_bone_data_contact_test_predicate : public physics::contact_test_predicate {
public:
	inline dz_bone_data_contact_test_predicate(
		vectora<std::pair<collision::bone_collision_data *, float> >& result,
		vector<fixed_string<16> > const* body_parts_filter
	) :
		m_result( &result ),
		m_body_parts_filter( body_parts_filter )
	{
	}

	virtual	float		add_single_result				(
							void*						user_data,
							physics::primitive_type		first_shape_type,
							float4x4 const&				first_shape_transform,
							float3 const&				first_shape_dimension,
							physics::primitive_type		second_shape_type,
							float4x4 const&				second_shape_transform,
							float3 const&				second_shape_dimension
						) override;
public:
	/* 0x0000 */	/* physics::contact_test_predicate */
	/* 0x0004 */	vectora<std::pair<collision::bone_collision_data *,float> >*	m_result;
	/* 0x0008 */	vector<fixed_string<16> > const*	m_body_parts_filter;
}; // struct dz_bone_data_contact_test_predicate

STATIC_SIZE_ASSERT(dz_bone_data_contact_test_predicate, 0xC);

float dz_bone_data_contact_test_predicate::add_single_result(
	void*						user_data,
	physics::primitive_type		first_shape_type,
	float4x4 const&				first_shape_transform,
	float3 const&				first_shape_dimension,
	physics::primitive_type		second_shape_type,
	float4x4 const&				second_shape_transform,
	float3 const&				second_shape_dimension
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( first_shape_type, first_shape_dimension );

	std::pair< collision::bone_collision_data*, float > result;
	result.first = static_cast< collision::bone_collision_data* >( user_data );

	if ( m_body_parts_filter )
	{
		if ( std::find_if(
			m_body_parts_filter->begin( ),
			m_body_parts_filter->end( ),
			compare_body_parts_predicate( result.first->body_part_name.c_str( ) )
		) == m_body_parts_filter->end( ) )
			return 0.0f;
	}

	float max_distance = 1.0f;
	switch ( second_shape_type )
	{
	case physics::primitive_sphere:
		max_distance = distance_from_sphere_center_to_point_on_shape( second_shape_dimension[0] );
		break;
	case physics::primitive_box:
		max_distance = distance_from_box_center_to_point_on_shape( second_shape_transform, second_shape_dimension, first_shape_transform.c.xyz( ) );
		break;
	case physics::primitive_capsule:
		max_distance = distance_from_capsule_center_to_point_on_shape( second_shape_transform, second_shape_dimension[0], second_shape_dimension[1], first_shape_transform.c.xyz( ) );
		break;
	case physics::primitive_cylinder:
		max_distance = distance_from_cylinder_center_to_point_on_shape( second_shape_transform, second_shape_dimension[0], second_shape_dimension[1], first_shape_transform.c.xyz( ) );
		break;
	default: NODEFAULT( );
	}

	float const d_1 = ( second_shape_transform.c.xyz( ) - first_shape_transform.c.xyz( ) ).length( );
	result.second = d_1 / max_distance;
	m_result->push_back( result );

	return 0.0f;
}

void damage_zone_core::on_inside( buffer_vector<physics::base_physics_object *> const& objects )
{
	VOSTOK_UNREFERENCED_PARAMETER( objects );
}

void damage_zone_core::on_leave( buffer_vector<physics::base_physics_object *> const& objects )
{
	physics::base_physics_object* const* it		= objects.begin( );
	physics::base_physics_object* const* end	= objects.end( );
	for ( ; it != end ; ++it )
	{
		ASSERT( UNKNOWN_EXPRESSION_T( *it ) );
		hit_receiver* receiver = (*it)->user_data->cast_to_hit_receiver( );
		ASSERT( UNKNOWN_EXPRESSION_T( receiver ) );
		ASSERT( UNKNOWN_EXPRESSION_T( receiver ) );

		if ( m_owner )
		{
			receiver->unsubscribe_from_actions( this );
			m_owner->core( )->on_hit_receiver_leave( receiver, this );
		}

		hit_receiver_info info( receiver, NULL );
		ASSERT( UNKNOWN_EXPRESSION_T( receiver ) );
		m_receivers.erase( std::find( m_receivers.begin( ), m_receivers.end( ), info ) );
	}
}

void damage_zone_core::on_enter( buffer_vector<physics::base_physics_object *> const& objects )
{
	physics::base_physics_object* const* it		= objects.begin( );
	physics::base_physics_object* const* end	= objects.end( );
	for ( ; it != end ; ++it )
	{
		ASSERT( UNKNOWN_EXPRESSION_T( *it ) );
		hit_receiver* receiver = (*it)->user_data->cast_to_hit_receiver( );
		ASSERT( UNKNOWN_EXPRESSION_T( receiver ) );
		ASSERT( UNKNOWN_EXPRESSION_T( receiver ) );

		if ( m_owner )
		{
			receiver->subscribe_on_actions( this );
			m_owner->core( )->on_hit_receiver_enter( receiver, this );
		}

		hit_receiver_info info( receiver, *it );
		ASSERT( UNKNOWN_EXPRESSION_T( receiver ) );
		m_receivers.push_back( info );
	}
}

void damage_zone_core::tick( const u32 frame_delta, const u32 current_time )
{
	collision_sensor::tick( frame_delta, current_time );

	switch ( m_apply_hit_type )
	{
	case survarium::on_enter:
		hit_on_enter( frame_delta, current_time );
		break;
	case survarium::on_inside:
		hit_on_inside( frame_delta, current_time );
		break;
	case survarium::on_motion_inside:
		hit_on_motion_inside( frame_delta, current_time );
		break;
	default: NODEFAULT();
	}
}

static bool remove_null_receivers_predicate( hit_receiver_info const& info ) { return info.m_receiver == NULL; }

bool damage_zone_core::is_filter_passed( physics::base_physics_object* object ) const
{
	return ( object->get_collision_group( ) & 0x40 ) != 0;
}

void damage_zone_core::hit_on_enter( const u32 frame_delta, const u32 current_time )
{
	VOSTOK_UNREFERENCED_PARAMETERS( frame_delta, current_time );

	if ( m_receivers.empty( ) )
		return;

	hit_receiver_info* it = m_receivers.begin( );
	hit_receiver_info* end = m_receivers.end( );
	for ( ; it != end ; ++it )
	{
		if ( it->m_was_hit )
			continue;

		typedef vectora<std::pair<collision::bone_collision_data*, float> > bone_data_container;
		bone_data_container results( g_allocator );
		dz_bone_data_contact_test_predicate predicate( results, &m_body_parts_filter );

		ASSERT( UNKNOWN_EXPRESSION_T( it->m_rigid_body ) );
		contact_test( it->m_rigid_body, predicate );

		bone_data_container unique_bones( g_allocator );
		std::insert_iterator<bone_data_container> insert_it( unique_bones, unique_bones.begin( ) );
		std::unique_copy( results.begin( ), results.end( ), insert_it, compare_bone_data_predicate );

		std::pair<collision::bone_collision_data*, float> const* ub_it = unique_bones.begin( );
		std::pair<collision::bone_collision_data*, float> const* ub_end = unique_bones.end( );
		for ( ; ub_it != ub_end ; ++ub_it )
		{
			if ( ub_it->second <= 1.0f )
			{
				it->m_receiver->hit( this, *ub_it->first, m_damage_type.c_str( ), m_max_hit, m_max_armor_piercing, NULL );
				it->m_was_hit = true;
			}
		}

		ASSERT( UNKNOWN_EXPRESSION_T( it->m_receiver ) );
		if ( it->m_was_hit && m_owner )
			m_owner->on_zone_act( this, it->m_receiver );
	}
}

void damage_zone_core::hit_on_inside( const u32 frame_delta, const u32 current_time )
{
	m_accumulated_hit_time_ms += frame_delta;

	if ( m_receivers.empty( ) )
		return;

	if ( m_accumulated_hit_time_ms >= m_hit_interval_ms )
	{
		hit_receiver_info* it = m_receivers.begin( );
		hit_receiver_info* end = m_receivers.end( );
		for ( ; it != end ; ++it )
		{
			it->m_was_hit = false;

			typedef vectora<std::pair<collision::bone_collision_data*, float> > bone_data_container;
			bone_data_container results( g_allocator );
			dz_bone_data_contact_test_predicate predicate( results, &m_body_parts_filter );

			ASSERT( UNKNOWN_EXPRESSION_T( it->m_rigid_body ) );
			contact_test( it->m_rigid_body, predicate );

			bone_data_container unique_bones( g_allocator );
			std::insert_iterator<bone_data_container> insert_it( unique_bones, unique_bones.begin( ) );
			std::unique_copy( results.begin( ), results.end( ), insert_it, compare_bone_data_predicate );

			std::pair<collision::bone_collision_data*, float> const* ub_it = unique_bones.begin( );
			std::pair<collision::bone_collision_data*, float> const* ub_end = unique_bones.end( );
			for ( ; ub_it != ub_end ; ++ub_it )
			{
				if ( ub_it->second <= 1.0f )
				{
					float hit_coeff = m_hit_curve.evaluate( ub_it->second, 0.0f, math::range_time_type, 0.0f, 0.0f );
					math::clamp( hit_coeff, 0.0f, 1.0f );
					float const hit_value = math::lerp( m_min_hit, m_max_hit, hit_coeff );
					float const armor_piercing_value = math::lerp( m_min_armor_piercing, m_max_armor_piercing, hit_coeff );
					it->m_receiver->hit( this, *ub_it->first, m_damage_type.c_str( ), hit_value, armor_piercing_value, NULL );
					it->m_was_hit = true;
				}
			}

			ASSERT( UNKNOWN_EXPRESSION_T( it->m_receiver ) );
			if ( it->m_was_hit && m_owner )
				m_owner->on_zone_act( this, it->m_receiver );
		}

		m_accumulated_hit_time_ms = 0;
	}
}

void damage_zone_core::hit_on_motion_inside( const u32 frame_delta, const u32 current_time )
{
	VOSTOK_UNREFERENCED_PARAMETERS( frame_delta, current_time );

	if ( m_receivers.empty( ) )
		return;

	hit_receiver_info* it = m_receivers.begin( );
	hit_receiver_info* end = m_receivers.end( );
	for ( ; it != end ; ++it )
	{
		it->m_was_hit = false;

		typedef vectora<std::pair<collision::bone_collision_data*, float> > bone_data_container;
		bone_data_container results( g_allocator );
		dz_bone_data_contact_test_predicate predicate( results, &m_body_parts_filter );

		ASSERT( UNKNOWN_EXPRESSION_T( it->m_rigid_body ) );
		contact_test( it->m_rigid_body, predicate );

		bone_data_container unique_bones( g_allocator );
		std::insert_iterator<bone_data_container> insert_it( unique_bones, unique_bones.begin( ) );
		std::unique_copy( results.begin( ), results.end( ), insert_it, compare_bone_data_predicate );

		std::pair<collision::bone_collision_data*, float> const* ub_it = unique_bones.begin( );
		std::pair<collision::bone_collision_data*, float> const* ub_end = unique_bones.end( );
		for ( ; ub_it != ub_end ; ++ub_it )
		{
			if ( ub_it->second <= 1.0f )
			{
				float on_bound_hit = m_motion_on_bound_curve.evaluate( it->m_receiver->get_speed( ), 0.0f, math::range_time_type, 0.0f, 0.0f );
				float on_center_hit = m_motion_on_center_curve.evaluate( it->m_receiver->get_speed( ), 0.0f, math::range_time_type, 0.0f, 0.0f );
				float const hit_val = math::lerp( m_min_hit, m_max_hit, ( on_bound_hit + on_center_hit ) / 2.0f );
				it->m_receiver->hit( this, *ub_it->first, m_damage_type.c_str( ), hit_val, m_min_armor_piercing, NULL );
				it->m_was_hit = true;
			}
		}

		ASSERT( UNKNOWN_EXPRESSION_T( it->m_receiver ) );
		if ( it->m_was_hit && m_owner )
			m_owner->on_zone_act( this, it->m_receiver );
	}
}

void damage_zone_core::activate( zone_group* owner, physics::world* p_world, scheduler& scheduler )
{
	m_physics_world = p_world;
	collision_sensor::insert( p_world );
	m_owner = owner;
	m_accumulated_hit_time_ms = 0;
	m_scheduler = &scheduler;
	scheduler.register_on_frame( &m_scheduler_identifier, boost::bind( &damage_zone_core::tick, this, _1, _2 ), true );
}

void damage_zone_core::deactivate( )
{
	collision_sensor::remove( );
	m_scheduler->unregister( &m_scheduler_identifier );

	m_scheduler = NULL;

	if ( m_owner )
	{
		hit_receiver_info* it	= m_receivers.begin( );
		hit_receiver_info* end	= m_receivers.end( );
		for ( ; it != end ; ++it )
			it->m_receiver->unsubscribe_from_actions( this );
	}
	m_receivers.clear( );
	m_owner = NULL;
}

void damage_zone_core::on_player_action(
	hit_receiver const*						receiver,
	player_actions_subscriber::action		action,
	float									param
)
{
	m_owner->core( )->on_player_action( receiver, action, param );
}

} // namespace survarium
