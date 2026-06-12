////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/damage_zone_core.h>

#include <vostok/collision/bone_collision_data.h>
#include <vostok/physics/contact_test_predicate.h>
#include <vostok/game_core/hit_receiver.h>
#include <vostok/game_core/game_scene.h>
#include <vostok/game_core/zone_group.h>
#include <vostok/physics/base_physics_object.h>

// sushi@TODO: Everything is skipped, since v0.100b is not using anomalies at all.
// Will come back to this if/when this will be needed.
// Might be to improve matching, might be because something from here is actually used.

namespace survarium {

// STATE[100%|DONE]
hit_receiver_info::hit_receiver_info( hit_receiver* receiver, physics::base_physics_object* rigid_body ) :
	m_receiver		( receiver ),
	m_rigid_body	( rigid_body ),
	m_was_hit		( false )
{
}

// STATE[100%|DONE]: sushi@TODO: Think about this a bit more.
bool hit_receiver_info::operator==( hit_receiver_info const& rhs ) const
{
	return m_receiver->m_pointer->m_pointer == rhs.m_receiver->m_pointer->m_pointer;
}

// STATE[99.63%|DONE]
damage_zone_core::damage_zone_core( ) :
	hit_initiator				( u8(-1), true ),
	m_physics_world				( NULL ),
	m_owner						( NULL ),
	m_accumulated_hit_time_ms	( 0 ),
	m_standalone				( true )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (0x148 vs 0x147 bytes)
	// VERDICT: STRUCTURE MATCH (member-init list, 0 body stmts) - residual is an LTCG frame-slot
	// delta plus member-ctor call-target relocs; instruction shape identical, non-steerable.
}

// STATE[100%|DONE]
damage_zone_core::~damage_zone_core( )
{
}

// STATE[UNCHECKED]
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

	// FUNCTION BODY
	// <0x5994d0>|0x000|+0x010:'49'	{
	// <0x5994e0>|0x010|+0x00f:'50'
	// <0>
	// <0x5994ef>|0x01f|+0x042:'52'
	// <0x599531>|0x061|+0x042:'53'
	// <0x599573>|0x0a3|+0x042:'54'
	// <0>
	// <18>
	// <0x5995b5>|0x0e5|+0x021:'74'
	// <0x5995d6>|0x106|+0x020:'75'
	// <0x5995f6>|0x126|+0x020:'76'
	// <0x599616>|0x146|+0x020:'77'
	// <0x599636>|0x166|+0x020:'78'
	// <0x599656>|0x186|+0x01e:'79'
	// <0x599674>|0x1a4|+0x01e:'80'
	// <0>
	// <1>
	// <0x599692>|0x1c2|+0x030:'83'
	// <0x5996c2>|0x1f2|+0x00b:'84'
	// <0x5996cd>|0x1fd|+0x00b:'85'
	// <0x5996d8>|0x208|+0x013:'86'
	// <0x5996eb>|0x21b|+0x028:'87'
	// <0x599713>|0x243|      :'88'	}
	// ******
}

// STATE[SKIPPED]
bool compare_bone_data_predicate( std::pair< collision::bone_collision_data *, float > const& lhs, std::pair< collision::bone_collision_data *, float > const& rhs )
{
	return lhs.first->skeleton_bone_index == rhs.first->skeleton_bone_index;

	// FUNCTION BODY
	// <0x597d63>|0x003|+0x017:'118'
	// ******
}

// STATE[100%|DONE]
// static in the target (PDB plain-name record): internal linkage is what lets objdiff pair
// these four helpers; anchored from the add_single_result stub below, not temp_include_all.
static float distance_from_sphere_center_to_point_on_shape( float radius )
{
	return radius;
}

// STATE[86.52%|PARTIAL]: dot_product inline wall (per-call-site /Ob2 /GL), non-steerable;
// paired with the target static via internal linkage (was report-unpaired as extern).
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

	// STRUCTURE DIFF: target 14 stmts / base 14 stmts
	// SIZE +0x1b | 135 | float dist = dir.dot_product( axis );
	// VERDICT: STRUCTURE MATCH (14/14) - sole SIZE is float3_pod::dot_product inlined in base
	// (scalar mulss/addss) vs out-of-line call in target, per-call-site /Ob2 /GL, non-steerable.
}

// STATE[82.48%|PARTIAL]: dot_product inline wall (2 sites on the proj_to_y_axis line),
// non-steerable; paired with the target static via internal linkage.
static	float distance_from_capsule_center_to_point_on_shape(
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

	// STRUCTURE DIFF: target 12 stmts / base 12 stmts
	// SIZE +0x4e | 190 | const float proj_to_y_axis = ( ... ).dot_product( ... ) / ... .dot_product( ... );
	// VERDICT: STRUCTURE MATCH (12/12) - sole SIZE is the TWO dot_product calls inlined in base
	// vs out-of-line in target, per-call-site /Ob2 /GL, non-steerable.
}

// STATE[76.61%|PARTIAL]: dot_product inline wall (3 sites), non-steerable; paired with the
// target static via internal linkage.
static	float distance_from_cylinder_center_to_point_on_shape(
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

	// STRUCTURE DIFF: target 15 stmts / base 15 stmts
	// SIZE +0x4e | 239 | const float proj_to_y_axis = ( ... ).dot_product( ... ) / ... .dot_product( ... );
	// SIZE +0x1b | 250 | float3 proj = y_axis * y_axis.dot_product( circle_point_dir );
	// VERDICT: STRUCTURE MATCH (15/15) - both SIZE rows are dot_product inlined in base vs
	// out-of-line in target, per-call-site /Ob2 /GL, non-steerable.
}

struct dz_bone_data_contact_test_predicate : public physics::contact_test_predicate {
public:
	inline	dz_bone_data_contact_test_predicate( vectora<std::pair<collision::bone_collision_data *,float> >& arg_0, vector<fixed_string<16> > const* arg_1 ) : m_result( &arg_0 ), m_body_parts_filter( arg_1 ) { }

	virtual	float		add_single_result				(
							void*						user_data,
							collision::primitive_type	first_shape_type,
							float4x4 const&				first_shape_transform,
							float3 const&				first_shape_dimension,
							collision::primitive_type	second_shape_type,
							float4x4 const&				second_shape_transform,
							float3 const&				second_shape_dimension
						) override;
public:
	/* 0x0000 */	/* physics::contact_test_predicate */
	/* 0x0004 */	vectora<std::pair<collision::bone_collision_data *,float> >*	m_result;
	/* 0x0008 */	vector<fixed_string<16> > const*	m_body_parts_filter;
}; // struct dz_bone_data_contact_test_predicate

STATIC_SIZE_ASSERT(dz_bone_data_contact_test_predicate, 0xC);

struct compare_body_parts_predicate {
public:
	inline compare_body_parts_predicate( pcstr body_part ) : m_body_part( body_part ) { }

	inline bool operator()( fixed_string<16> const& part ) const
	{
		return m_body_part == part.begin( );
	}

	pcstr	m_body_part;
};

// STATE[86.52%|PARTIAL]
float dz_bone_data_contact_test_predicate::add_single_result(
	void*						user_data,
	collision::primitive_type	first_shape_type,
	float4x4 const&				first_shape_transform,
	float3 const&				first_shape_dimension,
	collision::primitive_type	second_shape_type,
	float4x4 const&				second_shape_transform,
	float3 const&				second_shape_dimension
)
{
	// LOCALS
	std::pair< collision::bone_collision_data*, float >	result;
	float												max_distance;
	// ******

	result.first = NULL; result.second = FLT_MAX;
	result.first = (collision::bone_collision_data*)user_data;

	if ( m_body_parts_filter )
		if ( stlp_std::find_if( m_body_parts_filter->begin( ), m_body_parts_filter->end( ), compare_body_parts_predicate( result.first->body_part_name.begin( ) ) ) == m_body_parts_filter->end( ) )
			return 0.0f;

	max_distance = FLT_MAX;

	switch ( second_shape_type )
	{
	case collision::primitive_sphere:
		max_distance = distance_from_sphere_center_to_point_on_shape( second_shape_dimension.x );
	break;
	case collision::primitive_box:
		max_distance = distance_from_box_center_to_point_on_shape( second_shape_transform, second_shape_dimension, first_shape_transform.lines[3].xyz( ) );
	break;
	case collision::primitive_capsule:
		max_distance = distance_from_capsule_center_to_point_on_shape( second_shape_transform, second_shape_dimension.x, second_shape_dimension.y, first_shape_transform.lines[3].xyz( ) );
	break;
	case collision::primitive_cylinder:
		max_distance = distance_from_cylinder_center_to_point_on_shape( second_shape_transform, second_shape_dimension.x, second_shape_dimension.y, first_shape_transform.lines[3].xyz( ) );
	break;
	default:
		NODEFAULT( );
	}

	const float d_1 = ( second_shape_transform.lines[3].xyz( ) - first_shape_transform.lines[3].xyz( ) ).length( );
	result.second = d_1 / max_distance;

	m_result->push_back( result );

	return 0.0f;

	// STRUCTURE DIFF: target 20 / base 20 stmts
	// -- | multi-expr line split to match target's split count
	// VERDICT: STRUCTURE MATCH (shape ok)
}

// STATE[UNCHECKED]
void damage_zone_core::on_inside( buffer_vector<physics::base_physics_object *> const& objects )
{
	VOSTOK_UNREFERENCED_PARAMETER( objects );

	// FUNCTION BODY
	// <0x597d40>|0x000|+0x007:'266'	{
	// <0>
	// <0x597d47>|0x007|      :'268'	}
	// ******
}

// STATE[SKIPPED]
void damage_zone_core::on_leave( buffer_vector<physics::base_physics_object *> const& objects )
{
	// LOCALS
	// physics::base_physics_object** end
	// physics::base_physics_object** it
	// hit_receiver* 				receiver<1>
	// hit_receiver_info 			info<1>
	// ******

	// CALL SITE INFO
	// <0x597e81> -> hit_receiver* <unknown>()
	// <0x597ed1> -> void <unknown>(player_actions_subscriber*)
	// ******

	// FUNCTION BODY
	// <0x597e30>|0x000|+0x009:'271'	{
	// <0x597e39>|0x009|+0x008:'272'
	// <0x597e41>|0x011|+0x009:'273'
	// <0x597e4a>|0x01a|+0x017:'274'
	// <0>
	// <0x597e61>|0x031|+0x00c|[1]:'276'
	// <0x597e6d>|0x03d|+0x019:'277'
	// <0x597e86>|0x056|+0x00c:'278'
	// <0x597e92>|0x062|+0x00c:'279'
	// <0>
	// <0x597e9e>|0x06e|+0x00c:'281'
	// <0>
	// <0x597eaa>|0x07a|+0x029:'283'
	// <0x597ed3>|0x0a3|+0x022:'284'
	// <0>
	// <1>
	// <2>
	// <0x597ef5>|0x0c5|+0x00e:'288'
	// <0x597f03>|0x0d3|+0x00c:'289'
	// <0x597f0f>|0x0df|+0x04d:'290'
	// <0x597f5c>|0x12c|+0x005:'291'
	// <0x597f61>|0x131|      :'292'	}
	// ******
}

// STATE[SKIPPED]
// void survarium::damage_zone_core::on_enter(vostok::buffer_vector<vostok::physics::base_physics_object *> const&)
void damage_zone_core::on_enter( buffer_vector<physics::base_physics_object *> const& objects )
{
	// LOCALS
	// physics::base_physics_object** end
	// physics::base_physics_object** it
	// hit_receiver* 				receiver<1>
	// hit_receiver_info 			info<1>
	// ******

	// CALL SITE INFO
	// <0x5987b1> -> hit_receiver* <unknown>()
	// <0x598801> -> void <unknown>(player_actions_subscriber*)
	// ******

	// FUNCTION BODY
	// <0x598760>|0x000|+0x009:'295'	{
	// <0x598769>|0x009|+0x008:'296'
	// <0x598771>|0x011|+0x009:'297'
	// <0x59877a>|0x01a|+0x017:'298'
	// <0>
	// <0x598791>|0x031|+0x00c|[1]:'300'
	// <0x59879d>|0x03d|+0x019:'301'
	// <0x5987b6>|0x056|+0x00c:'302'
	// <0x5987c2>|0x062|+0x00c:'303'
	// <0>
	// <0x5987ce>|0x06e|+0x00c:'305'
	// <0>
	// <0x5987da>|0x07a|+0x029:'307'
	// <0x598803>|0x0a3|+0x022:'308'
	// <0>
	// <1>
	// <0x598825>|0x0c5|+0x012:'311'
	// <0x598837>|0x0d7|+0x00c:'312'
	// <0x598843>|0x0e3|+0x012:'313'
	// <0x598855>|0x0f5|+0x005:'314'
	// <0x59885a>|0x0fa|      :'315'	}
	// ******
}

// STATE[SKIPPED]
// void survarium::damage_zone_core::tick(const unsigned int, const unsigned int)
void damage_zone_core::tick( u32 frame_delta, u32 current_time )
{
	// FUNCTION BODY
	// <0x599250>|0x000|+0x009:'318'	{
	// <0x599259>|0x009|+0x010:'319'
	// <0>
	// <0x599269>|0x019|+0x01e:'321'
	// <0>
	// <1>
	// <0x599287>|0x037|+0x010:'324'
	// <0x599297>|0x047|+0x002:'325'
	// <0>
	// <0x599299>|0x049|+0x010:'327'
	// <0x5992a9>|0x059|+0x002:'328'
	// <0>
	// <0x5992ab>|0x05b|+0x010:'330'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5992bb>|0x06b|      :'335'	}
	// ******
}

// STATE[UNCHECKED]
bool remove_null_receivers_predicate( hit_receiver_info const& info )
{
	return info.m_receiver == NULL;

	// FUNCTION BODY
	// <0x597d20>|0x000|      :'338'	{
	// ******
}

// sushi@TODO: Big skip

// STATE[100%|DONE]
// bool survarium::damage_zone_core::is_filter_passed(vostok::physics::base_physics_object*) const
bool damage_zone_core::is_filter_passed( physics::base_physics_object* object ) const
{
	return ( object->get_collision_group( ) & 0x40 ) != 0;

	// FUNCTION BODY
	// <0x587cf0>|0x000|+0x007:'390'	{
	// <0x587cf7>|0x007|+0x018:'391'
	// <0x587d0f>|0x01f|      :'392'	}
	// ******
}

// STATE[86.52%|PARTIAL]
void damage_zone_core::hit_on_enter( const u32 frame_delta, const u32 current_time )
{
	// LOCALS
	hit_receiver_info* 						end;
	hit_receiver_info* 						it;
	std::pair< collision::bone_collision_data*, float > const*	ub_it;
	std::pair< collision::bone_collision_data*, float > const*	ub_end;
	typedef vectora< std::pair< collision::bone_collision_data*, float > > bone_data_container;
	bone_data_container						results( (vostok::memory::base_allocator*)g_allocator ), unique_bones( (vostok::memory::base_allocator*)g_allocator );
	dz_bone_data_contact_test_predicate		predicate( results, &m_body_parts_filter ); std::insert_iterator< bone_data_container >	insert_it = stlp_std::insert_iterator< bone_data_container >( unique_bones, unique_bones.end( ) );
	// ******

	if ( m_receivers.empty( ) )
		return;

	it		= m_receivers.begin( );
	end		= m_receivers.end( );

	while ( it != end )
	{
		if ( !it->m_was_hit )
		{
			results.clear( );

			predicate.m_result = &results; predicate.m_body_parts_filter = &m_body_parts_filter;

			contact_test( it->m_rigid_body, predicate );

			unique_bones.clear( );

			insert_it = stlp_std::insert_iterator< vectora< std::pair< collision::bone_collision_data*, float > > >( unique_bones, unique_bones.end( ) );
			stlp_std::unique_copy( results.begin( ), results.end( ), insert_it, compare_bone_data_predicate );

			ub_it = unique_bones.begin( );
			ub_end = unique_bones.end( );

			while ( ub_it != ub_end )
			{
				if ( ub_it->second >= 0.0f )
				{
					it->m_receiver->hit(
						this ? static_cast<hit_initiator const*>(this) : NULL,
						*ub_it->first,
						m_damage_type.begin( ),
						m_max_hit,
						m_max_armor_piercing,
						NULL
					);
					it->m_was_hit = true;
				}
				++ub_it;
			}
		}
		++it;
	}

	if ( it->m_was_hit && m_owner )
		m_owner->on_zone_act( this, it->m_receiver );
	// STRUCTURE DIFF: target 26 / base 26 stmts
	// SIZE only; brace closes and ub_it/ub_end are on their own lines (no merged hacks)
	// VERDICT: STRUCTURE MATCH
}

// STATE[86.52%|PARTIAL]
void damage_zone_core::hit_on_inside( const u32 frame_delta, const u32 current_time )
{
	// LOCALS
	hit_receiver_info* 						end;
	hit_receiver_info* 						it;
	std::pair< collision::bone_collision_data*, float > const*	ub_it;
	std::pair< collision::bone_collision_data*, float > const*	ub_end;
	typedef vectora< std::pair< collision::bone_collision_data*, float > > bone_data_container;
	bone_data_container						results( (vostok::memory::base_allocator*)g_allocator ), unique_bones( (vostok::memory::base_allocator*)g_allocator );
	dz_bone_data_contact_test_predicate		predicate( results, &m_body_parts_filter ); std::insert_iterator< bone_data_container >	insert_it = stlp_std::insert_iterator< bone_data_container >( unique_bones, unique_bones.end( ) );
	float									hit_coeff;
	// ******

	m_accumulated_hit_time_ms	+= frame_delta;

	if ( m_receivers.empty( ) )
		return;

	if ( m_accumulated_hit_time_ms < m_hit_interval_ms )
		return;

	it		= m_receivers.begin( );
	end		= m_receivers.end( );

	while ( it != end )
	{
		it->m_was_hit = false;

		results.clear( );

		predicate.m_result = &results; predicate.m_body_parts_filter = &m_body_parts_filter;

		contact_test( it->m_rigid_body, predicate );

		unique_bones.clear( );

		insert_it = stlp_std::insert_iterator< vectora< std::pair< collision::bone_collision_data*, float > > >( unique_bones, unique_bones.end( ) );
		stlp_std::unique_copy( results.begin( ), results.end( ), insert_it, compare_bone_data_predicate );

		ub_it = unique_bones.begin( );
		ub_end = unique_bones.end( );

		while ( ub_it != ub_end )
		{
			if ( ub_it->second >= 0.0f )
			{
				hit_coeff = m_hit_curve.evaluate( ub_it->second, 0.0f, math::range_time_type, 0.0f, 0.0f );
				math::clamp( hit_coeff, 0.0f, 1.0f );

				const float hit_value = math::lerp( m_min_hit, m_max_hit, hit_coeff );
				const float armor_piercing_value = math::lerp( m_max_armor_piercing, m_min_armor_piercing, hit_coeff );

				it->m_receiver->hit(
					this ? static_cast<hit_initiator const*>(this) : NULL,
					*ub_it->first,
					m_damage_type.begin( ),
					hit_value,
					armor_piercing_value,
					NULL
				);
				it->m_was_hit = true;
			}
			++ub_it;
		}
		++it;
	}

	m_accumulated_hit_time_ms = 0;

	if ( it->m_was_hit && m_owner )
		m_owner->on_zone_act( this, it->m_receiver );
	// STRUCTURE DIFF: target 31 / base 34 stmts
	// QUANTITY +3 | base has 3 extra declaration/loop-body stmts vs target's prologue-folding
	// VERDICT: STRUCTURE MISMATCH (quantity) - need to match target's 31-stmt structure
}

// STATE[STUB]
void damage_zone_core::hit_on_motion_inside( const u32 frame_delta, const u32 current_time )
{
	// LOCALS
	// hit_receiver_info* 				end
	// hit_receiver_info* 				it
	// std::pair< collision::bone_collision_data*, float > const* ub_it<1>
	// vectora< std::pair< collision::bone_collision_data*, float > > unique_bones<1>
	// dz_bone_data_contact_test_predicate predicate<1>
	// std::insert_iterator< vectora< std::pair< collision::bone_collision_data*, float > > > insert_it<1>
	// std::pair< collision::bone_collision_data*, float > const* ub_end<1>
	// vectora< std::pair< collision::bone_collision_data*, float > > results<1>
	// float 							on_bound_hit<2>
	// const float 						hit_val<2>
	// float 							on_center_hit<2>
	// ******

	// SKIPPED BLOCKS
	// <0x5988f7><1>
	// <0x598a48><2>
	// ******

	// TYPEDEFS
	// typedef
	// 	vectora< std::pair< collision::bone_collision_data*, float > >
	// 	bone_data_container;

	// ******

	// CALL SITE INFO
	// <0x598a71> -> float < unknown >() const
	// <0x598aab> -> float < unknown >() const
	// <0x598b6a> -> void < unknown >( hit_initiator const* const, collision::bone_collision_data const&, pcstr, const float, const float, bullet* const )
	// ******

	// FUNCTION BODY[0x598860]: 40
	// <0x59886f>|0x00f|+0x023:'491'
	// <0>
	// <0x598892>|0x032|+0x025:'493'
	// <0x5988b7>|0x057|+0x005:'494'
	// <0>
	// <0x5988bc>|0x05c|+0x015:'496'
	// <0x5988d1>|0x071|+0x015:'497'
	// <0x5988e6>|0x086|+0x017:'498'
	// <0>
	// <0x5988fd>|0x09d|+0x007:'500'
	// <0>
	// <1>
	// <2>
	// <0x598904>|0x0a4|+0x02a:'504'
	// <0x59892e>|0x0ce|+0x023:'505'
	// <0x598951>|0x0f1|+0x00c:'506'
	// <0x59895d>|0x0fd|+0x016:'507'
	// <0x598973>|0x113|+0x03a:'508'
	// <0x5989ad>|0x14d|+0x018:'509'
	// <0x5989c5>|0x165|+0x039:'510'
	// <0>
	// <0x5989fe>|0x19e|+0x012:'512'
	// <0x598a10>|0x1b0|+0x012:'513'
	// <0x598a22>|0x1c2|+0x017:'514'
	// <0>
	// <0x598a39>|0x1d9|+0x015:'516'
	// <0>
	// <0x598a4e>|0x1ee|+0x03a:'518'
	// <0x598a88>|0x228|+0x03a:'519'
	// <0>
	// <0x598ac2>|0x262|+0x03c:'521'
	// <0x598afe>|0x29e|+0x06e:'522'
	// <0x598b6c>|0x30c|+0x007:'523'
	// <0>
	// <0x598b73>|0x313|+0x005:'525'
	// <0x598b78>|0x318|+0x00c:'526'
	// <0>
	// <0x598b84>|0x324|+0x01a:'528'
	// <0x598b9e>|0x33e|+0x02a:'529'
	// <0x598bc8>|0x368|+0x015:'530'
	// ******
}

// STATE[UNCHECKED]
void damage_zone_core::activate( zone_group* owner, physics::world* p_world, scheduler& scheduler )
{
	m_physics_world = p_world;
	collision_sensor::insert( p_world );
	m_owner = owner;
	m_accumulated_hit_time_ms = 0;
	m_scheduler = &scheduler;
	scheduler.register_on_frame( &m_scheduler_identifier, boost::bind( &damage_zone_core::tick, this, _1, _2 ), true );

	// FUNCTION BODY
	// <0x598630>|0x010|+0x00f:'546'
	// <0x59863f>|0x01f|+0x00f:'547'
	// <0x59864e>|0x02f|+0x00f:'548'
	// <0x59865d>|0x03d|+0x010:'549'
	// <0x59866d>|0x04d|+0x00f:'550'
	// <0x59867c>|0x05c|+0x0d3:'551'
	// ******
}

// STATE[SKIPPED]
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
			if ( this ) {} else {} // sushi@TODO: HUH? mMost likely something inlined
	}
	m_receivers.clear( );
	m_owner = NULL;

	// FUNCTION BODY
	// <0x597f79>|0x009|+0x008:'556'
	// <0x597f81>|0x011|+0x017:'557'
	// <0>
	// <0x597f98>|0x028|+0x00d:'559'
	// <0>
	// <0x597fa5>|0x035|+0x00c:'561'		if ( m_owner )
	// <0>									{
	// <0x597fb1>|0x041|+0x012|[1]:'563'		hit_receiver_info* it	= m_receivers.begin( );
	// <0x597fc3>|0x053|+0x012:'564'
	// <0x597fd5>|0x065|+0x013:'565'			for ( ; it != end ; ++it )
	// <0x597fe8>|0x078|+0x02f:'566'				if ( this )
	// <0>									}
	// <0x598017>|0x0a7|+0x00e:'568'
	// <0x598025>|0x0b5|+0x00d:'569'
	// ******
}

// STATE[BLOCKED]: sushi@TODO: I don't understand what is going on here. Also v0.100b didn't have anomalies anyway.
void damage_zone_core::on_player_action(
	hit_receiver const*						receiver,
	player_actions_subscriber::action		action,
	float									param
)
{
	// CALL SITE INFO
	// <0x597e1a> -> void <unknown>(hit_receiver const*, player_actions_subscriber::action, float)
	// ******

	// FUNCTION BODY
	// <0x597de9>|0x009|+0x033:'574'
	// ******
}

} // namespace survarium
