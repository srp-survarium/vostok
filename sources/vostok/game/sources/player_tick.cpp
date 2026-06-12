////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player.h"

namespace survarium {

// TU statics (compiler-generated dynamic initializers / atexit
// destructors); a matcher recovers their types/initializers from the asm.
/*
// STATE[STUB]
void `dynamic initializer for 's_net_max_position_discrepancy_command''( )
{
	// FUNCTION BODY[0x7d8620]
	// <0x7d8620>|0x000|      :'31'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_test_players_random_input_cc''( )
{
	// FUNCTION BODY[0x7d8690]
	// <0x7d8690>|0x000|      :'35'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_local_player_random_input_cc''( )
{
	// FUNCTION BODY[0x7d86e0]
	// <0x7d86e0>|0x000|      :'38'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_print_animations_cc''( )
{
	// FUNCTION BODY[0x7d8730]
	// <0x7d8730>|0x000|      :'42'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_sf_animation_states_cc''( )
{
	// FUNCTION BODY[0x7d8780]
	// <0x7d8780>|0x000|      :'45'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_smooth_linear_speed_command''( )
{
	// FUNCTION BODY[0x7d87d0]
	// <0x7d87d0>|0x000|      :'48'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_smooth_angular_speed_command''( )
{
	// FUNCTION BODY[0x7d8830]
	// <0x7d8830>|0x000|      :'51'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_smooth_pitch_speed_command''( )
{
	// FUNCTION BODY[0x7d8890]
	// <0x7d8890>|0x000|      :'54'	{
	// ******
}
*/

// STATE[STUB]
u32 player::history_lower_bound_index( const u32 time_in_ms ) const
{
	return 0;

	// FUNCTION BODY[0x5d5450]: 9
	// <0x5d5450>|0x000|+0x013:'67'
	// <0>
	// <0x5d5463>|0x013|+0x029:'69'
	// <0x5d548c>|0x03c|-0x01c:'69'
	// <0>
	// <0x5d5470>|0x020|+0x010:'71'
	// <0x5d5480>|0x030|+0x010:'72'
	// <0>
	// <1>
	// <0x5d5490>|0x040|+0x006:'75'
	// ******
}

// STATE[STUB]
void player::remove_oldest_history_item( )
{
	// FUNCTION BODY[0x5d54a0]: 1
	// <0x5d54a0>|0x000|+0x015:'80'
	// ******
}

// STATE[STUB]
void player::remove_oldest_history_items( const u32 new_oldest_time_in_ms )
{
	// FUNCTION BODY[0x5d54c0]: 2
	// <0x5d54c0>|0x000|+0x028:'85'
	// <0x5d54e8>|0x028|+0x023:'86'
	// ******
}

// STATE[STUB]
void player::restore_history_item( client_player_history_item& item )
{
	// FUNCTION BODY[0x5d57b0]: 18
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5d57bf>|0x00f|+0x012:'106'
	// <0x5d57d1>|0x021|+0x009:'107'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5d57da>|0x02a|+0x018:'115'
	// <0>
	// ******
}

// STATE[STUB]
void player::update_history_item(
	client_player_history_item&		item,
	client_player_history_item const* const	next_item,
	server_player_update const&		server_action,
	const u32						server_action_time_in_ms,
	float4x4&						previous_transform,
	bool&							__formal
)
{
	// FUNCTION BODY[0x5d5bd0]: 61
	// <0>
	// <1>
	// <0x5d5be1>|0x011|+0x009:'123'
	// <0>
	// <0x5d5bea>|0x01a|+0x008:'125'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d5bf2>|0x022|+0x046:'134'
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
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <0x5d5c38>|0x068|+0x03e:'152'
	// <0>
	// <1>
	// <0x5d5c76>|0x0a6|+0x018:'155'
	// <0x5d5c8e>|0x0be|+0x003:'156'
	// <0>
	// <1>
	// <0x5d5c91>|0x0c1|+0x029:'159'
	// <0>
	// <1>
	// <0x5d5cba>|0x0ea|+0x008:'162'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5d5cc2>|0x0f2|+0x003:'169'
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
	// <10>
	// <11>
	// ******
}

// STATE[STUB]
void player::update_history_item_from_previous(
	client_player_history_item const&		previous_item,
	client_player_history_item&				item_to_update,
	float4x4&								previous_transform
)
{
	// LOCALS
	// math::quaternion 				target_rotation
	// ******

	// FUNCTION BODY[0x5d5800]: 75
	// <0x5d5811>|0x011|+0x029:'186'
	// <0x5d583a>|0x03a|+0x02f:'187'
	// <0x5d5869>|0x069|+0x0f3:'188'
	// <0x5d595c>|0x15c|+0x06d:'189'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d59c9>|0x1c9|+0x0c7:'198'
	// <0x5d5a90>|0x290|+0x02a:'199'
	// <0>
	// <1>
	// <2>
	// <0x5d5aba>|0x2ba|+0x01e:'203'
	// <0>
	// <0x5d5ad8>|0x2d8|+0x02d:'205'
	// <0x5d5b05>|0x305|+0x01f:'206'
	// <0x5d5b24>|0x324|+0x017:'207'
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
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <0x5d5b3b>|0x33b|+0x012:'240'
	// <0>
	// <1>
	// <0x5d5b4d>|0x34d|+0x03b:'243'
	// <0>
	// <0x5d5b88>|0x388|+0x01d:'245'
	// <0>
	// <0x5d5ba5>|0x3a5|+0x00d:'247'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5d5bb2>|0x3b2|+0x00f:'255'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
void player::replay_history( const u32 from_index, float4x4& previous_transform )
{
	// FUNCTION BODY[0x5d5fa0]: 16
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5d5fa6>|0x006|+0x008:'270'
	// <0>
	// <1>
	// <0x5d5fae>|0x00e|+0x031:'273'
	// <0>
	// <1>
	// <0x5d5fdf>|0x03f|+0x024:'276'
	// <0>
	// <1>
	// <2>
	// <0x5d6003>|0x063|+0x031:'280'
	// ******
}

// STATE[STUB]
void player::time_warp( server_player_update const& action, u32 time_in_ms )
{
	// LOCALS
	// float4x4 						transform
	// const u32 						lower_bound_index
	// float4x4 						previous_transform
	// ******

	// FUNCTION BODY[0x5d6040]: 127
	// <0x5d6040>|0x000|+0x013:'284'	{
	// <0x5d6053>|0x013|+0x020:'285'
	// <0x5d6073>|0x033|+0x006:'286'
	// <0>
	// <0x5d6079>|0x039|+0x00a:'288'
	// <0>
	// <1>
	// <2>
	// <0x5d6083>|0x043|+0x006:'292'
	// <0x5d6089>|0x049|+0x035:'293'
	// <0x5d60be>|0x07e|+0x02b:'294'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d60e9>|0x0a9|+0x03e:'299'
	// <0>
	// <1>
	// <0x5d6127>|0x0e7|+0x003:'302'
	// <0>
	// <0x5d612a>|0x0ea|+0x016:'304'
	// <0>
	// <1>
	// <2>
	// <0x5d6140>|0x100|+0x024:'308'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5d6164>|0x124|+0x02a:'316'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5d618e>|0x14e|+0x014:'324'
	// <0>
	// <1>
	// <2>
	// <0x5d61a2>|0x162|+0x00f:'328'
	// <0x5d61b1>|0x171|+0x009:'329'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5d61ba>|0x17a|+0x019:'337'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5d61d3>|0x193|+0x021:'344'
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
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <0x5d61f4>|0x1b4|+0x007:'370'
	// <0>
	// <1>
	// <0x5d61fb>|0x1bb|+0x00a:'373'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5d6205>|0x1c5|+0x036:'380'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d623b>|0x1fb|+0x01d:'389'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d6258>|0x218|+0x00e:'394'
	// <0x5d6266>|0x226|+0x013:'395'
	// <0x5d6279>|0x239|+0x00e:'396'
	// <0>
	// <0x5d6287>|0x247|+0x00e:'398'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d6295>|0x255|-0x15e:'407'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d6137>|0x0f7|+0x16e:'412'
	// <0x5d62a5>|0x265|      :'412'	}
	// ******
}

// STATE[STUB]
void player::log_active_object( pcstr const header ) const
{
	// FUNCTION BODY[0x5d5440]: 11
	// <0x5d5440>|0x000|+0x000:'415'	{
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
	// <10>
	// <0x5d5440>|0x000|      :'427'	}
	// ******
}

// STATE[STUB]
void player::apply_input_before_new_transform(
	client_player_state&	player_state,
	player_input const&		previous_input,
	const float				time_delta
)
{
	// LOCALS
	// math::quaternion 				previous_rotation
	// float 							angle
	// math::quaternion 				new_rotation
	// float4x4 						new_transform
	// float3 							axe
	// ******

	// FUNCTION BODY[0x5d5cd0]: 18
	// <0x5d5cde>|0x00e|+0x037:'512'
	// <0x5d5d15>|0x045|+0x01d:'513'
	// <0x5d5d32>|0x062|+0x00e:'514'
	// <0x5d5d40>|0x070|+0x100:'515'
	// <0>
	// <1>
	// <0x5d5e40>|0x170|+0x024:'518'
	// <0x5d5e64>|0x194|+0x01a:'519'
	// <0>
	// <0x5d5e7e>|0x1ae|+0x006:'521'
	// <0x5d5e84>|0x1b4|+0x060:'522'
	// <0x5d5ee4>|0x214|+0x029:'523'
	// <0>
	// <0x5d5f0d>|0x23d|+0x061:'525'
	// <0>
	// <1>
	// <0x5d5f6e>|0x29e|+0x00d:'528'
	// <0x5d5f7b>|0x2ab|+0x017:'529'
	// ******
}

// TU statics (compiler-generated dynamic initializers / atexit
// destructors); a matcher recovers their types/initializers from the asm.
/*
// STATE[STUB]
void `dynamic initializer for 'cc_player_name_min_font_size''( )
{
	// FUNCTION BODY[0x7d88f0]
	// <0x7d88f0>|0x000|      :'538'	{
	// ******
}

// STATE[STUB]
void `dynamic initializer for 'cc_player_name_max_font_size''( )
{
	// FUNCTION BODY[0x7d8960]
	// <0x7d8960>|0x000|      :'539'	{
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_net_max_position_discrepancy_command''( )
{
	// FUNCTION BODY[0x7f0330]
	// <0x7d89d0>|0x000|      :'540'	{
	// ******
}
*/

// STATE[STUB]
void player::smooth( const float time_delta )
{
	// LOCALS
	// float4x4 						result
	// math::quaternion 				right_rotation
	// math::quaternion 				left_rotation
	// ******

	// FUNCTION BODY[0x5d5510]: 42
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
	// <10>
	// <11>
	// <12>
	// <13>
	// <0x5d5510>|0x000|+0x01d:'558'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d552d>|0x01d|+0x00d:'563'
	// <0>
	// <1>
	// <0x5d553a>|0x02a|+0x011:'566'
	// <0x5d554b>|0x03b|+0x00b:'567'
	// <0>
	// <0x5d5556>|0x046|+0x03f:'569'
	// <0x5d5595>|0x085|+0x01f:'570'
	// <0x5d55b4>|0x0a4|+0x01f:'571'
	// <0x5d55d3>|0x0c3|+0x080:'572'
	// <0>
	// <1>
	// <0x5d5653>|0x143|+0x014:'575'
	// <0x5d5667>|0x157|+0x002:'576'
	// <0>
	// <0x5d5669>|0x159|+0x03c:'578'
	// <0x5d56a5>|0x195|+0x037:'579'
	// <0x5d56dc>|0x1cc|+0x050:'580'
	// <0x5d572c>|0x21c|+0x022:'581'
	// <0>
	// <0x5d574e>|0x23e|+0x00d:'583'
	// <0x5d575b>|0x24b|+0x03d:'584'
	// <0>
	// ******
}

// STATE[STUB]
void player::tick( const u32 current_time_in_ms )
{
	// LOCALS
	// const u32 						previous_time_in_ms
	// const u32 						time_delta_in_ms
	// bool 							name_visible
	// const bool 						is_current
	// player_input 					previous_input
	// u32 								time_in_ms
	// const float 						time_delta
	// const bool 						are_there_any_callbacks
	// float2 							screen_p
	// const float 						near_plane
	// const float 						width
	// const float 						font_size
	// float2 							p
	// ******

	// STATICS
	// static math::random32 			random = <0x4c2b60c>;
	// ******

	// CALL SITE INFO
	// <0x5d65c7> -> bool < unknown >() const
	// <0x5d65fd> -> damage_model_ptr const& < unknown >() const
	// <0x5d6619> -> weapon_core* < unknown >()
	// <0x5d686b> -> void < unknown >( player_input const&, const u32, float4x4 const&, const float )
	// <0x5d68aa> -> damage_model_ptr const& < unknown >() const
	// ******

	// FUNCTION BODY[0x5d62b0]: 265
	// <0x5d62b0>|0x000|+0x010:'589'	{
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
	// <10>
	// <0x5d62c0>|0x010|+0x00e:'601'
	// <0x5d62ce>|0x01e|+0x007:'602'
	// <0x5d62d5>|0x025|+0x006:'603'
	// <0>
	// <0x5d62db>|0x02b|+0x06c:'605'
	// <0x5d6347>|0x097|+0x002:'606'
	// <0x5d6349>|0x099|+0x00a:'607'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5d6353>|0x0a3|+0x010:'615'
	// <0x5d6363>|0x0b3|+0x00d:'616'
	// <0x5d6370>|0x0c0|+0x00c:'617'
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
	// <0x5d637c>|0x0cc|+0x022:'628'
	// <0x5d639e>|0x0ee|+0x065:'629'
	// <0x5d6403>|0x153|+0x015:'630'
	// <0x5d6418>|0x168|+0x02b:'631'
	// <0>
	// <1>
	// <2>
	// <0x5d6443>|0x193|+0x022:'635'
	// <0>
	// <0x5d6465>|0x1b5|+0x022:'637'
	// <0>
	// <0x5d6487>|0x1d7|+0x02e:'639'
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
	// <10>
	// <11>
	// <0x5d64b5>|0x205|+0x078:'652'
	// <0x5d652d>|0x27d|+0x028:'653'
	// <0>
	// <0x5d6555>|0x2a5|+0x020:'655'
	// <0x5d6575>|0x2c5|+0x019:'656'
	// <0x5d658e>|0x2de|+0x017:'657'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d65a5>|0x2f5|+0x009:'666'
	// <0x5d65ae>|0x2fe|+0x031:'667'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d65df>|0x32f|+0x007:'676'
	// <0x5d65e6>|0x336|+0x007:'677'
	// <0x5d65ed>|0x33d|+0x009:'678'
	// <0x5d65f6>|0x346|+0x01b:'679'
	// <0>
	// <0x5d6611>|0x361|+0x00e:'681'
	// <0x5d661f>|0x36f|+0x007:'682'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5d6626>|0x376|+0x00a:'688'
	// <0>
	// <1>
	// <0x5d6630>|0x380|+0x01b:'691'
	// <0>
	// <0x5d664b>|0x39b|+0x00f:'693'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5d665a>|0x3aa|+0x009:'701'
	// <0>
	// <0x5d6663>|0x3b3|+0x00f:'703'
	// <0>
	// <1>
	// <0x5d6672>|0x3c2|+0x018:'706'
	// <0x5d668a>|0x3da|+0x019:'707'
	// <0>
	// <0x5d66a3>|0x3f3|+0x013:'709'
	// <0x5d66b6>|0x406|+0x013:'710'
	// <0>
	// <0x5d66c9>|0x419|+0x011:'712'
	// <0x5d66da>|0x42a|+0x006:'713'
	// <0>
	// <0x5d66e0>|0x430|+0x022:'715'
	// <0x5d6702>|0x452|+0x014:'716'
	// <0>
	// <0x5d6716>|0x466|+0x054:'718'
	// <0x5d676a>|0x4ba|+0x014:'719'
	// <0>
	// <0x5d677e>|0x4ce|+0x00e:'721'
	// <0x5d678c>|0x4dc|+0x02c:'722'
	// <0x5d67b8>|0x508|+0x02e:'723'
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
	// <10>
	// <0x5d67e6>|0x536|+0x004:'735'
	// <0x5d67ea>|0x53a|+0x009:'736'
	// <0x5d67f3>|0x543|+0x007:'737'
	// <0x5d67fa>|0x54a|+0x008:'738'
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
	// <10>
	// <0x5d6802>|0x552|+0x01d:'750'
	// <0x5d681f>|0x56f|+0x00a:'751'
	// <0>
	// <1>
	// <0x5d6829>|0x579|+0x00f:'754'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d6838>|0x588|+0x035:'759'
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
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <0x5d686d>|0x5bd|+0x006:'792'
	// <0x5d6873>|0x5c3|+0x009:'793'
	// <0x5d687c>|0x5cc|+0x00c:'794'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d6888>|0x5d8|+0x007:'799'
	// <0x5d688f>|0x5df|+0x00b:'800'
	// <0x5d689a>|0x5ea|+0x009:'801'
	// <0x5d68a3>|0x5f3|+0x016:'802'
	// <0>
	// <0x5d68b9>|0x609|+0x019:'804'
	// <0>
	// <0x5d68d2>|0x622|+0x006:'806'
	// <0>
	// <0x5d68d8>|0x628|+0x008:'808'
	// <0>
	// <0x5d68e0>|0x630|+0x00e:'810'
	// <0x5d68ee>|0x63e|+0x00d:'811'
	// <0>
	// <1>
	// <2>
	// <0x5d68fb>|0x64b|+0x02b:'815'
	// <0>
	// <0x5d6926>|0x676|+0x00c:'817'
	// <0>
	// <0x5d6932>|0x682|+0x015:'819'
	// <0x5d6947>|0x697|+0x006:'820'
	// <0>
	// <1>
	// <0x5d694d>|0x69d|+0x00c:'823'
	// <0>
	// <1>
	// <0x5d6959>|0x6a9|+0x01b:'826'
	// <0>
	// <0x5d6974>|0x6c4|+0x008:'828'
	// <0x5d697c>|0x6cc|+0x056:'829'
	// <0>
	// <0x5d69d2>|0x722|+0x021:'831'
	// <0>
	// <0x5d69f3>|0x743|+0x027:'833'
	// <0>
	// <0x5d6a1a>|0x76a|+0x047:'835'
	// <0x5d6a61>|0x7b1|+0x032:'836'
	// <0x5d6a93>|0x7e3|+0x00f:'837'
	// <0x5d6aa2>|0x7f2|+0x017:'838'
	// <0x5d6ab9>|0x809|+0x014:'839'
	// <0x5d6acd>|0x81d|+0x032:'840'
	// <0x5d6aff>|0x84f|+0x026:'841'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d6b25>|0x875|-0x009:'846'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d6b1c>|0x86c|+0x035:'855'
	// <0x5d6b51>|0x8a1|      :'855'	}
	// ******
}

} // namespace survarium
