////////////////////////////////////////////////////////////////////////////
//	Created		: 21.09.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/mixing_n_ary_tree.h>

#include "mixing_n_ary_tree_destroyer.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include <vostok/animation/base_interpolator.h>
#include "mixing_n_ary_tree_dumper.h"
#include "bone_matrices_computer.h"
#include "mixing_n_ary_tree_event_iterator.h"
#include "mixing_n_ary_tree_animation_time_calculator.h"
#include "mixing_n_ary_tree_weight_calculator.h"
#include "mixing_n_ary_tree_time_scale_calculator.h"
#include "mixing_n_ary_tree_time_scale_start_time_modifier.h"
#include <vostok/animation/animation_callback.h>
#include "i_editor_mixer_inline.h"
#include "mixing_animation_state.h"
#include <vostok/animation/animation_player.h>
#include <vostok/animation/skeleton.h>

namespace vostok {
namespace animation {
namespace mixing {

// STATE[STUB]
 n_ary_tree::n_ary_tree(
	n_ary_tree_animation_node* const	weight_root,
	n_ary_tree_animation_node* const	time_root,
	base_interpolator const** const		interpolators,
	animation_state* const				animation_states,
	animation_state** const				animation_events,
	animated_object_holder* const		animated_objects,
	n_ary_tree_intrusive_base* const	reference_counter,
	const u32							animations_count,
	const u32							animated_objects_count,
	const u32							interpolators_count,
	const u32							current_time_in_ms
)
{
	// FUNCTION BODY
	// <0x6ef337>|0x047|+0x013:'480'
	// ******
}

// STATE[STUB]
void n_ary_tree::initialize( )
{
	// FUNCTION BODY
	// <0x6ef238>|0x008|+0x055:'492'
	// <0>
	// <0x6ef28d>|0x05d|+0x014:'494'
	// <0x6ef2a1>|0x071|+0x022:'495'
	// <0x6ef2c3>|0x093|+0x015:'496'
	// <0>
	// ******
}

// STATE[STUB]
n_ary_tree& n_ary_tree::operator=( n_ary_tree const& other )
{
	return *this;
	// FUNCTION BODY
	// <0>
	// <0x6ee290>|0x000|+0x006:'503'
	// <0>
	// <0x6ee296>|0x006|+0x005:'505'
	// <0>
	// <0x6ee29b>|0x00b|+0x016:'507'
	// <0x6ee2b1>|0x021|+0x006:'508'
	// <0x6ee2b7>|0x027|+0x006:'509'
	// <0x6ee2bd>|0x02d|+0x006:'510'
	// <0x6ee2c3>|0x033|+0x006:'511'
	// <0x6ee2c9>|0x039|+0x006:'512'
	// <0x6ee2cf>|0x03f|+0x006:'513'
	// <0x6ee2d5>|0x045|+0x006:'514'
	// <0x6ee2db>|0x04b|+0x006:'515'
	// <0x6ee2e1>|0x051|+0x006:'516'
	// <0x6ee2e7>|0x057|+0x006:'517'
	// <0x6ee2ed>|0x05d|+0x006:'518'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee2f3>|0x063|+0x002:'523'
	// ******
}

// STATE[STUB]
 n_ary_tree::~n_ary_tree( )
{
	// FUNCTION BODY
	// <0x6ee280>|0x000|+0x007:'528'
	// ******
}

// STATE[STUB]
void n_ary_tree::destroy( )
{
	// LOCALS
	// n_ary_tree_destroyer 			tree_destroyer
	// ******

	// CALL SITE INFO
	// <0x6ee1df> -> void < unknown >( n_ary_tree_visitor& )
	// <0x6ee208> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY
	// <0x6ee1a0>|0x000|+0x006:'532'	{
	// <0x6ee1a6>|0x006|+0x00d:'533'
	// <0>
	// <1>
	// <0x6ee1b3>|0x013|+0x007:'536'
	// <0x6ee1ba>|0x01a|+0x011:'537'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ee1cb>|0x02b|+0x008:'544'
	// <0>
	// <0x6ee1d3>|0x033|+0x015:'546'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee1e8>|0x048|+0x018:'551'
	// <0x6ee200>|0x060|+0x011:'552'
	// <0>
	// <0x6ee211>|0x071|+0x003:'554'
	// <0x6ee214>|0x074|+0x00b:'555'
	// <0x6ee21f>|0x07f|+0x004:'556'
	// <0x6ee223>|0x083|-0x05a:'557'
	// <0x6ee1c9>|0x029|+0x0af:'558'
	// <0x6ee278>|0x0d8|      :'558'	}
	// ******
}

// STATE[STUB]
float4x4 n_ary_tree::get_object_transform( pcvoid const animated_object ) const
{
	// LOCALS
	// float4x4 						left
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY
	// <0x6efa2f>|0x00f|+0x022:'641'
	// <0>
	// <1>
	// <0x6efa51>|0x031|+0x009:'644'
	// <0x6efa5a>|0x03a|+0x02e:'645'
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
	// <0x6efa88>|0x068|+0x063:'659'
	// ******
}

// STATE[STUB]
void n_ary_tree::set_object_transform( n_ary_tree_animation_node& animation_node )
{
	// LOCALS
	// current_frame_position 			frame_position
	// resources::pinned_ptr_const< cubic_spline_skeleton_animation > pinned_animation
	// ******

	// FUNCTION BODY
	// <0x6eef90>|0x000|+0x004:'663'	{
	// <0x6eef94>|0x004|+0x003:'664'
	// <0x6eef97>|0x007|+0x006:'665'
	// <0x6eef9d>|0x00d|+0x02c:'666'
	// <0>
	// <1>
	// <0x6eefc9>|0x039|+0x04e:'669'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ef017>|0x087|+0x02d:'674'
	// <0x6ef044>|0x0b4|+0x007:'675'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ef04b>|0x0bb|+0x01e:'680'
	// <0x6ef069>|0x0d9|+0x063:'681'
	// <0x6ef0cc>|0x13c|+0x033:'682'
	// <0>
	// <0x6ef0ff>|0x16f|+0x024:'684'
	// <0x6ef123>|0x193|+0x04a:'685'
	// <0x6ef16d>|0x1dd|+0x033:'686'
	// <0>
	// <1>
	// <0x6ef1a0>|0x210|+0x009:'689'
	// <0x6ef1a9>|0x219|+0x03d:'690'
	// <0x6ef1e6>|0x256|-0x04d:'691'
	// <0>
	// <0x6ef199>|0x209|+0x07b:'693'
	// <0x6ef214>|0x284|      :'693'	}
	// ******
}

// STATE[STUB]
void n_ary_tree::set_object_transform( pcvoid const animated_object, float4x4 const& object_transform )
{
	// FUNCTION BODY
	// <0x6ef9d9>|0x009|+0x007:'697'
	// <0>
	// <1>
	// <2>
	// <0x6ef9e0>|0x010|+0x005:'701'
	// <0x6ef9e5>|0x015|+0x00e:'702'
	// <0>
	// <0x6ef9f3>|0x023|+0x017:'704'
	// <0>
	// <1>
	// <0x6efa0a>|0x03a|+0x00d:'707'
	// ******
}

// STATE[STUB]
void n_ary_tree::set_objects_transform( )
{
	// FUNCTION BODY
	// <0x6f0899>|0x009|+0x017:'712'
	// <0x6f08b0>|0x020|+0x02b:'713'
	// ******
}

// STATE[STUB]
void n_ary_tree::compute_bones_matrices(
	pcvoid const		animated_object,
	skeleton const&		skeleton,
	float4x4* const		begin,
	float4x4* const		end,
	u32* const			bones_masks
) const
{
	// FUNCTION BODY
	// <0x6ef996>|0x006|+0x02e:'718'
	// ******
}

// STATE[STUB]
void n_ary_tree::compute_bones_local_matrices(
	pcvoid				animated_object,
	skeleton const&		skeleton,
	float4x4* const		begin,
	float4x4* const		end,
	u32* const			bones_masks
) const
{
	// LOCALS
	// bone_matrices_computer 			computer
	// ******

	// FUNCTION BODY
	// <0x6ef956>|0x006|+0x018:'723'
	// <0x6ef96e>|0x01e|+0x012:'724'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// ******
}

// STATE[STUB]
void n_ary_tree::convert_to_object_matrices(
	pcvoid				animated_object,
	skeleton const&		skeleton,
	float4x4* const		begin,
	float4x4* const		end
) const
{
	// FUNCTION BODY
	// <0x6ef916>|0x006|+0x029:'735'
	// ******
}

// STATE[STUB]
void n_ary_tree::accumulate_object_movement(
	n_ary_tree_animation_node&		animation_node,
	const float						animation_interval_time,
	const u32						time_in_ms
)
{
	// LOCALS
	// resources::pinned_ptr_const< cubic_spline_skeleton_animation > pinned_animation
	// current_frame_position 			frame_position
	// n_ary_tree_weight_calculator 	weight_calculator
	// ******

	// CALL SITE INFO
	// <0x6eeb5d> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY
	// <0>
	// <0x6eeb16>|0x006|+0x008:'745'
	// <0x6eeb1e>|0x00e|+0x041:'746'
	// <0>
	// <0x6eeb5f>|0x04f|+0x00b:'748'
	// <0>
	// <0x6eeb6a>|0x05a|+0x009:'750'
	// <0x6eeb73>|0x063|+0x00c:'751'
	// <0>
	// <0x6eeb7f>|0x06f|+0x00c:'753'
	// <0x6eeb8b>|0x07b|+0x049:'754'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6eebd4>|0x0c4|+0x05f:'761'
	// <0>
	// <0x6eec33>|0x123|+0x02c:'763'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6eec5f>|0x14f|+0x026:'768'
	// <0x6eec85>|0x175|+0x216:'769'
	// <0x6eee9b>|0x38b|+0x053:'770'
	// <0>
	// <0x6eeeee>|0x3de|+0x041:'772'
	// <0x6eef2f>|0x41f|+0x016:'773'
	// <0x6eef45>|0x435|+0x027:'774'
	// ******
}

// STATE[STUB]
animation_state& animation_state::operator=( animation_state const& __that )
{
	// FUNCTION BODY
	// <0x11c680>|0x000|+0x000:'785'	{
	// <0x11c680>|0x000|+0x014:'786'
	// <0>
	// <1>
	// <0x11c694>|0x014|+0x027:'789'
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
	// <0x11c6bb>|0x03b|-0x003:'800'
	// <0>
	// <0x11c6b8>|0x038|+0x038:'802'
	// <0x11c6f0>|0x070|      :'802'	}
	// ******
}

// STATE[STUB]
void n_ary_tree::update_synchronization_group_using_integration(
	n_ary_tree_animation_node&		animation_node,
	const u32						start_time_in_ms,
	const u32						target_time_in_ms
)
{
	// LOCALS
	// bool 							is_time_scale_node
	// float 							accumulated_animation_time
	// const float 						animation_interval_length
	// n_ary_tree_base_node* 			time_scale_node
	// const u32 						full_intervals_count
	// animation_state& 				animation_state
	// n_ary_tree_time_scale_calculator time_scale_calculator
	// const u32 						time_synchronization_group_id
	// ******

	// CALL SITE INFO
	// <0x6ef6c7> -> bool < unknown >()
	// <0x6ef7af> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x6ef6a9>|0x009|+0x003:'812'
	// <0x6ef6ac>|0x00c|+0x067:'813'
	// <0x6ef713>|0x073|-0x055:'813'
	// <0x6ef6be>|0x01e|+0x059:'814'
	// <0x6ef717>|0x077|-0x040:'814'
	// <0>
	// <1>
	// <2>
	// <0x6ef6d7>|0x037|+0x017:'818'
	// <0x6ef6ee>|0x04e|+0x009:'819'
	// <0x6ef6f7>|0x057|+0x01a:'820'
	// <0x6ef711>|0x071|+0x00f:'821'
	// <0x6ef720>|0x080|+0x1cf:'821'
	// <0x6ef8ef>|0x24f|-0x1c7:'821'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ef728>|0x088|+0x036:'827'
	// <0>
	// <0x6ef75e>|0x0be|+0x040:'829'
	// <0>
	// <0x6ef79e>|0x0fe|+0x013:'831'
	// <0x6ef7b1>|0x111|+0x00c:'832'
	// <0x6ef7bd>|0x11d|+0x003:'833'
	// <0>
	// <1>
	// <0x6ef7c0>|0x120|+0x017:'836'
	// <0>
	// <1>
	// <2>
	// <0x6ef7d7>|0x137|+0x004:'840'
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
	// <0x6ef7db>|0x13b|+0x02b:'861'
	// <0x6ef806>|0x166|+0x002:'862'
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
	// <0x6ef808>|0x168|+0x043:'878'
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
	// <0x6ef84b>|0x1ab|+0x023:'897'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6ef86e>|0x1ce|+0x011:'905'
	// <0x6ef87f>|0x1df|+0x00a:'906'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ef889>|0x1e9|+0x011:'911'
	// <0>
	// <0x6ef89a>|0x1fa|+0x007:'913'
	// <0x6ef8a1>|0x201|+0x005:'914'
	// <0>
	// <1>
	// <2>
	// <0x6ef8a6>|0x206|+0x003:'918'
	// <0>
	// <1>
	// <0x6ef8a9>|0x209|+0x00c:'921'
	// <0>
	// <0x6ef8b5>|0x215|+0x009:'923'
	// <0>
	// <1>
	// <0x6ef8be>|0x21e|+0x00c:'926'
	// <0x6ef8ca>|0x22a|+0x009:'927'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ef8d3>|0x233|+0x027:'932'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void n_ary_tree::update_animation_time( animation_state& animation_state )
{
	// LOCALS
	// const float 						animation_time
	// const float 						animation_length
	// ******

	// FUNCTION BODY
	// <0x6eea33>|0x003|+0x00b:'940'
	// <0>
	// <1>
	// <0x6eea3e>|0x00e|+0x012:'943'
	// <0x6eea50>|0x020|+0x00a:'944'
	// <0x6eea5a>|0x02a|+0x07e:'945'
	// <0>
	// <0x6eead8>|0x0a8|+0x024:'947'
	// <0>
	// <1>
	// <0x6eeafc>|0x0cc|+0x005:'950'
	// ******
}

// STATE[STUB]
void n_ary_tree::update_animation_state(
	n_ary_tree_animation_node&		animation_node,
	const u32						start_time_in_ms,
	const u32						target_time_in_ms
)
{
	// LOCALS
	// n_ary_tree_weight_calculator 	weight_calculator
	// ******

	// FUNCTION BODY
	// <0x6ef5d0>|0x000|+0x003:'958'	{
	// <0>
	// <1>
	// <0x6ef5d3>|0x003|+0x010:'961'
	// <0x6ef5e3>|0x013|+0x030:'962'
	// <0x6ef613>|0x043|+0x00b:'963'
	// <0>
	// <1>
	// <0x6ef61e>|0x04e|+0x005:'966'
	// <0>
	// <1>
	// <0x6ef623>|0x053|+0x007:'969'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6ef62a>|0x05a|+0x02c:'977'
	// <0>
	// <1>
	// <0x6ef656>|0x086|+0x003:'980'
	// <0x6ef659>|0x089|+0x00c:'981'
	// <0x6ef665>|0x095|+0x00c:'982'
	// <0x6ef671>|0x0a1|-0x02f:'983'
	// <0>
	// <1>
	// <0x6ef642>|0x072|+0x043:'986'
	// <0x6ef685>|0x0b5|-0x039:'986'
	// <0>
	// <1>
	// <0x6ef64c>|0x07c|+0x046:'989'
	// <0x6ef692>|0x0c2|      :'989'	}
	// ******
}

// STATE[STUB]
void n_ary_tree::update_time_synchronization_group(
	n_ary_tree_animation_node&		animation_node,
	const u32						start_time_in_ms,
	const u32						target_time_in_ms
)
{
	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0x6f07fa>|0x00a|+0x00a:'1000'
	// <0x6f0804>|0x014|+0x004:'1001'
	// <0x6f0808>|0x018|+0x005:'1002'
	// <0>
	// <0x6f080d>|0x01d|+0x003:'1004'
	// <0>
	// <1>
	// <0x6f0810>|0x020|+0x009:'1007'
	// <0>
	// <0x6f0819>|0x029|+0x059:'1009'
	// <0x6f0872>|0x082|+0x002:'1010'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6f0874>|0x084|+0x00d:'1018'
	// <0x6f0881>|0x091|-0x058:'1019'
	// <0x6f0829>|0x039|+0x00e:'1020'
	// <0>
	// <0x6f0837>|0x047|+0x005:'1022'
	// <0>
	// <1>
	// <2>
	// <0x6f083c>|0x04c|+0x007:'1026'
	// <0>
	// <1>
	// <0x6f0843>|0x053|+0x009:'1029'
	// <0>
	// <0x6f084c>|0x05c|+0x009:'1031'
	// <0>
	// <1>
	// <0x6f0855>|0x065|+0x015:'1034'
	// <0>
	// ******
}

// STATE[STUB]
void n_ary_tree::update_animation_states( const u32 start_time_in_ms, const u32 target_time_in_ms )
{
	// FUNCTION BODY
	// <0>
	// <0x6f0a76>|0x006|+0x00a:'1041'
	// <0x6f0a80>|0x010|+0x006:'1042'
	// <0x6f0a86>|0x016|+0x010:'1043'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
bool n_ary_tree::need_new_transform( const u32 target_time_in_ms ) const
{
	return false;

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x6ee136>|0x006|+0x00b:'1052'
	// <0x6ee141>|0x011|+0x00f:'1053'
	// <0x6ee150>|0x020|+0x00e:'1054'
	// <0x6ee15e>|0x02e|+0x01e:'1055'
	// <0>
	// <0x6ee17c>|0x04c|+0x007:'1057'
	// <0x6ee183>|0x053|+0x00d:'1058'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void n_ary_tree::remove_animation( n_ary_tree_animation_node*& i, n_ary_tree_animation_node* j )
{
	// LOCALS
	// n_ary_tree_destroyer 			destroyer
	// ******

	// CALL SITE INFO
	// <0x6ee102> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY
	// <0x6ee0a0>|0x000|+0x000:'1069'	{
	// <0>
	// <0x6ee0a0>|0x000|+0x020:'1071'
	// <0x6ee0c0>|0x020|+0x00f:'1072'
	// <0x6ee0cf>|0x02f|+0x00d:'1073'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6ee0dc>|0x03c|+0x00c:'1082'
	// <0>
	// <0x6ee0e8>|0x048|+0x002:'1084'
	// <0x6ee0ea>|0x04a|+0x006:'1085'
	// <0>
	// <1>
	// <0x6ee0f0>|0x050|+0x008:'1088'
	// <0x6ee0f8>|0x058|+0x00c:'1089'
	// <0>
	// <0x6ee104>|0x064|+0x004:'1091'
	// <0x6ee108>|0x068|+0x00d:'1092'
	// <0>
	// <0x6ee115>|0x075|-0x00a:'1094'
	// <0>
	// <0x6ee10b>|0x06b|+0x00d:'1096'
	// <0x6ee118>|0x078|-0x006:'1096'
	// <0x6ee112>|0x072|+0x00d:'1097'
	// <0x6ee11f>|0x07f|      :'1097'	}
	// ******
}

// STATE[STUB]
void n_ary_tree::process_event( n_ary_tree_animation_node& current_animation_node, const u32 event_types )
{
	// LOCALS
	// const u32 						event_type
	// n_ary_tree_time_scale_calculator time_scale_calculator
	// const u32 						event_time_in_ms
	// n_ary_tree_time_scale_calculator time_scale_calculator
	// const float 						animation_time
	// const float 						animation_length
	// animation_interval const& 		interval
	// const float 						animation_length
	// animation_interval const& 		interval
	// current_frame_position 			frame_position
	// resources::pinned_ptr_const< cubic_spline_skeleton_animation > pinned_animation
	// current_frame_position 			frame_position
	// n_ary_tree_weight_calculator 	weight_calculator
	// n_ary_tree_event_iterator 		event_iterator
	// resources::pinned_ptr_const< cubic_spline_skeleton_animation > pinned_animation
	// ******

	// CALL SITE INFO
	// <0x6efdcc> -> bool < unknown >()
	// <0x6efde1> -> void < unknown >( n_ary_tree_visitor& )
	// <0x6efdea> -> bool < unknown >()
	// <0x6efe59> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6efd2f>|0x00f|+0x003:'1244'
	// <0x6efd32>|0x012|+0x010:'1245'
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
	// <0x6efd42>|0x022|+0x019:'1277'
	// <0x6efd5b>|0x03b|+0x05e:'1278'
	// <0>
	// <0x6efdb9>|0x099|+0x046:'1280'
	// <0x6efdff>|0x0df|-0x03e:'1280'
	// <0x6efdc1>|0x0a1|+0x011:'1281'
	// <0x6efdd2>|0x0b2|+0x011:'1282'
	// <0x6efde3>|0x0c3|+0x01e:'1283'
	// <0x6efe01>|0x0e1|+0x008:'1283'
	// <0>
	// <1>
	// <2>
	// <0x6efe09>|0x0e9|+0x00c:'1287'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6efe15>|0x0f5|+0x010:'1292'
	// <0x6efe25>|0x105|+0x036:'1293'
	// <0x6efe5b>|0x13b|+0x006:'1294'
	// <0>
	// <1>
	// <0x6efe61>|0x141|+0x00d:'1297'
	// <0x6efe6e>|0x14e|+0x00c:'1298'
	// <0x6efe7a>|0x15a|+0x00a:'1299'
	// <0x6efe84>|0x164|+0x08e:'1300'
	// <0>
	// <0x6eff12>|0x1f2|+0x00f:'1302'
	// <0x6eff21>|0x201|+0x008:'1303'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6eff29>|0x209|+0x00b:'1309'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6eff34>|0x214|+0x00c:'1314'
	// <0x6eff40>|0x220|+0x006:'1315'
	// <0x6eff46>|0x226|+0x008:'1316'
	// <0x6eff4e>|0x22e|+0x008:'1317'
	// <0x6eff56>|0x236|+0x064:'1318'
	// <0>
	// <0x6effba>|0x29a|+0x005:'1320'
	// <0x6effbf>|0x29f|+0x011:'1321'
	// <0x6effd0>|0x2b0|+0x070:'1322'
	// <0x6f0040>|0x320|+0x006:'1323'
	// <0>
	// <0x6f0046>|0x326|+0x017:'1325'
	// <0x6f005d>|0x33d|+0x007:'1326'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6f0064>|0x344|+0x00b:'1331'
	// <0x6f006f>|0x34f|+0x004:'1332'
	// <0x6f0073>|0x353|+0x086:'1333'
	// <0x6f00f9>|0x3d9|+0x007:'1334'
	// <0x6f0100>|0x3e0|+0x003:'1335'
	// <0>
	// <0x6f0103>|0x3e3|+0x002:'1337'
	// <0>
	// <0x6f0105>|0x3e5|+0x008:'1339'
	// <0x6f010d>|0x3ed|+0x00c:'1340'
	// <0x6f0119>|0x3f9|-0x009:'1340'
	// <0x6f0110>|0x3f0|+0x005:'1341'
	// <0x6f0115>|0x3f5|+0x009:'1342'
	// <0>
	// <1>
	// <2>
	// <0x6f011e>|0x3fe|+0x00b:'1346'
	// <0x6f0129>|0x409|+0x00a:'1347'
	// <0x6f0133>|0x413|+0x003:'1348'
	// <0>
	// <0x6f0136>|0x416|+0x012:'1350'
	// <0>
	// <0x6f0148>|0x428|+0x010:'1352'
	// <0x6f0158>|0x438|+0x073:'1353'
	// <0x6f01cb>|0x4ab|+0x026:'1354'
	// <0x6f01f1>|0x4d1|+0x016:'1355'
	// <0x6f0207>|0x4e7|+0x006:'1356'
	// <0x6f020d>|0x4ed|+0x002:'1357'
	// <0>
	// <1>
	// <2>
	// <0x6f020f>|0x4ef|+0x003:'1361'
	// <0>
	// <0x6f0212>|0x4f2|+0x00e:'1363'
	// <0>
	// <1>
	// <0x6f0220>|0x500|+0x005:'1366'
	// <0>
	// <1>
	// <2>
	// <0x6f0225>|0x505|+0x044:'1370'
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
	// <0x6f0269>|0x549|+0x007:'1389'
	// <0x6f0270>|0x550|+0x005:'1390'
	// <0>
	// <1>
	// <2>
	// <0x6f0275>|0x555|+0x044:'1394'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6f02b9>|0x599|+0x00b:'1402'
	// <0x6f02c4>|0x5a4|+0x009:'1403'
	// <0x6f02cd>|0x5ad|+0x026:'1404'
	// <0>
	// <1>
	// <0x6f02f3>|0x5d3|+0x046:'1407'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6f0339>|0x619|+0x02d:'1412'
	// <0x6f0366>|0x646|+0x007:'1413'
	// <0>
	// <1>
	// <0x6f036d>|0x64d|+0x042:'1416'
	// <0x6f03af>|0x68f|+0x158:'1417'
	// <0x6f0507>|0x7e7|+0x04b:'1418'
	// <0x6f0552>|0x832|+0x006:'1419'
	// <0>
	// <1>
	// <2>
	// <0x6f0558>|0x838|+0x00e:'1423'
	// <0>
	// <0x6f0566>|0x846|+0x00c:'1425'
	// <0x6f0572>|0x852|+0x032:'1426'
	// <0>
	// <1>
	// <0x6f05a4>|0x884|+0x046:'1429'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6f05ea>|0x8ca|+0x030:'1434'
	// <0x6f061a>|0x8fa|+0x007:'1435'
	// <0>
	// <1>
	// <0x6f0621>|0x901|+0x033:'1438'
	// <0x6f0654>|0x934|+0x044:'1439'
	// <0x6f0698>|0x978|+0x036:'1440'
	// <0>
	// <1>
	// <0x6f06ce>|0x9ae|+0x01b:'1443'
	// <0x6f06e9>|0x9c9|+0x026:'1444'
	// <0>
	// <1>
	// <0x6f070f>|0x9ef|+0x011:'1447'
	// <0x6f0720>|0xa00|+0x03a:'1448'
	// <0x6f075a>|0xa3a|+0x030:'1449'
	// <0>
	// <0x6f078a>|0xa6a|+0x057:'1451'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// ******
}

// STATE[STUB]
void n_ary_tree::process_events( const u32 target_time_in_ms, const u32 event_types )
{
	// FUNCTION BODY
	// <0>
	// <0x6f0a36>|0x006|+0x00a:'1464'
	// <0x6f0a40>|0x010|+0x00b:'1465'
	// <0x6f0a4b>|0x01b|+0x00b:'1466'
	// <0x6f0a56>|0x026|+0x00e:'1467'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void n_ary_tree::update_event_iterators( const u32 target_time_in_ms )
{
	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6efc40>|0x000|+0x020:'1482'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6efc60>|0x020|+0x061:'1489'
	// <0>
	// <0x6efcc1>|0x081|+0x020:'1491'
	// <0x6efce1>|0x0a1|+0x005:'1492'
	// <0x6efce6>|0x0a6|+0x01b:'1493'
	// <0x6efd01>|0x0c1|+0x014:'1494'
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
	// ******
}

// STATE[STUB]
bool n_ary_tree::dispatch_callbacks(
	callback_generator_info const* const	callback_generators_head,
	subscribed_channel*&	channels_head,
	const u32				current_time_in_ms,
	bool&					callbacks_are_actual
)
{
	// LOCALS
	// bool 							result
	// callback_generator_info const* 	generator
	// resources::pinned_ptr_const< cubic_spline_skeleton_animation > pinned_animation
	// animation_callback_params 		params
	// subscribed_channel const* 		subscribed_channel
	// u8 								domain_data
	// const u32 						knot_upper_id
	// animation_callback_params 		params
	// ******

	// CONSTANTS
	// const n_ary_tree::dispatch_callbacks::__l77::< unnamed-tag > partitions = 1;
	// const n_ary_tree::dispatch_callbacks::__l77::< unnamed-tag > intervals = 0;
	// const n_ary_tree::dispatch_callbacks::__l77::< unnamed-tag > events = 2;
	// ******

	// CALL SITE INFO
	// <0x6ee5a7> -> < unknown >
	// <0x6ee948> -> < unknown >
	// ******

	return false;

	// FUNCTION BODY
	// <0x6ee486>|0x016|+0x008:'1526'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ee48e>|0x01e|+0x012:'1533'
	// <0x6ee4a0>|0x030|+0x00a:'1534'
	// <0x6ee4aa>|0x03a|+0x150:'1535'
	// <0x6ee5fa>|0x18a|-0x13a:'1535'
	// <0x6ee4c0>|0x050|+0x00c:'1536'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ee4cc>|0x05c|+0x009:'1542'
	// <0x6ee4d5>|0x065|+0x005:'1543'
	// <0>
	// <0x6ee4da>|0x06a|+0x009:'1545'
	// <0x6ee4e3>|0x073|+0x005:'1546'
	// <0>
	// <0x6ee4e8>|0x078|+0x00d:'1548'
	// <0x6ee4f5>|0x085|+0x005:'1549'
	// <0>
	// <0x6ee4fa>|0x08a|+0x008:'1551'
	// <0>
	// <1>
	// <0x6ee502>|0x092|+0x0ed:'1554'
	// <0x6ee5ef>|0x17f|-0x0df:'1554'
	// <0x6ee510>|0x0a0|+0x00a:'1555'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee51a>|0x0aa|+0x018:'1560'
	// <0>
	// <1>
	// <0x6ee532>|0x0c2|+0x010:'1563'
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
	// <0x6ee542>|0x0d2|+0x06c:'1579'
	// <0x6ee5ae>|0x13e|+0x013:'1580'
	// <0x6ee5c1>|0x151|+0x013:'1581'
	// <0x6ee5d4>|0x164|+0x031:'1582'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee605>|0x195|+0x00a:'1587'
	// <0>
	// <1>
	// <0x6ee60f>|0x19f|+0x050:'1590'
	// <0x6ee65f>|0x1ef|+0x004:'1591'
	// <0x6ee663>|0x1f3|+0x006:'1592'
	// <0x6ee669>|0x1f9|+0x04c:'1593'
	// <0>
	// <0x6ee6b5>|0x245|+0x2eb:'1595'
	// <0x6ee9a0>|0x530|-0x2d0:'1595'
	// <0x6ee6d0>|0x260|+0x059:'1596'
	// <0x6ee729>|0x2b9|+0x009:'1597'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee732>|0x2c2|+0x011:'1602'
	// <0>
	// <1>
	// <0x6ee743>|0x2d3|+0x006:'1605'
	// <0x6ee749>|0x2d9|+0x009:'1606'
	// <0x6ee752>|0x2e2|+0x04c:'1607'
	// <0>
	// <0x6ee79e>|0x32e|+0x012:'1609'
	// <0x6ee7b0>|0x340|+0x012:'1610'
	// <0x6ee7c2>|0x352|+0x025:'1611'
	// <0x6ee7e7>|0x377|+0x03b:'1612'
	// <0>
	// <0x6ee822>|0x3b2|+0x02e:'1614'
	// <0x6ee850>|0x3e0|+0x008:'1615'
	// <0>
	// <1>
	// <0x6ee858>|0x3e8|+0x00e:'1618'
	// <0>
	// <0x6ee866>|0x3f6|+0x002:'1620'
	// <0x6ee868>|0x3f8|+0x002:'1621'
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
	// <0x6ee86a>|0x3fa|+0x004:'1638'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ee86e>|0x3fe|+0x127:'1644'
	// <0x6ee995>|0x525|-0x0f1:'1644'
	// <0x6ee8a4>|0x434|+0x00a:'1645'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee8ae>|0x43e|+0x018:'1650'
	// <0>
	// <1>
	// <0x6ee8c6>|0x456|+0x00f:'1653'
	// <0>
	// <1>
	// <0x6ee8d5>|0x465|+0x010:'1656'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ee8e5>|0x475|+0x07a:'1661'
	// <0x6ee95f>|0x4ef|+0x01c:'1662'
	// <0x6ee97b>|0x50b|+0x038:'1663'
	// <0>
	// <1>
	// <0x6ee9b3>|0x543|+0x061:'1666'
	// <0>
	// <0x6eea14>|0x5a4|+0x004:'1668'
	// ******
}

// STATE[STUB]
void n_ary_tree::remove_animations( const u32 target_time_in_ms )
{
	// LOCALS
	// animation_state* 				current_alive_animation_state
	// buffer_vector< pcvoid > 			animated_objects
	// animation_state* 				current_animation_state
	// n_ary_tree_animation_node* 		previous_animation
	// n_ary_tree_animation_node* 		current_animation
	// animated_object_holder* const 	e
	// animated_object_holder* 			j
	// ******

	// FUNCTION BODY
	// <0>
	// <0x6ef358>|0x008|+0x021:'1674'
	// <0>
	// <1>
	// <2>
	// <0x6ef379>|0x029|+0x02d:'1678'
	// <0>
	// <1>
	// <0x6ef3a6>|0x056|+0x00b:'1681'
	// <0x6ef3b1>|0x061|+0x009:'1682'
	// <0>
	// <0x6ef3ba>|0x06a|+0x038:'1684'
	// <0>
	// <1>
	// <0x6ef3f2>|0x0a2|+0x010:'1687'
	// <0x6ef402>|0x0b2|+0x005:'1688'
	// <0>
	// <1>
	// <2>
	// <0x6ef407>|0x0b7|+0x018:'1692'
	// <0x6ef41f>|0x0cf|+0x012:'1693'
	// <0>
	// <0x6ef431>|0x0e1|+0x00a:'1695'
	// <0x6ef43b>|0x0eb|+0x006:'1696'
	// <0x6ef441>|0x0f1|+0x003:'1697'
	// <0>
	// <1>
	// <2>
	// <0x6ef444>|0x0f4|+0x003:'1701'
	// <0x6ef447>|0x0f7|+0x01e:'1702'
	// <0>
	// <1>
	// <0x6ef465>|0x115|+0x00c:'1705'
	// <0x6ef471>|0x121|+0x01f:'1706'
	// <0x6ef490>|0x140|+0x00e:'1707'
	// <0>
	// <0x6ef49e>|0x14e|+0x0a8:'1709'
	// <0>
	// <1>
	// <0x6ef546>|0x1f6|-0x056:'1712'
	// <0x6ef4f0>|0x1a0|+0x059:'1713'
	// <0>
	// <0x6ef549>|0x1f9|+0x00f:'1715'
	// <0x6ef558>|0x208|+0x056:'1716'
	// <0x6ef5ae>|0x25e|-0x03e:'1716'
	// <0x6ef570>|0x220|+0x012:'1717'
	// <0x6ef582>|0x232|+0x007:'1718'
	// <0x6ef589>|0x239|+0x012:'1719'
	// <0>
	// <1>
	// <0x6ef59b>|0x24b|+0x016:'1722'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ef5b1>|0x261|+0x00b:'1728'
	// <0>
	// ******
}

// STATE[STUB]
bool n_ary_tree::update_event_iterators_and_dispatch_callbacks(
	const u32				target_time_in_ms,
	subscribed_channel*&	channels_head,
	bool&					callbacks_are_actual
)
{
	// LOCALS
	// callback_generator_info const* 	callback_generators_head
	// callback_generator_info* 		previous_generator_info
	// animation_state const* const 	e
	// const u16 						event_type
	// const bool 						result
	// ******

	return false;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0x6f08f8>|0x008|+0x0e8:'1741'
	// <0x6f09e0>|0x0f0|-0x0c0:'1741'
	// <0x6f0920>|0x030|+0x00f:'1742'
	// <0>
	// <1>
	// <0x6f092f>|0x03f|+0x010:'1745'
	// <0>
	// <1>
	// <0x6f093f>|0x04f|+0x015:'1748'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6f0954>|0x064|+0x006:'1756'
	// <0>
	// <1>
	// <0x6f095a>|0x06a|+0x003:'1759'
	// <0x6f095d>|0x06d|+0x00d:'1760'
	// <0>
	// <1>
	// <0x6f096a>|0x07a|+0x015:'1763'
	// <0x6f097f>|0x08f|+0x003:'1764'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6f0982>|0x092|+0x038:'1773'
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
	// <0x6f09ba>|0x0ca|+0x007:'1790'
	// <0x6f09c1>|0x0d1|+0x003:'1791'
	// <0x6f09c4>|0x0d4|+0x002:'1792'
	// <0x6f09c6>|0x0d6|+0x003:'1793'
	// <0>
	// <0x6f09c9>|0x0d9|+0x01a:'1795'
	// <0>
	// <1>
	// <0x6f09e3>|0x0f3|+0x00a:'1798'
	// <0x6f09ed>|0x0fd|+0x009:'1799'
	// <0x6f09f6>|0x106|+0x015:'1800'
	// <0>
	// <0x6f0a0b>|0x11b|+0x006:'1802'
	// <0x6f0a11>|0x121|+0x002:'1803'
	// <0x6f0a13>|0x123|+0x003:'1804'
	// <0x6f0a16>|0x126|+0x009:'1805'
	// <0>
	// <1>
	// <0x6f0a1f>|0x12f|+0x003:'1808'
	// ******
}

// STATE[STUB]
callback_generator_info* n_ary_tree::generate_animation_lexeme_end_events(
	n_ary_tree const&					previous_tree,
	n_ary_tree const&					new_tree,
	callback_generator_info*			callback_generators_buffer_begin,
	callback_generator_info* const		callback_generators_buffer_end,
	subscribed_channel* const			channels_head
)
{
	// LOCALS
	// callback_generator_info* 		callback_generators_head
	// callback_generator_info* 		previous_generator_info
	// ******

	return NULL;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0x6ee307>|0x007|+0x150:'1822'
	// <0x6ee457>|0x157|-0x137:'1822'
	// <0x6ee320>|0x020|+0x02d:'1823'
	// <0x6ee34d>|0x04d|+0x008:'1823'
	// <0>
	// <1>
	// <2>
	// <0x6ee355>|0x055|+0x05f:'1827'
	// <0x6ee3b4>|0x0b4|-0x051:'1827'
	// <0x6ee363>|0x063|+0x007:'1828'
	// <0x6ee36a>|0x06a|+0x043:'1829'
	// <0x6ee3ad>|0x0ad|-0x03c:'1829'
	// <0x6ee371>|0x071|+0x006:'1830'
	// <0>
	// <1>
	// <0x6ee377>|0x077|+0x02a:'1833'
	// <0>
	// <1>
	// <0x6ee3a1>|0x0a1|-0x056:'1836'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ee34b>|0x04b|+0x075:'1843'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6ee3c0>|0x0c0|+0x004:'1852'
	// <0>
	// <1>
	// <2>
	// <0x6ee3c4>|0x0c4|+0x012:'1856'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6ee3d6>|0x0d6|+0x06c:'1865'
	// <0>
	// <0x6ee442>|0x142|+0x008:'1867'
	// <0x6ee44a>|0x14a|+0x003:'1868'
	// <0x6ee44d>|0x14d|+0x002:'1869'
	// <0x6ee44f>|0x14f|+0x004:'1870'
	// <0>
	// <0x6ee453>|0x153|+0x00f:'1872'
	// <0>
	// <1>
	// <0x6ee462>|0x162|+0x004:'1875'
	// ******
}

// STATE[STUB]
u32 n_ary_tree::nearest_event_time_in_ms( ) const
{
	return 0;

	// FUNCTION BODY
	// <0x6ee080>|0x000|+0x000:'1892'	{
	// <0x6ee080>|0x000|+0x012:'1893'
	// <0x6ee092>|0x012|-0x001:'1893'
	// <0x6ee091>|0x011|+0x004:'1894'
	// <0x6ee095>|0x015|      :'1894'	}
	// ******
}

// STATE[STUB]
void n_ary_tree::adjust_animation_events_times( n_ary_tree const& other )
{
	// LOCALS
	// animation_state* const 			e
	// n_ary_tree_animation_node* const animation
	// subscribed_channel** const 		channels_head
	// ******

	// FUNCTION BODY
	// <0>
	// <0x6efb03>|0x003|+0x0e1:'1951'
	// <0x6efbe4>|0x0e4|-0x0b4:'1951'
	// <0x6efb30>|0x030|+0x006:'1952'
	// <0x6efb36>|0x036|+0x006:'1953'
	// <0x6efb3c>|0x03c|+0x049:'1954'
	// <0>
	// <0x6efb85>|0x085|+0x025:'1956'
	// <0x6efbaa>|0x0aa|+0x012:'1957'
	// <0x6efbbc>|0x0bc|+0x02c:'1958'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6efbe8>|0x0e8|+0x04d:'1964'
	// ******
}

// STATE[STUB]
bool n_ary_tree::tick(
	const u32				target_time_in_ms,
	subscribed_channel*&	channels_head,
	bool&					callbacks_are_actual
)
{
	// LOCALS
	// animated_object_holder* 			e
	// bool 							user_handled_callbacks
	// ******

	return false;

	// FUNCTION BODY
	// <0x6f0aa0>|0x000|+0x009:'1972'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6f0aa9>|0x009|+0x00d:'1979'
	// <0x6f0ab6>|0x016|+0x007:'1980'
	// <0x6f0abd>|0x01d|+0x00c:'1981'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6f0ac9>|0x029|+0x10e:'1987'
	// <0x6f0bd7>|0x137|-0x0f3:'1987'
	// <0>
	// <1>
	// <2>
	// <0x6f0ae4>|0x044|+0x007:'1991'
	// <0x6f0aeb>|0x04b|+0x00a:'1992'
	// <0>
	// <0x6f0af5>|0x055|+0x009:'1994'
	// <0x6f0afe>|0x05e|+0x013:'1995'
	// <0>
	// <0x6f0b11>|0x071|+0x016:'1997'
	// <0x6f0b27>|0x087|+0x009:'1998'
	// <0x6f0b30>|0x090|+0x030:'1999'
	// <0>
	// <1>
	// <0x6f0b60>|0x0c0|+0x00d:'2002'
	// <0>
	// <0x6f0b6d>|0x0cd|+0x013:'2004'
	// <0x6f0b80>|0x0e0|+0x009:'2005'
	// <0x6f0b89>|0x0e9|+0x012:'2006'
	// <0x6f0b9b>|0x0fb|+0x011:'2007'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6f0bac>|0x10c|+0x05f:'2014'
	// <0x6f0c0b>|0x16b|-0x05b:'2015'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6f0bb0>|0x110|+0x035:'2022'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6f0be5>|0x145|+0x00d:'2031'
	// <0>
	// <0x6f0bf2>|0x152|+0x008:'2033'
	// <0>
	// <0x6f0bfa>|0x15a|+0x003:'2035'
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
	// <0x6f0bfd>|0x15d|-0x13e:'2049'
	// <0x6f0abf>|0x01f|+0x142:'2050'
	// <0x6f0c01>|0x161|      :'2050'	}
	// ******
}

} // namespace mixing
} // namespace animation
} // namespace vostok
