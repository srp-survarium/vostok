////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

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

// sushi@TODO: Everything is skipped, since v0.100b is not using anomalies at all.
// Will come back to this if/when this will be needed.
// Might be to improve matching, might be because something from here is actually used.

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

// defined further down (after is_filter_passed); forward-declared so the add_single_result
// anchor below can take its address and keep it emitted (target records it static).
static bool remove_null_receivers_predicate( hit_receiver_info const& info );

// static in the target (PDB plain-name record): internal linkage is what lets objdiff pair
// these four helpers; anchored from the add_single_result stub below, not temp_include_all.
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

struct dz_bone_data_contact_test_predicate : public physics::contact_test_predicate {
public:
	inline	dz_bone_data_contact_test_predicate( vectora<std::pair<collision::bone_collision_data *,float> >& arg_0, vector<fixed_string<16> > const* arg_1 ) { /* no source */ }

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

// STATE[STUB]: claude@NOTE: target rva 0xb7be0, 20 stmts. Switches on second_shape_type
// to the four distance_from_*_center helpers above, then result.first=bone,
// result.second=dist/radius, m_result->push_back(result). BLOCKER: the override mangles
// its primitive_type param under physics:: in the target but collision:: here, because the
// base virtual in contact_test_predicate.h still uses collision::primitive_type (its
// sushi@TODO "moved from collision to physics"). Matching requires introducing
// physics::primitive_type and retyping contact_test_predicate::add_single_result -
// a cross-unit change owned by contact_test_predicate's PR. Belongs with the hit_on_*
// block (they instantiate this predicate). The four helpers it calls already pair.
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
	// temp anchor: the real body dispatches to the four static distance_from_* helpers above
	// and the two file-local predicates; keep them emitted (paired against the target statics)
	// until that dispatch and the hit_on_* callers are matched.
	{
		typedef float ( *shape_fn0 )( float );
		typedef float ( *shape_fn1 )( float4x4 const&, float3 const&, float3 const& );
		typedef float ( *shape_fn2 )( float4x4 const&, float, float, float3 const& );
		volatile shape_fn0 p0 = &distance_from_sphere_center_to_point_on_shape;
		volatile shape_fn1 p1 = &distance_from_box_center_to_point_on_shape;
		volatile shape_fn2 p2 = &distance_from_capsule_center_to_point_on_shape;
		volatile shape_fn2 p3 = &distance_from_cylinder_center_to_point_on_shape;
		(void)p0; (void)p1; (void)p2; (void)p3;

		// the two file-local predicates are likewise only referenced from the hit_on_*
		// stubs; keep them emitted (paired against the target statics) until those match.
		typedef bool ( *cmp_fn )( std::pair< collision::bone_collision_data *, float > const&, std::pair< collision::bone_collision_data *, float > const& );
		typedef bool ( *null_fn )( hit_receiver_info const& );
		volatile cmp_fn  q0 = &compare_bone_data_predicate;
		volatile null_fn q1 = &remove_null_receivers_predicate;
		(void)q0; (void)q1;
	}

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

void damage_zone_core::tick( u32 frame_delta, u32 current_time )
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

// STATE[STUB]: claude@NOTE: target rva 0x588f80, 26 stmts. Per-receiver: build a
// dz_bone_data_contact_test_predicate, run a contact test filling a
// vectora<pair<bone_collision_data*,float>> results, copy/sort into unique_bones via
// insert_iterator + compare_bone_data_predicate, then for each unique bone call
// hit_receiver::hit(hit_initiator, bone_data, damage_type, amount, armor_piercing, bullet).
// BLOCKER: needs dz_bone_data_contact_test_predicate::add_single_result matched first
// (blocked on physics::primitive_type, see above) plus a full vectora/insert_iterator/sort
// STL reconstruction. Parked as a coherent hit_on_* block.
void damage_zone_core::hit_on_enter( const u32 frame_delta, const u32 current_time )
{
}

// STATE[STUB]: claude@NOTE: target rva 0x588bf0, 47 stmts. Like hit_on_enter but only
// hits receivers already in m_receivers and folds in hit_value/armor_piercing_value via
// m_hit_curve plus a hit_coeff. Same BLOCKER (predicate vtable/add_single_result +
// physics::primitive_type + STL). Parked with the hit_on_* block.
void damage_zone_core::hit_on_inside( const u32 frame_delta, const u32 current_time )
{
}

// STATE[STUB]: claude@NOTE: target rva 0x588860, 40 stmts. Like hit_on_enter but mixes
// on_bound_hit/on_center_hit from m_motion_on_bound_curve / m_motion_on_center_curve into
// hit_val. Same BLOCKER (predicate vtable/add_single_result + physics::primitive_type +
// STL). Parked with the hit_on_* block.
void damage_zone_core::hit_on_motion_inside( const u32 frame_delta, const u32 current_time )
{
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
