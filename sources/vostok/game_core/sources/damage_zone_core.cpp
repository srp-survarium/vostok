////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/damage_zone_core.h>

#include <vostok/collision/bone_collision_data.h>
#include <vostok/physics/contact_test_predicate.h>
#include <vostok/game_core/hit_receiver.h>
#include <vostok/game_core/zone_group.h>
#include <vostok/physics/base_physics_object.h>
#include <vostok/game_core/game_scene.h>

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

	// STRUCTURE DIFF: target 12 stmts / base 12 stmts
	// SIZE +0x4e | 190 | const float proj_to_y_axis = ( ... ).dot_product( ... ) / ... .dot_product( ... );
	// VERDICT: STRUCTURE MATCH (12/12) - sole SIZE is the TWO dot_product calls inlined in base
	// vs out-of-line in target, per-call-site /Ob2 /GL, non-steerable.
}

// STATE[76.61%|PARTIAL]: dot_product inline wall (3 sites), non-steerable; paired with the
// target static via internal linkage.
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

	// STRUCTURE DIFF: target 15 stmts / base 15 stmts
	// SIZE +0x4e | 239 | const float proj_to_y_axis = ( ... ).dot_product( ... ) / ... .dot_product( ... );
	// SIZE +0x1b | 250 | float3 proj = y_axis * y_axis.dot_product( circle_point_dir );
	// VERDICT: STRUCTURE MATCH (15/15) - both SIZE rows are dot_product inlined in base vs
	// out-of-line in target, per-call-site /Ob2 /GL, non-steerable.
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

// STATE[STUB]: claude@NOTE: dispatches (switch on second_shape_type) to the four
// distance_from_*_center helpers (target rva 0xb7be0), then result.first = bone,
// result.second = dist/radius, m_result->push_back(result). Out of the geometric-
// helper unit's scope: matching it needs the .cpp-local
// dz_bone_data_contact_test_predicate vtable anchored in base (the class is only
// instantiated inside hit_on_enter/inside/motion_inside, themselves STUBs), and a
// full ~0x1fc-byte reconstruction (switch + folded xyz() + STL push_back). The four
// helpers it calls would pair fine; the dispatch body itself does not hit the
// dot_product inline wall. Deferred to a hit_on_* unit that brings the vtable.
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
	// std::pair< collision::bone_collision_data*, float > result
	// const float 						d_1
	// float 							max_distance
	// ******

	// STATICS
	// static < NoType > 				 = <0xc7de0>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xb6cbe }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xb6cde }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xb6d02 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xb6d42 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// temp anchor: the real body dispatches to the four static distance_from_* helpers above;
	// keep them emitted (and paired against the target statics) until that dispatch is matched.
	{
		typedef float ( *shape_fn0 )( float );
		typedef float ( *shape_fn1 )( float4x4 const&, float3 const&, float3 const& );
		typedef float ( *shape_fn2 )( float4x4 const&, float, float, float3 const& );
		volatile shape_fn0 p0 = &distance_from_sphere_center_to_point_on_shape;
		volatile shape_fn1 p1 = &distance_from_box_center_to_point_on_shape;
		volatile shape_fn2 p2 = &distance_from_capsule_center_to_point_on_shape;
		volatile shape_fn2 p3 = &distance_from_cylinder_center_to_point_on_shape;
		(void)p0; (void)p1; (void)p2; (void)p3;
	}

	return 0.0f;

	// FUNCTION BODY[0xc7be0]: 37
	// <0xc7bea>|0x00a|+0x037:'222'
	// <0xc7c21>|0x041|+0x014:'223'
	// <0xc7c35>|0x055|+0x006:'224'
	// <0>
	// <0xc7c3b>|0x05b|+0x009:'226'
	// <0>
	// <0xc7c44>|0x064|+0x056:'228'
	// <0xc7c9a>|0x0ba|+0x007:'229'
	// <0>
	// <1>
	// <0xc7ca1>|0x0c1|+0x00d:'232'
	// <0xc7cae>|0x0ce|+0x010:'233'
	// <0>
	// <1>
	// <0xc7cbe>|0x0de|+0x01b:'236'
	// <0xc7cd9>|0x0f9|+0x005:'237'
	// <0>
	// <1>
	// <0xc7cde>|0x0fe|+0x01f:'240'
	// <0xc7cfd>|0x11d|+0x005:'241'
	// <0>
	// <1>
	// <0xc7d02>|0x122|+0x03e:'244'
	// <0xc7d40>|0x160|+0x002:'245'
	// <0>
	// <1>
	// <0xc7d42>|0x162|+0x03e:'248'
	// <0xc7d80>|0x1a0|+0x002:'249'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0xc7d82>|0x1a2|+0x030:'255'
	// <0xc7db2>|0x1d2|+0x00f:'256'
	// <0xc7dc1>|0x1e1|+0x015:'257'
	// <0xc7dd6>|0x1f6|+0x002:'258'
	// ******
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
bool damage_zone_core::is_filter_passed( physics::base_physics_object* object ) const
{
	return ( object->get_collision_group( ) & 0x40 ) != 0;
}

// STATE[STUB]
void damage_zone_core::hit_on_enter( const u32 frame_delta, const u32 current_time )
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
	// ******

	// SKIPPED BLOCKS
	// <0x599017><1>
	// ******

	// TYPEDEFS
	// typedef
	// 	vectora< std::pair< collision::bone_collision_data*, float > >
	// 	bone_data_container;

	// ******

	// CALL SITE INFO
	// <0x5991d4> -> void < unknown >( hit_initiator const* const, collision::bone_collision_data const&, pcstr, const float, const float, bullet* const )
	// ******

	// FUNCTION BODY[0x598f80]: 40
	// <0x598f8f>|0x00f|+0x023:'396'
	// <0>
	// <0x598fb2>|0x032|+0x025:'398'
	// <0x598fd7>|0x057|+0x005:'399'
	// <0>
	// <0x598fdc>|0x05c|+0x015:'401'
	// <0x598ff1>|0x071|+0x015:'402'
	// <0x599006>|0x086|+0x017:'403'
	// <0>
	// <0x59901d>|0x09d|+0x00b:'405'
	// <0x599028>|0x0a8|+0x002:'406'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x59902a>|0x0aa|+0x02b:'411'
	// <0x599055>|0x0d5|+0x022:'412'
	// <0x599077>|0x0f7|+0x00c:'413'
	// <0x599083>|0x103|+0x016:'414'
	// <0x599099>|0x119|+0x02b:'415'
	// <0x5990c4>|0x144|+0x018:'416'
	// <0x5990dc>|0x15c|+0x039:'417'
	// <0>
	// <0x599115>|0x195|+0x012:'419'
	// <0x599127>|0x1a7|+0x012:'420'
	// <0>
	// <0x599139>|0x1b9|+0x017:'422'
	// <0>
	// <0x599150>|0x1d0|+0x011:'424'
	// <0>
	// <0x599161>|0x1e1|+0x075:'426'
	// <0x5991d6>|0x256|+0x007:'427'
	// <0>
	// <0x5991dd>|0x25d|+0x005:'429'
	// <0>
	// <0x5991e2>|0x262|+0x00c:'431'
	// <0>
	// <0x5991ee>|0x26e|+0x01a:'433'
	// <0x599208>|0x288|+0x02a:'434'
	// <0x599232>|0x2b2|+0x015:'435'
	// ******
}

// STATE[STUB]
void damage_zone_core::hit_on_inside( const u32 frame_delta, const u32 current_time )
{
	// LOCALS
	// hit_receiver_info* 				end<1>
	// hit_receiver_info* 				it<1>
	// std::pair< collision::bone_collision_data*, float > const* ub_it<2>
	// vectora< std::pair< collision::bone_collision_data*, float > > unique_bones<2>
	// dz_bone_data_contact_test_predicate predicate<2>
	// std::insert_iterator< vectora< std::pair< collision::bone_collision_data*, float > > > insert_it<2>
	// std::pair< collision::bone_collision_data*, float > const* ub_end<2>
	// vectora< std::pair< collision::bone_collision_data*, float > > results<2>
	// const float 						hit_value<3>
	// const float 						armor_piercing_value<3>
	// float 							hit_coeff<3>
	// ******

	// SKIPPED BLOCKS
	// <0x598c5c><1>
	// <0x598c9d><2>
	// <0x598dee><3>
	// ******

	// TYPEDEFS
	// typedef
	// 	vectora< std::pair< collision::bone_collision_data*, float > >
	// 	bone_data_container;

	// ******

	// CALL SITE INFO
	// <0x598ef1> -> void < unknown >( hit_initiator const* const, collision::bone_collision_data const&, pcstr, const float, const float, bullet* const )
	// ******

	// FUNCTION BODY[0x598bf0]: 47
	// <0>
	// <1>
	// <0x598bff>|0x00f|+0x01b:'442'
	// <0>
	// <0x598c1a>|0x02a|+0x025:'444'
	// <0x598c3f>|0x04f|+0x005:'445'
	// <0>
	// <0x598c44>|0x054|+0x01e:'447'
	// <0>
	// <0x598c62>|0x072|+0x015:'449'
	// <0x598c77>|0x087|+0x015:'450'
	// <0x598c8c>|0x09c|+0x017:'451'
	// <0>
	// <0x598ca3>|0x0b3|+0x007:'453'
	// <0>
	// <1>
	// <2>
	// <0x598caa>|0x0ba|+0x02a:'457'
	// <0x598cd4>|0x0e4|+0x023:'458'
	// <0x598cf7>|0x107|+0x00c:'459'
	// <0x598d03>|0x113|+0x016:'460'
	// <0x598d19>|0x129|+0x03a:'461'
	// <0x598d53>|0x163|+0x018:'462'
	// <0x598d6b>|0x17b|+0x039:'463'
	// <0>
	// <0x598da4>|0x1b4|+0x012:'465'
	// <0x598db6>|0x1c6|+0x012:'466'
	// <0x598dc8>|0x1d8|+0x017:'467'
	// <0>
	// <0x598ddf>|0x1ef|+0x015:'469'
	// <0>
	// <0x598df4>|0x204|+0x02f:'471'
	// <0x598e23>|0x233|+0x016:'472'
	// <0x598e39>|0x249|+0x02a:'473'
	// <0x598e63>|0x273|+0x02b:'474'
	// <0x598e8e>|0x29e|+0x065:'475'
	// <0x598ef3>|0x303|+0x007:'476'
	// <0>
	// <0x598efa>|0x30a|+0x005:'478'
	// <0>
	// <0x598eff>|0x30f|+0x00c:'480'
	// <0x598f0b>|0x31b|+0x01a:'481'
	// <0x598f25>|0x335|+0x02a:'482'
	// <0>
	// <0x598f4f>|0x35f|+0x015:'484'
	// <0x598f64>|0x374|+0x010:'485'
	// <0>
	// ******
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
	// <0x59864e>|0x02e|+0x00f:'548'
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

