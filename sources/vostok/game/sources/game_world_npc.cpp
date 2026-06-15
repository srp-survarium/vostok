////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_world.h"

#include <vostok/collision/common_types.h>	// ray_triangle_result (ray_query_predicate)
#include <vostok/physics/ray_result.h>	// closest_ray_result (get_first_npc... predicate)

namespace survarium {

// STATE[STUB]
void game_world::initialize_ai( )
{
	// FUNCTION BODY[0x5ce850]: 5
	// <0x5ce850>|0x000|+0x000:'26'	{
	// <0>
	// <1>
	// <2>
	// <0x5ce850>|0x000|+0x01a:'30'
	// <0x5ce86a>|0x01a|-0x001:'30'
	// <0>
	// <0x5ce869>|0x019|+0x012:'32'
	// <0x5ce87b>|0x02b|      :'32'	}
	// ******
}

// STATE[STUB]
void game_world::initialize_ai_navigation( )
{
	// FUNCTION BODY[0x5ce580]: 3
	// <0>
	// <0x5ce580>|0x000|+0x031:'37'
	// <0>
	// ******
}

// STATE[STUB]
void game_world::get_colliding_objects( math::aabb const& query_aabb, vectora< ai::game_object const* >& results )
{
	// FUNCTION BODY[0x5ce4f0]: 8
	// <0x5ce4f0>|0x000|+0x000:'42'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5ce4f0>|0x000|      :'51'	}
	// ******
}

// STATE[STUB]
void game_world::get_visible_objects(
	math::cuboid const&		cuboid,
	boost::function< void( ai::game_object const& ) > const&	update_callback
)
{
	// FUNCTION BODY[0x5ce4e0]: 3
	// <0x5ce4e0>|0x000|+0x000:'54'	{
	// <0>
	// <1>
	// <2>
	// <0x5ce4e0>|0x000|      :'58'	}
	// ******
}

// TU-local (canonical headers/get_first_npc_in_camera_direction_predicate.h;
// legacy lineage game_unused.cpp::get_first_npc_in_camera_direction - the new
// shape operates on physics::closest_ray_result; consumer is game_world.h's
// inline find_npc_in_camera_direction)
struct get_first_npc_in_camera_direction_predicate : public boost::noncopyable {
	inline			get_first_npc_in_camera_direction_predicate( ) { /* no source */ }

	inline	bool	operator()	( physics::closest_ray_result const& arg_0 ) { /* no source */ return false; }

	inline			~get_first_npc_in_camera_direction_predicate( ) { /* no source */ }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	human_npc*		first_npc;
}; // struct get_first_npc_in_camera_direction_predicate

STATIC_SIZE_ASSERT(get_first_npc_in_camera_direction_predicate, 0x4);

// TU-local (canonical headers/ray_query_predicate.h; legacy lineage
// game.cpp/game_unused.cpp::ray_query - this compiland carries that family)
struct ray_query_predicate : public boost::noncopyable {
	inline			ray_query_predicate	(
						float&								arg_0,
						collision::object const* const		arg_1,
						collision::object const* const		arg_2,
						const float							arg_3
					) :
		visibility_value( arg_0 ),
		requested_object( arg_1 ),
		object_to_ignore( arg_2 ),
		transparency_threshold( arg_3 ) { /* no source */ }

	inline	bool	predicate			( collision::ray_triangle_result const& arg_0 ) { /* no source */ return false; }

	inline			~ray_query_predicate( ) { /* no source */ }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	float&								visibility_value;
	/* 0x0004 */	collision::object const* const		requested_object;
	/* 0x0008 */	collision::object const* const		object_to_ignore;
	/* 0x000c */	const float							transparency_threshold;
	/* 0x0010 */	bool								requested_object_was_found;
}; // struct ray_query_predicate

STATIC_SIZE_ASSERT(ray_query_predicate, 0x14);

// STATE[STUB]
bool game_world::ray_query(
	ai::collision_object const* const		object_to_pick,
	ai::collision_object const* const		object_to_ignore,
	float3 const&							origin,
	float3 const&							direction,
	const float								max_distance,
	const float								transparency_threshold,
	float&									visibility_value
) const
{
	// LOCALS
	// vectora< physics::closest_ray_result > game_objects
	// ******

	// CALL SITE INFO
	// <0x5ce55a> -> void < unknown >( float3 const&, float3 const&, const float, vectora< physics::closest_ray_result >&, u16, u16 )
	// ******

	return false;

	// FUNCTION BODY[0x5ce510]: 18
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5ce513>|0x003|+0x005:'128'
	// <0x5ce518>|0x008|+0x044:'129'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5ce55c>|0x04c|+0x015:'138'
	// ******
}

// STATE[STUB]
void game_world::draw_frustum(
	float				fov_in_radians,
	float				far_plane_distance,
	float				aspect_ratio,
	float3 const&		position,
	float3 const&		direction,
	math::color			color
) const
{
	// FUNCTION BODY[0x5ce880]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x5ce883>|0x003|+0x075:'160'
	// ******
}

// STATE[STUB]
void game_world::draw_ray( float3 const& start_point, float3 const& end_point, bool sees_something ) const
{
	// FUNCTION BODY[0x5ce900]: 1
	// <0x5ce900>|0x000|+0x040:'165'
	// ******
}

// STATE[STUB]
void game_world::get_available_weapons( ai::npc* owner, vectora< ai::weapon* >& list_to_be_filled ) const
{
	// FUNCTION BODY[0x5ce500]: 2
	// <0>
	// <0x5ce500>|0x000|+0x00d:'171'
	// ******
}

// STATE[STUB]
u32 game_world::get_node_by_name( pcstr node_name ) const
{
	return 0;

	// FUNCTION BODY[0x5ce770]: 9
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5ce779>|0x009|+0x0c7:'183'
	// <0>
	// ******
}

// STATE[STUB]
void delete_weapons( human_npc_ptr& owner )
{
	// CALL SITE INFO
	// <0x5ce729> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x5ce6c0]: 2
	// <0x5ce6d0>|0x010|+0x06f:'286'
	// <0x5ce73f>|0x07f|-0x030:'286'
	// <0x5ce70f>|0x04f|+0x037:'287'
	// ******
}

// STATE[STUB]
void game_world::kill_npc( human_npc_ptr& condemned )
{
	// CALL SITE INFO
	// <0x5ce760> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5ce750]: 1
	// <0x5ce750>|0x000|+0x006:'292'
	// ******
}

// STATE[STUB]
void game_world::on_behaviour_created( resources::queries_result& data )
{
	// LOCALS
	// resources::unmanaged_resource_ptr new_behaviour
	// ******

	// FUNCTION BODY[0x5ce640]: 6
	// <0>
	// <1>
	// <0x5ce641>|0x001|+0x01e:'330'
	// <0>
	// <0x5ce65f>|0x01f|+0x012:'332'
	// <0x5ce671>|0x031|+0x01e:'333'
	// ******
}

// STATE[STUB]
void game_world::on_npc_attributes_received( configs::binary_config_value const& attributes_config, human_npc_ptr owner )
{
	// LOCALS
	// human_npc::npc_game_attributes 	attributes
	// configs::binary_config_value const* it_end
	// float3 							color
	// ******

	// CALL SITE INFO
	// <0x5ced82> -> pcstr < unknown >( const u32, const u32 ) const
	// ******

	// FUNCTION BODY[0x5ceaa0]: 29
	// <0x5ceaaa>|0x00a|+0x00b:'338'
	// <0x5ceab5>|0x015|+0x00e:'339'
	// <0x5ceac3>|0x023|+0x01c:'340'
	// <0x5ceadf>|0x03f|+0x00e:'341'
	// <0x5ceaed>|0x04d|+0x013:'342'
	// <0x5ceb00>|0x060|+0x029:'343'
	// <0x5ceb29>|0x089|+0x09f:'344'
	// <0x5cebc8>|0x128|+0x025:'345'
	// <0x5cebed>|0x14d|+0x05d:'346'
	// <0x5cec4a>|0x1aa|+0x01b:'347'
	// <0x5cec65>|0x1c5|+0x026:'348'
	// <0x5cec8b>|0x1eb|+0x01b:'349'
	// <0x5ceca6>|0x206|+0x05f:'350'
	// <0x5ced05>|0x265|+0x00e:'351'
	// <0x5ced13>|0x273|+0x013:'352'
	// <0>
	// <0x5ced26>|0x286|+0x002:'354'
	// <0x5ced28>|0x288|+0x00e:'355'
	// <0>
	// <0x5ced36>|0x296|+0x008:'357'
	// <0>
	// <1>
	// <0x5ced3e>|0x29e|+0x00c:'360'
	// <0x5ced4a>|0x2aa|+0x002:'361'
	// <0x5ced4c>|0x2ac|+0x00c:'362'
	// <0x5ced58>|0x2b8|+0x082:'363'
	// <0>
	// <1>
	// <0x5cedda>|0x33a|+0x05e:'366'
	// ******
}

// STATE[STUB]
void game_world::finish_npc_creation( human_npc_ptr& new_npc, human_npc::npc_game_attributes& attributes )
{
	// FUNCTION BODY[0x5cea60]: 3
	// <0x5cea64>|0x004|+0x007:'371'
	// <0x5cea6b>|0x00b|+0x007:'372'
	// <0x5cea72>|0x012|+0x01d:'373'
	// ******
}

// STATE[STUB]
void game_world::query_npc_dictionary( )
{
	// FUNCTION BODY[0x5ce4d0]: 6
	// <0x5ce4d0>|0x000|+0x000:'377'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5ce4d0>|0x000|      :'384'	}
	// ******
}

// STATE[STUB]
void game_world::tick_npcs( const u32 current_frame_id, const bool is_game_paused )
{
	// LOCALS
	// human_npc_ptr 					it_npc
	// ******

	// FUNCTION BODY[0x5ce950]: 2
	// <0x5ce958>|0x008|+0x0b9:'403'
	// <0x5cea11>|0x0c1|-0x087:'403'
	// <0x5ce98a>|0x03a|+0x0b2:'404'
	// <0x5cea3c>|0x0ec|+0x01e:'404'
	// ******
}

// STATE[STUB]
void game_world::update_npc_stats( )
{
	// CALL SITE INFO
	// <0x5ce604> -> ui::world& < unknown >()
	// <0x5ce60d> -> render::ui::renderer& < unknown >()
	// ******

	// FUNCTION BODY[0x5ce5c0]: 8
	// <0x5ce5c0>|0x000|+0x003:'408'	{
	// <0x5ce5c3>|0x003|+0x01b:'409'
	// <0>
	// <0x5ce5de>|0x01e|+0x00a:'411'
	// <0x5ce5e8>|0x028|+0x011:'412'
	// <0x5ce5f9>|0x039|+0x02d:'413'
	// <0>
	// <1>
	// <0x5ce626>|0x066|-0x001:'416'
	// <0x5ce625>|0x065|+0x00c:'417'
	// <0x5ce631>|0x071|      :'417'	}
	// ******
}


} // namespace survarium
