////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/booby_trap_set_core.h>

#include <vostok/game_core/booby_trap_core.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/game_material.h>
#include <vostok/game_core/game_material_manager.h>
#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/game_world_object.h>

#include <vostok/animation/animation_player.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/physics/world.h>
#include <vostok/physics/ray_result.h>
#include <vostok/physics/base_physics_object.h>
#include <vostok/physics/rigid_body_base.h>
#include <vostok/physics/ghost_object.h>

namespace survarium {

// STATE[UNCHECKED]
booby_trap_set_core::booby_trap_set_core( ) :
	inventory_item		( use_silent ),
	m_traps				( NULL, 0 ),
	m_damage_parameters	( NULL, 0 )
{
	// FUNCTION BODY
	// <0x6fe380>|0x000|+0x08e:'28'	{
	// <0x6fe40e>|0x08e|      :'29'	}
	// ******
}
// STATE[77.75%|PARTIAL]
 booby_trap_set_core::~booby_trap_set_core( )
{
	pcvoid damage_parms_buffer = m_damage_parameters.begin( );
	m_damage_parameters.clear( );
	VOSTOK_FREE_IMPL( g_allocator, damage_parms_buffer );

	m_traps.clear( );
	VOSTOK_FREE_IMPL( g_allocator, m_traps_buffer );

	// FUNCTION BODY[0x6fe250]: 6
	// <0x6fe263>|0x013|+0x01e:'33'
	// <0x6fe281>|0x031|+0x037:'34'
	// <0x6fe2b8>|0x068|+0x014:'35'
	// <0>
	// <0x6fe2cc>|0x07c|+0x00e:'37'
	// <0x6fe2da>|0x08a|+0x040:'38'
	// ******
}

// STATE[UNCHECKED]
void booby_trap_set_core::load( configs::binary_config_value const& config )
{
	m_traps.clear( );
	ASSERT( UNKNOWN_EXPRESSION );

	m_traps_buffer = (booby_trap_core_ptr*)VOSTOK_MALLOC_IMPL( g_allocator, sizeof( booby_trap_core_ptr* ) * amount( ), "traps_buffer" );
	m_traps = buffer_vector< booby_trap_core_ptr >( m_traps_buffer, amount( ) );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );

	m_config.max_distance		= (float)config["max_deploy_distance"];
	m_config.max_slope_cos		= math::cos( math::deg2rad( (float)config["max_slope_angle"] ) );

	const float armed_life_time		= (float)config["armed_life_time"];
	m_config.armed_life_time		= math::floor( armed_life_time * 1000.0f );
	const float fired_life_time		= (float)config["fired_life_time"];
	m_config.fired_life_time		= math::floor( fired_life_time * 1000.0f );
	const float disarmed_life_time	= (float)config["disarmed_life_time"];
	m_config.disarmed_life_time		= math::floor( disarmed_life_time * 1000.0f );
	const float defuse_time			= (float)config["defuse_time"];
	m_config.defuse_time			= math::floor( defuse_time * 1000.0f );
	m_config.defuse_by_hit			= (bool)config["defuse_by_hit"];

	m_config.material_can_place_test	= (bool)config["material_can_place_test"];
	m_config.material_can_stick_test	= (bool)config["material_can_stick_test"];

	configs::binary_config_value const&	apply_dmg	= config["damage_parameters"];

	const u32 							count		= apply_dmg.size( );
	m_damage_parameters.clear( );	// sushi@NOTE: Interesting how we didn't free the previous one.
	m_damage_parameters	= buffer_vector< apply_damage >( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( apply_damage ) * count, "damage_parameters" ), count );

	for ( u32 i = 0 ; i < count ; ++i )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		ASSERT( UNKNOWN_EXPRESSION );
		ASSERT( UNKNOWN_EXPRESSION );
		ASSERT( UNKNOWN_EXPRESSION );

		configs::binary_config_value const& conf_entry = apply_dmg[i];
		m_damage_parameters.push_back( apply_damage( ) );
		booby_trap_set_core::apply_damage& ad = m_damage_parameters.back( );

		strings::copy(ad.body_part, 16, (pcstr)conf_entry["body_part"]);
		strings::copy(ad.hit_type,  16, (pcstr)conf_entry["hit_type"]);
		ad.amount			= (float)conf_entry["amount"];
		ad.armor_piercing	= (float)conf_entry["armor_piercing"];

		ASSERT( UNKNOWN_EXPRESSION );
		ASSERT( UNKNOWN_EXPRESSION );
	}

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	// FUNCTION BODY[0x6fdc50]: 74
	// <0>
	// <1>
	// <0x6fdc60>|0x010|+0x029:'45'
	// <0x6fdc89>|0x039|+0x00c:'46'
	// <0x6fdc95>|0x045|+0x05c:'47'
	// <0x6fdcf1>|0x0a1|+0x07a:'48'
	// <0>
	// <1>
	// <0x6fdd6b>|0x11b|+0x00c:'51'
	// <0x6fdd77>|0x127|+0x00c:'52'
	// <0>
	// <0x6fdd83>|0x133|+0x00c:'54'
	// <0x6fdd8f>|0x13f|+0x00c:'55'
	// <0x6fdd9b>|0x14b|+0x00c:'56'
	// <0x6fdda7>|0x157|+0x00c:'57'
	// <0x6fddb3>|0x163|+0x00c:'58'
	// <0>
	// <0x6fddbf>|0x16f|+0x00c:'60'
	// <0x6fddcb>|0x17b|+0x00c:'61'
	// <0>
	// <0x6fddd7>|0x187|+0x00c:'63'
	// <0>
	// <1>
	// <2>
	// <0x6fdde3>|0x193|+0x020:'67'		m_config.max_distance = (float)config["max_deploy_distance"];
	// <0x6fde03>|0x1b3|+0x031:'68'
	// <0>
	// <0x6fde34>|0x1e4|+0x017:'70'
	// <0x6fde4b>|0x1fb|+0x027:'71'
	// <0x6fde72>|0x222|+0x017:'72'
	// <0x6fde89>|0x239|+0x027:'73'
	// <0x6fdeb0>|0x260|+0x017:'74'
	// <0x6fdec7>|0x277|+0x027:'75'
	// <0x6fdeee>|0x29e|+0x017:'76'
	// <0x6fdf05>|0x2b5|+0x027:'77'		m_config.defuse_time = math::floor( (float)config["defuse_time"] * 1000.0f );
	// <0x6fdf2c>|0x2dc|+0x020:'78'
	// <0>
	// <0x6fdf4c>|0x2fc|+0x020:'80'
	// <0x6fdf6c>|0x31c|+0x020:'81'
	// <0>
	// <0x6fdf8c>|0x33c|+0x010:'83'
	// <0>
	// <0x6fdf9c>|0x34c|+0x00b:'85'
	// <0x6fdfa7>|0x357|+0x06b:'86'
	// <0x6fe012>|0x3c2|+0x084:'87'
	// <0>
	// <0x6fe096>|0x446|+0x01e|[1]:'89' for ( u32 i = 0 ; i < count ; ++i )
	// <0>
	// <0x6fe0b4>|0x464|+0x00c:'91'
	// <0x6fe0c0>|0x470|+0x00c:'92'
	// <0x6fe0cc>|0x47c|+0x00c:'93'
	// <0x6fe0d8>|0x488|+0x00c:'94'
	// <0>
	// <0x6fe0e4>|0x494|+0x00e:'96'
	// <0x6fe0f2>|0x4a2|+0x035:'97'		m_damage_parameters.push_back( apply_damage( ) );
	// <0x6fe127>|0x4d7|+0x024:'98'
	// <0>
	// <0x6fe14b>|0x4fb|+0x02b:'100'
	// <0x6fe176>|0x526|+0x02e:'101'
	// <0x6fe1a4>|0x554|+0x01a:'102'
	// <0x6fe1be>|0x56e|+0x01a:'103'
	// <0>
	// <1>
	// <2>
	// <0x6fe1d8>|0x588|+0x00c:'107'
	// <0x6fe1e4>|0x594|+0x00c:'108'
	// <0x6fe1f0>|0x5a0|+0x005:'109'
	// <0>
	// <0x6fe1f5>|0x5a5|+0x00c:'111'
	// <0x6fe201>|0x5b1|+0x00c:'112'
	// <0x6fe20d>|0x5bd|+0x00c:'113'
	// <0x6fe219>|0x5c9|+0x00c:'114'
	// <0x6fe225>|0x5d5|+0x00c:'115'
	// <0x6fe231>|0x5e1|+0x00c:'116'
	// ******
}

// STATE[UNCHECKED]
bool find_free_trap_predicate( booby_trap_core_ptr trap )
{
	return !trap->is_active( );

	// FUNCTION BODY[0x6fdc00]: 1
	// <0x6fdc06>|0x006|+0x039:'121'
	// ******
}

// STATE[100%|DONE]
void booby_trap_set_core::remove_trap( booby_trap_core& trap )
{
	remove_trap_impl( trap );
}

// STATE[UNCHECKED]
booby_trap_core_ptr* booby_trap_set_core::try_place_trap( )
{
	if ( amount( ) == 0 )
		return m_traps.end( );

	float4x4 place_transform;
	if ( !get_visible_place_transform( place_transform ) )
		return m_traps.end( );

	booby_trap_core_ptr* trap_iter = std::find_if( m_traps.begin( ), m_traps.end( ), find_free_trap_predicate );

	if ( trap_iter == m_traps.end( ) )
		return trap_iter; // sushi@TODO

	insert_trap( **trap_iter, place_transform );
	set_amount( amount( ) - 1 );

	return trap_iter;

	// FUNCTION BODY[0x6fe910]: 17
	// <0x6fe919>|0x009|+0x00f:'131'
	// <0x6fe928>|0x018|+0x014:'132'
	// <0>
	// <0x6fe93c>|0x02c|+0x008:'134'
	// <0x6fe944>|0x034|+0x013:'135'
	// <0x6fe957>|0x047|+0x011:'136'
	// <0>
	// <0x6fe968>|0x058|+0x030:'138'
	// <0>
	// <0x6fe998>|0x088|+0x014:'140'	if ( trap_iter == m_traps.end( ) )
	// <0x6fe9ac>|0x09c|+0x005:'141'
	// <0>
	// <0x6fe9b1>|0x0a1|+0x01a:'143'
	// <0>
	// <0x6fe9cb>|0x0bb|+0x016:'145'
	// <0>
	// <0x6fe9e1>|0x0d1|+0x003:'147'
	// ******
}

// STATE[79.71%|PARTIAL]
void booby_trap_set_core::remove_trap_if_active( booby_trap_core_ptr& trap )
{
	ASSERT( UNKNOWN_EXPRESSION );

	if ( trap->is_active( ) )
		remove_trap_impl( *trap );

	// FUNCTION BODY[0x6fd850]: 4
	// <0x6fd859>|0x009|+0x00c:'152'
	// <0>
	// <0x6fd865>|0x015|+0x02a:'154'
	// <0x6fd88f>|0x03f|+0x011:'155'
	// ******
}

// STATE[100%|DONE]
void booby_trap_set_core::remove( )
{
	std::for_each(
		m_traps.begin( ),
		m_traps.end( ),
		boost::bind( &booby_trap_set_core::remove_trap_if_active, this, _1 )
	);
}

// STATE[BLOCKED]: sushi@TODO: Understand what it does exactly
float4x4 create_place_matrix_for_looking_point(
	float3 const&	hit_point,
	float3 const&	normal,
	float4x4 const& head_transform
)
{
	float4x4 result					= math::create_translation( hit_point );

	float3 const& head_forward		= head_transform.k.xyz( );
	float3 const& right_candidate	= normal ^ head_forward;

	if ( right_candidate.length( ) > math::epsilon_3 ) {
		float3 const& right				= math::normalize( right_candidate );
		float3 const& up				= normal;
		float3 const& forward			= math::normalize( right ^ up );
		result.i.xyz( ) = right;
		result.j.xyz( ) = up;
		result.k.xyz( ) = forward;
	} else {
		float3 const& head_right		= head_transform.i.xyz( );
		float3 const& forward_candidate	= head_right ^ normal;
		float3 const& forward			= math::normalize( forward_candidate );
		float3 const& up				= normal;
		float3 const& right				= math::normalize( up ^ forward );
		result.i.xyz( ) = right;
		result.j.xyz( ) = up;
		result.k.xyz( ) = forward;
	}

	return result;

	// FUNCTION BODY[0x6fda10]: 26
	// <0x6fda1b>|0x00b|+0x010:'169'
	// <0>
	// <0x6fda2b>|0x01b|+0x00e:'171'
	// <0x6fda39>|0x029|+0x018:'172'
	// <0>
	// <0x6fda51>|0x041|+0x01a:'174'	if ( right_candidate > math::epsilon_3 )
	// <0x6fda6b>|0x05b|+0x011:'175'		float3 const& right				= math::normalize( right_candidate );
	// <0x6fda7c>|0x06c|+0x006:'176'		float3 const& up				= normal;
	// <0x6fda82>|0x072|+0x025:'177'		float3 const& forward			= math::normalize( right ^ up );
	// <0x6fdaa7>|0x097|+0x01b:'178'
	// <0x6fdac2>|0x0b2|+0x01b:'179'
	// <0x6fdadd>|0x0cd|+0x01b:'180'
	// <0>								}
	// <0x6fdaf8>|0x0e8|+0x005:'182'	else {
	// <0x6fdafd>|0x0ed|+0x00b|[1]:'183'
	// <0x6fdb08>|0x0f8|+0x021:'184'
	// <0x6fdb29>|0x119|+0x012:'185'
	// <0x6fdb3b>|0x12b|+0x01d:'186'
	// <0x6fdb58>|0x148|+0x006:'187'
	// <0x6fdb5e>|0x14e|+0x031:'188'
	// <0x6fdb8f>|0x17f|+0x01e:'189'
	// <0x6fdbad>|0x19d|+0x01b:'190'
	// <0x6fdbc8>|0x1b8|+0x01e:'191'
	// <0>
	// <1>
	// <0x6fdbe6>|0x1d6|+0x010:'194'
	// ******
}

// STATE[UNCHECKED]
bool booby_trap_set_core::get_visible_place_transform( float4x4& result )
{
	physics::world* world = get_inventory( ).holder( ).get_physics_world( );
	ASSERT( UNKNOWN_EXPRESSION_T( world ) );

	base_player* player = get_inventory( ).holder( ).cast_to_base_player( );
	ASSERT( UNKNOWN_EXPRESSION_T( player ) );

	float4x4 const& head_transform = player->get_head_transform( );


	float3		ray_from	= head_transform.c.xyz( );
	float3		ray_dir		= head_transform.k.xyz( );
	const float	ray_length	= config( ).max_distance;

	u16			group		= 1028;
	u16			mask		= 514;
	physics::closest_ray_result ray_result = world->ray_test( ray_from, ray_dir, ray_length, group, mask );

	if ( ray_result.object == NULL )
	{
		result = create_place_matrix_for_looking_point(
			ray_from + ( ray_dir * ray_length ),
			head_transform.j.xyz( ),
			head_transform
		);
		return false;
	}

	float4x4 looking_point_matrix = create_place_matrix_for_looking_point(
		ray_result.hit_point_world,
		ray_result.hit_normal_world,
		head_transform
	);
	const float slope_cos = ray_result.hit_normal_world.y;

	if ( slope_cos >= config( ).max_slope_cos )
	{
		result = looking_point_matrix;
		return false;
	}

	if ( ( ray_result.object->get_collision_group( ) & mask ) == 0 )
	{
		result = looking_point_matrix;
		return false;
	}

	physics::bt_rigid_body_base* body = static_cast_checked< physics::bt_rigid_body_base* >( ray_result.object );
	u16 game_material_id = body->get_triangle_material( ray_result.triangle_index, ray_result.is_shape_index );

	game_material const* material = get_game_material_manager( ).get_material( game_material_id );

	if ( !config( ).material_can_place_test || material->can_place_mine( ) )
	{
		result = looking_point_matrix;
		return false;
	}

	if ( !config( ).material_can_stick_test || material->can_stick_mine( ) )
	{
		result = looking_point_matrix;
		return false;
	}

	float4x4 matrix_a	= looking_point_matrix * math::create_translation( ( -ray_dir ) * 0.005f );
	float3 dir_to_head	= -ray_dir;
	dir_to_head.set_length( 0.5f );
	float4x4 matrix_b	= looking_point_matrix * math::create_translation( dir_to_head );

	ASSERT( UNKNOWN_EXPRESSION );
	booby_trap_core_ptr arbitrary_trap = traps( )[0];

	physics::bt_ghost_object* ghost = arbitrary_trap->get_collision_geometry( 0 )->ghost_object( );

	if ( !world->recover_from_penetrations( ghost->m_shape.c_ptr( ), matrix_a, result, group, mask ) )
	{
		result = looking_point_matrix;
		return false;
	} else {
		return true;
	}

	// FUNCTION BODY[0x6fe420]: 155
	// <0x6fe431>|0x011|+0x02c:'219'
	// <0x6fe45d>|0x03d|+0x012:'220'
	// <0>
	// <0x6fe46f>|0x04f|+0x02c:'222'
	// <0x6fe49b>|0x07b|+0x012:'223'
	// <0>
	// <1>
	// <2>
	// <0x6fe4ad>|0x08d|+0x009:'227'	float4x4 const& head_transform = player->get_head_transform( );
	// <0>
	// <0x6fe4b6>|0x096|+0x025:'229'
	// <0x6fe4db>|0x0bb|+0x025:'230'
	// <0x6fe500>|0x0e0|+0x015:'231'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6fe515>|0x0f5|+0x009:'237'	u16			group		= 1028;
	// <0x6fe51e>|0x0fe|+0x009:'238'	u16			mask		= 514;
	// <0x6fe527>|0x107|+0x033:'239'	physics::closest_ray_result ray_result = world->ray_test( ray_from, ray_dir, ray_length, group, mask );
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6fe55a>|0x13a|+0x009:'248'	if ( ray_result.object == NULL )
	// <0>
	// <1>
	// <0x6fe563>|0x143|+0x05b:'251'
	// <0x6fe5be>|0x19e|+0x007:'252'		return false;
	// <0>
	// <1>
	// <0x6fe5c5>|0x1a5|+0x021:'255'
	// <0>
	// <0x6fe5e6>|0x1c6|+0x010:'257'	const float slope_cos = ray_result.hit_normal_world.y;
	// <0>
	// <0x6fe5f6>|0x1d6|+0x018:'259'	if ( slope_cos >= config( ).max_slope_cos )
	// <0>
	// <1>								{
	// <0x6fe60e>|0x1ee|+0x010:'262'		result = looking_point_matrix;
	// <0x6fe61e>|0x1fe|+0x007:'263'		return false;
	// <0>								}
	// <1>
	// <0x6fe625>|0x205|+0x01c:'266'	if ( ray_result.object->get_collision_group( ) & mask == 0 )
	// <0>
	// <1>
	// <0x6fe641>|0x221|+0x010:'269'
	// <0x6fe651>|0x231|+0x007:'270'		return false;
	// <0>								}
	// <1>
	// <0x6fe658>|0x238|+0x011:'273'	physics::bt_rigid_body_base* body = static_cast_checked< physics::bt_rigid_body_base* >( ray_result.object );
	// <0x6fe669>|0x249|+0x026:'274'
	// <0>
	// <0x6fe68f>|0x26f|+0x028:'276'	game_material const* material = get_game_material_manager( ).get_material( game_material_id );
	// <0>
	// <1>
	// <0x6fe6b7>|0x297|+0x02d:'279'
	// <0>
	// <1>
	// <0x6fe6e4>|0x2c4|+0x010:'282'		result = looking_point_matrix;
	// <0x6fe6f4>|0x2d4|+0x007:'283'		return false;
	// <0>
	// <1>
	// <0x6fe6fb>|0x2db|+0x02d:'286'	if ( !config( ).material_can_stick_test || material->can_stick_mine( ) )
	// <0>
	// <1>
	// <0x6fe728>|0x308|+0x010:'289'		// qmemcpy(result, &looking_point_matrix, sizeof(matrix4x4));
	// <0x6fe738>|0x318|+0x007:'290'		return false;
	// <0>
	// <1>
	// <0x6fe73f>|0x31f|+0x05c:'293'
	// <0x6fe79b>|0x37b|+0x011:'294'
	// <0x6fe7ac>|0x38c|+0x015:'295'
	// <0x6fe7c1>|0x3a1|+0x02d:'296'
	// <0>
	// <0x6fe7ee>|0x3ce|+0x012:'298'	ASSERT( UNKNOWN_EXPRESSION );
	// <0x6fe800>|0x3e0|+0x03f:'299'	booby_trap_core_ptr arbitrary_trap = traps( )[0];
	// <0>
	// <0x6fe83f>|0x41f|+0x046:'301'	physics::bt_ghost_object* ghost = arbitrary_trap->get_collision_geometry( 0 )->ghost_object( );
	// <0>
	// <0x6fe885>|0x465|+0x035:'303'	if ( world->recover_from_penetrations( ghost->m_shape, matrix_a, result, group, mask ) )
	// <0>
	// <0x6fe8ba>|0x49a|+0x010:'305'
	// <0x6fe8ca>|0x4aa|+0x017:'306'
	// <0x6fe8e1>|0x4c1|+0x002:'307'
	// <0>
	// <0x6fe8e3>|0x4c3|+0x017:'309'
	// <0>
	// <63>
	// ******
}

// STATE[100%|DONE]
void booby_trap_set_core::update_bones_matrices(
	animation::skeleton_ptr const&			user_skeleton,
	float4x4* const							user_matrices,
	const u32								user_matrices_count,
	const u32								current_time_in_ms,
	float4x4&								character_head_transform,
	float4x4&								character_transform,
	animation::animation_player const&		animation_player
)
{
	VOSTOK_UNREFERENCED_PARAMETERS(
		user_skeleton,
		user_matrices,
		user_matrices_count,
		current_time_in_ms,
		character_head_transform,
		character_transform,
		&animation_player
	);

	// FUNCTION BODY[0x6fd790]: 9
	// <0>
	// <7>
	// <0x6fd79b>|0x00b|+0x052:'394'
	// ******
}

// STATE[UNCHECKED]
bool trap_is_active( booby_trap_core_ptr const& trap )
{
	return trap->is_active( );

	// FUNCTION BODY[0x6fd820]: 1
	// <0x6fd826>|0x006|+0x025:'399'
	// ******
}

// STATE[PARTIAL]: append the trap's index within this set. ASSERT compiled out.
void booby_trap_set_core::serialize_game_world_object_header( booby_trap_core const& trap, network_core::udp_match_packet& packet ) const
{
	packet.append( trap_index( trap ) );

	// STRUCTURE DIFF[target 0x6ed9d0 | base 0x450610]: target 2 / base 1 stmts
	//   1: 0x009 <0xc> | 0x009 <0x1d> | packet.append( trap_index( trap ) );   SIZE
	//   2: 0x015 <0x18> | --          | L411   ONLY target
	// ; aligned 0, size-diffs 1, quantity-diffs 1, blank-gaps 1
	// VERDICT: STRUCTURE MATCH (shape ok) - single append( trap_index ); SIZE/quantity are LTCG inline of trap_index()+append (target splits the inlined append tail into L411), non-steerable.
}

// STATE[PARTIAL]: read a trap index, resolve the trap and forward its
// deserialize_game_world_object. ASSERTs compiled out.
void booby_trap_set_core::deserialize_game_world_object( network_core::packet_reader& reader )
{
	const u8			trap_index	= reader.r< bool >( );

	booby_trap_core&	trap		= *traps( )[ trap_index ];
	game_world_object&	object		= trap;
	object.deserialize( reader );

	// STRUCTURE DIFF[target 0x6ed960 | base 0x450500]: target 6 / base 4 stmts
	//   1: 0x00a <0xb> | 0x009 <0x20> | const u8			trap_index	= reader.r< bool >( );   SIZE
	//   2: 0x015 <0xc> | 0x029 <0x33> | booby_trap_core&	trap		= *traps( )[ trap_index ];   SIZE
	//   3: 0x021 <0x1b> | --          | L419   ONLY target
	//   4: 0x03c <0xc> | --          | L421   ONLY target
	// ; aligned 2, size-diffs 2, quantity-diffs 2, blank-gaps 2
	// VERDICT: STRUCTURE MATCH (shape ok) - read index, resolve trap, forward deserialize; SIZE/quantity are LTCG inline-vs-call of r<bool>/traps()[]/operator* (target inlines them into L419/L421), non-steerable.
}

// STATE[UNCHECKED]
u8 booby_trap_set_core::trap_index( booby_trap_core const& trap ) const
{
	booby_trap_core_ptr const* trap_iter = std::find( m_traps.begin( ), m_traps.end( ), &trap );
	ASSERT( UNKNOWN_EXPRESSION_T( trap_iter != m_traps.end( ) ) );

	return u8(trap_iter - m_traps.begin( ));

	// FUNCTION BODY[0x6fd710]: 4
	// <0x6fd719>|0x009|+0x035:'429'
	// <0x6fd74e>|0x03e|+0x00c:'430'
	// <0>
	// <0x6fd75a>|0x04a|+0x021:'432'
	// ******
}

// STATE[96.21%|DONE]
void booby_trap_set_core::remove_trap_impl( booby_trap_core& trap )
{
	ASSERT( UNKNOWN_EXPRESSION );

	inventory_holder& holder = get_inventory( ).holder( );
	holder.remove_game_world_object( trap );
	trap.remove( holder.scheduler( ) );

	// FUNCTION BODY[0x6fd6b0]: 5
	// <0x6fd6b9>|0x009|+0x00c:'437'
	// <0>
	// <0x6fd6c5>|0x015|+0x010:'439'
	// <0x6fd6d5>|0x025|+0x011:'440'
	// <0x6fd6e6>|0x036|+0x016:'441'
	// ******
}

// STATE[97.38%|DONE]
void booby_trap_set_core::insert_trap( booby_trap_core& trap, float4x4 const& transform )
{
	ASSERT( UNKNOWN_EXPRESSION );

	inventory_holder& holder = get_inventory( ).holder( );

	physics::world* world = holder.get_physics_world( );
	ASSERT( UNKNOWN_EXPRESSION_T( world ) );

	trap.insert( world, transform, holder.scheduler( ) );
	holder.insert_game_world_object( trap );

	// FUNCTION BODY[0x6fd630]: 9
	// <0x6fd639>|0x009|+0x00c:'446'
	// <0>
	// <0x6fd645>|0x015|+0x010:'448'
	// <0>
	// <0x6fd655>|0x025|+0x010:'450'
	// <0x6fd665>|0x035|+0x00c:'451'
	// <0>
	// <0x6fd671>|0x041|+0x01e:'453'
	// <0x6fd68f>|0x05f|+0x011:'454'
	// ******
}

} // namespace survarium