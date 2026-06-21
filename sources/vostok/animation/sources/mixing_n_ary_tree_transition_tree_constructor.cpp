////////////////////////////////////////////////////////////////////////////
//	Created		: 05.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_n_ary_tree_transition_tree_constructor.h"

#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_time_scale_transition_node.h"
#include <vostok/animation/mixing_n_ary_tree.h>
#include "mixing_n_ary_tree_node_comparer.h"
#include "mixing_n_ary_tree_time_scale_calculator.h"
#include "mixing_n_ary_tree_node_comparer.h"
#include "mixing_n_ary_tree_interpolator_selector.h"
#include "mixing_n_ary_tree_transition_tree_constructor_impl.h"
#include "mixing_n_ary_tree_comparer.h"
#include "i_editor_mixer_inline.h"
#include "mixing_n_ary_tree_target_time_scale_calculator.h"
#include "mixing_n_ary_tree_weight_calculator.h"

#define VOSTOK_DEBUG_ANIMATIONS_COUNT			0

namespace vostok {
namespace animation {
namespace mixing {

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_animation_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::add_animation_node(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::animation_state const*, unsigned int, float, bool)
n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::add_animation_node(
	n_ary_tree_animation_node&		new_animation,
	animation_state const*			previous_animation_state,
	u32								animation_interval_id,
	float							animation_interval_time,
	bool							is_new_animation
)
{
	// LOCALS
	// n_ary_tree_weight_calculator 	weight_calculator
	// u16 								initial_event_types
	// ******

	// CALL SITE INFO
	// <0x6e9e97> -> bool < unknown >()
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6e9e03>|0x003|+0x00d:'50'
	// <0>
	// <0x6e9e10>|0x010|+0x00c:'52'
	// <0x6e9e1c>|0x01c|+0x006:'53'
	// <0>
	// <0x6e9e22>|0x022|+0x008:'55'
	// <0>
	// <0x6e9e2a>|0x02a|+0x002:'57'
	// <0x6e9e2c>|0x02c|+0x008:'58'
	// <0>
	// <0x6e9e34>|0x034|+0x005:'60'
	// <0x6e9e39>|0x039|+0x008:'61'
	// <0>
	// <1>
	// <2>
	// <0x6e9e41>|0x041|+0x009:'65'
	// <0x6e9e4a>|0x04a|+0x030:'66'
	// <0>
	// <1>
	// <2>
	// <0x6e9e7a>|0x07a|+0x016:'70'
	// <0x6e9e90>|0x090|+0x010:'71'
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
	// <0x6e9ea0>|0x0a0|+0x026:'83'
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
	// <0x6e9ec6>|0x0c6|+0x04b:'107'
	// <0>
	// <0x6e9f11>|0x111|+0x007:'109'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_time_scale(vostok::animation::mixing::n_ary_tree_animation_node&, unsigned int&, float&)
n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale( n_ary_tree_animation_node& new_time_driving_animation, u32& animation_interval_id, float& animation_interval_time )
{
	// LOCALS
	// float 							new_time_driving_animation_time_offset
	// n_ary_tree_time_scale_calculator time_scale_calculator
	// n_ary_tree_base_node* 			time_scale_node
	// float 							directional_time_scale_factor
	// float 							new_driving_animation_length
	// float 							time_scale_factor
	// bool 							is_target_time_scale_node
	// n_ary_tree_base_node* 			target_time_scale_node
	// float 							new_time_driving_animation_target_time_scale
	// ******

	// CALL SITE INFO
	// <0x6ea52f> -> bool < unknown >()
	// <0x6ea546> -> void < unknown >( n_ary_tree_visitor& )
	// <0x6ea57a> -> float < unknown >() const
	// <0x6ea598> -> bool < unknown >()
	// <0x6ea641> -> bool < unknown >()
	// <0x6ea783> -> bool < unknown >()
	// ******

	return NULL;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ea3e3>|0x003|+0x00d:'130'
	// <0x6ea3f0>|0x010|+0x01d:'131'
	// <0x6ea40d>|0x02d|-0x00b:'131'
	// <0x6ea402>|0x022|+0x01e:'132'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ea420>|0x040|-0x00c:'138'
	// <0x6ea414>|0x034|+0x015:'139'
	// <0>
	// <0x6ea429>|0x049|+0x00a:'141'
	// <0x6ea433>|0x053|+0x00e:'142'
	// <0x6ea441>|0x061|+0x002:'143'
	// <0x6ea443>|0x063|+0x002:'144'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6ea445>|0x065|+0x011:'154'
	// <0x6ea456>|0x076|+0x016:'155'
	// <0>
	// <1>
	// <2>
	// <0x6ea46c>|0x08c|+0x011:'159'
	// <0>
	// <1>
	// <0x6ea47d>|0x09d|+0x004:'162'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ea481>|0x0a1|+0x022:'167'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6ea4a3>|0x0c3|+0x02c:'175'
	// <0x6ea4cf>|0x0ef|+0x004:'176'
	// <0>
	// <1>
	// <0x6ea4d3>|0x0f3|+0x045:'179'
	// <0x6ea518>|0x138|+0x03c:'180'
	// <0x6ea554>|0x174|-0x030:'180'
	// <0x6ea524>|0x144|+0x011:'181'
	// <0>
	// <0x6ea535>|0x155|+0x013:'183'
	// <0x6ea548>|0x168|+0x010:'184'
	// <0x6ea558>|0x178|+0x00a:'184'
	// <0>
	// <1>
	// <2>
	// <0x6ea562>|0x182|+0x011:'188'
	// <0>
	// <0x6ea573>|0x193|+0x017:'190'
	// <0>
	// <1>
	// <0x6ea58a>|0x1aa|+0x017:'193'
	// <0x6ea5a1>|0x1c1|+0x036:'194'
	// <0>
	// <1>
	// <0x6ea5d7>|0x1f7|+0x046:'197'
	// <0>
	// <1>
	// <2>
	// <0x6ea61d>|0x23d|+0x02e:'201'
	// <0>
	// <0x6ea64b>|0x26b|+0x009:'203'
	// <0>
	// <0x6ea654>|0x274|+0x006:'205'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ea65a>|0x27a|+0x037:'212'
	// <0x6ea691>|0x2b1|+0x029:'213'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6ea6ba>|0x2da|+0x035:'222'
	// <0x6ea6ef>|0x30f|+0x009:'223'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6ea6f8>|0x318|+0x03a:'231'
	// <0x6ea732>|0x352|+0x019:'232'
	// <0x6ea74b>|0x36b|+0x019:'232'
	// <0>
	// <1>
	// <0x6ea764>|0x384|+0x029:'235'
	// <0>
	// <0x6ea78d>|0x3ad|+0x01f:'237'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ea7ac>|0x3cc|+0x013:'242'
	// <0x6ea7bf>|0x3df|+0x009:'243'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ea7c8>|0x3e8|+0x031:'250'
	// <0x6ea7f9>|0x419|+0x01a:'251'
	// <0x6ea813>|0x433|-0x0d4:'251'
	// <0>
	// <1>
	// <2>
	// <0x6ea73f>|0x35f|+0x019:'255'
	// <0x6ea758>|0x378|+0x041:'255'
	// <0x6ea799>|0x3b9|+0x06e:'255'
	// <0x6ea807>|0x427|+0x01e:'255'
	// <0x6ea825>|0x445|-0x40f:'255'
	// <0x6ea416>|0x036|+0x32b:'256'
	// <0x6ea741>|0x361|+0x019:'256'
	// <0x6ea75a>|0x37a|+0x048:'256'
	// <0x6ea7a2>|0x3c2|+0x067:'256'
	// <0x6ea809>|0x429|+0x020:'256'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_animation_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node*, unsigned int, unsigned int&, unsigned int&, unsigned int&, float&, bool, bool)
n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::new_animation(
	n_ary_tree_animation_node&		to,
	n_ary_tree_animation_node&		from,
	n_ary_tree_animation_node*		weight_driving_animation,
	u32								weight_operands_count,
	u32&							time_scale_operands_count,
	u32&							operands_offset,
	u32&							animation_interval_id,
	float&							animation_interval_time,
	bool							is_transitting_to_zero,
	bool							can_be_time_driving_animation
)
{
	// LOCALS
	// n_ary_tree_base_node* 			time_scale_node
	// animation_interval const* 		cloned_intervals_begin
	// animation_state const& 			time_driving_animation_state
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6eaae0>|0x000|+0x035:'271'
	// <0x6eab15>|0x035|+0x011:'272'
	// <0>
	// <1>
	// <2>
	// <0x6eab26>|0x046|+0x033:'276'
	// <0x6eab59>|0x079|+0x00e:'277'
	// <0x6eab67>|0x087|+0x004:'278'
	// <0x6eab6b>|0x08b|+0x00a:'279'
	// <0x6eab75>|0x095|+0x006:'280'
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
	// <0x6eab7b>|0x09b|+0x006:'292'
	// <0x6eab81>|0x0a1|+0x00f:'293'
	// <0x6eab90>|0x0b0|+0x049:'294'
	// <0x6eabd9>|0x0f9|-0x045:'294'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6eab94>|0x0b4|+0x034:'299'
	// <0x6eabc8>|0x0e8|+0x015:'300'
	// <0>
	// <1>
	// <0x6eabdd>|0x0fd|+0x005:'303'
	// <0x6eabe2>|0x102|+0x00c:'304'
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
	// <0x6eabee>|0x10e|+0x0a1:'326'
	// <0x6eac8f>|0x1af|+0x002:'327'
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
	// <0x6eac91>|0x1b1|+0x05a:'348'
	// <0>
	// <1>
	// <0x6eaceb>|0x20b|+0x00a:'351'
	// <0>
	// <0x6eacf5>|0x215|+0x006:'353'
	// <0>
	// <0x6eacfb>|0x21b|+0x006:'355'
	// <0x6ead01>|0x221|+0x003:'356'
	// <0x6ead04>|0x224|+0x002:'357'
	// <0>
	// <0x6ead06>|0x226|+0x006:'359'
	// <0>
	// <1>
	// <0x6ead0c>|0x22c|+0x016:'362'
	// <0x6ead22>|0x242|+0x005:'363'
	// <0x6ead27>|0x247|+0x010:'364'
	// <0x6ead37>|0x257|+0x003:'365'
	// <0>
	// <0x6ead3a>|0x25a|+0x005:'367'
	// <0>
	// <0x6ead3f>|0x25f|+0x007:'369'
	// <0x6ead46>|0x266|+0x003:'370'
	// <0x6ead49>|0x269|+0x007:'371'
	// <0>
	// <0x6ead50>|0x270|+0x018:'373'
	// <0x6ead68>|0x288|+0x02e:'374'
	// <0>
	// <0x6ead96>|0x2b6|+0x002:'376'
	// <0x6ead98>|0x2b8|+0x026:'377'
	// <0x6eadbe>|0x2de|+0x01e:'378'
	// <0>
	// <1>
	// <2>
	// <0x6eaddc>|0x2fc|+0x01c:'382'
	// <0x6eadf8>|0x318|+0x004:'383'
	// <0x6eadfc>|0x31c|+0x014:'384'
	// <0x6eae10>|0x330|+0x00f:'385'
	// <0x6eae1f>|0x33f|+0x018:'386'
	// <0x6eae37>|0x357|+0x002:'387'
	// <0>
	// <0x6eae39>|0x359|+0x002:'389'
	// <0x6eae3b>|0x35b|+0x01c:'390'
	// <0>
	// <0x6eae57>|0x377|+0x007:'392'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_weight_transition(vostok::animation::base_interpolator const&, float, float)
n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_weight_transition( base_interpolator const& interpolator, float from, float to )
{
	// CALL SITE INFO
	// <0x6e9d0f> -> float < unknown >() const
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6e9d02>|0x002|+0x01b:'407'
	// <0x6e9d1d>|0x01d|+0x007:'408'
	// <0x6e9d24>|0x024|+0x007:'409'
	// <0x6e9d2b>|0x02b|+0x02f:'410'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6e9d5a>|0x05a|+0x007:'415'
	// <0>
	// <0x6e9d61>|0x061|+0x006:'417'
	// <0x6e9d67>|0x067|+0x007:'418'
	// <0x6e9d6e>|0x06e|+0x01e:'419'
	// <0>
	// <0x6e9d8c>|0x08c|+0x005:'421'
	// <0x6e9d91>|0x091|+0x007:'422'
	// <0x6e9d98>|0x098|+0x028:'423'
	// <0>
	// <0x6e9dc0>|0x0c0|-0x06e:'425'
	// <0>
	// <0x6e9d52>|0x052|+0x0a0:'427'
	// <0x6e9df2>|0x0f2|-0x09c:'427'
	// <0x6e9d56>|0x056|+0x0a0:'428'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_animation_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::remove_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node* const, bool)
n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::remove_animation( n_ary_tree_animation_node& animation, n_ary_tree_animation_node* weight_driving_animation, bool is_new_driving_animation )
{
	// LOCALS
	// float 							animation_interval_time
	// bool 							can_be_time_driving_animation
	// n_ary_tree_animation_node* 		result
	// u32 								animation_interval_id
	// n_ary_tree_base_node** 			new_operands
	// n_ary_tree_weight_transition_node* transition
	// u32 								operands_offset
	// float 							animation_interval_time
	// n_ary_tree_animation_node* 		result
	// u32 								animation_interval_id
	// u32 								operands_offset
	// n_ary_tree_base_node** 			multiplicands
	// n_ary_tree_base_node** 			e
	// u32 								time_scale_operands_count
	// n_ary_tree_base_node* 			weight_from
	// bool 							can_be_time_driving_animation
	// u32 								time_scale_operands_count
	// n_ary_tree_base_node** 			new_operands
	// n_ary_tree_base_node** 			operands_end
	// ******

	// CALL SITE INFO
	// <0x6ebb6c> -> float < unknown >() const
	// <0x6ebbaa> -> bool < unknown >()
	// <0x6ebc67> -> bool < unknown >()
	// <0x6ebd25> -> bool < unknown >()
	// <0x6ebe13> -> bool < unknown >()
	// <0x6ebe75> -> bool < unknown >()
	// <0x6ebf60> -> bool < unknown >()
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6ebb4a>|0x00a|+0x01d:'436'
	// <0x6ebb67>|0x027|+0x015:'437'
	// <0x6ebb7c>|0x03c|+0x00c:'438'
	// <0>
	// <0x6ebb88>|0x048|+0x015:'440'
	// <0>
	// <0x6ebb9d>|0x05d|+0x01c:'442'
	// <0x6ebbb9>|0x079|+0x003:'443'
	// <0>
	// <1>
	// <0x6ebbbc>|0x07c|+0x013:'446'
	// <0x6ebbcf>|0x08f|+0x011:'447'
	// <0x6ebbe0>|0x0a0|+0x00e:'448'
	// <0x6ebbee>|0x0ae|+0x004:'449'
	// <0x6ebbf2>|0x0b2|+0x005:'450'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ebbf7>|0x0b7|+0x008:'457'
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
	// <0x6ebbff>|0x0bf|+0x025:'474'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ebc24>|0x0e4|+0x010:'479'
	// <0x6ebc34>|0x0f4|+0x014:'480'
	// <0>
	// <1>
	// <2>
	// <0x6ebc48>|0x108|+0x00d:'484'
	// <0>
	// <1>
	// <0x6ebc55>|0x115|+0x00b:'487'
	// <0x6ebc60>|0x120|+0x00d:'488'
	// <0x6ebc6d>|0x12d|+0x007:'489'
	// <0x6ebc74>|0x134|+0x005:'490'
	// <0>
	// <0x6ebc79>|0x139|+0x002:'492'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ebc7b>|0x13b|+0x038:'498'
	// <0>
	// <0x6ebcb3>|0x173|+0x002:'500'
	// <0>
	// <0x6ebcb5>|0x175|+0x037:'502'
	// <0>
	// <1>
	// <2>
	// <0x6ebcec>|0x1ac|+0x027:'506'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ebd13>|0x1d3|+0x024:'511'
	// <0>
	// <0x6ebd37>|0x1f7|+0x00d:'513'
	// <0x6ebd44>|0x204|-0x158:'514'
	// <0x6ebbec>|0x0ac|+0x164:'515'
	// <0x6ebd50>|0x210|+0x00e:'515'
	// <0x6ebd5e>|0x21e|+0x004:'516'
	// <0x6ebd62>|0x222|+0x005:'517'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ebd67>|0x227|+0x004:'524'
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
	// <0x6ebd6b>|0x22b|+0x026:'541'
	// <0>
	// <1>
	// <0x6ebd91>|0x251|+0x004:'544'
	// <0x6ebd95>|0x255|+0x020:'545'
	// <0>
	// <0x6ebdb5>|0x275|+0x004:'547'
	// <0>
	// <1>
	// <0x6ebdb9>|0x279|+0x03c:'550'
	// <0>
	// <1>
	// <0x6ebdf5>|0x2b5|+0x005:'553'
	// <0x6ebdfa>|0x2ba|+0x007:'554'
	// <0>
	// <1>
	// <0x6ebe01>|0x2c1|+0x026:'557'
	// <0x6ebe27>|0x2e7|+0x16c:'558'
	// <0>
	// <0x6ebf93>|0x453|+0x005:'560'
	// <0x6ebf98>|0x458|+0x00b:'561'
	// <0x6ebfa3>|0x463|-0x04b:'562'
	// <0>
	// <1>
	// <2>
	// <0x6ebf58>|0x418|+0x036:'566'
	// <0x6ebf8e>|0x44e|-0x153:'567'
	// <0>
	// <1>
	// <0x6ebe3b>|0x2fb|+0x005:'570'
	// <0x6ebe40>|0x300|+0x00b:'571'
	// <0x6ebe4b>|0x30b|+0x00d:'572'
	// <0x6ebe58>|0x318|+0x005:'573'
	// <0x6ebe5d>|0x31d|+0x00c:'574'
	// <0x6ebe69>|0x329|+0x027:'575'
	// <0x6ebe90>|0x350|+0x033:'576'
	// <0>
	// <0x6ebec3>|0x383|+0x004:'578'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ebec7>|0x387|+0x005:'584'
	// <0x6ebecc>|0x38c|+0x007:'585'
	// <0x6ebed3>|0x393|+0x023:'586'
	// <0>
	// <1>
	// <0x6ebef6>|0x3b6|+0x035:'589'
	// <0x6ebf2b>|0x3eb|-0x227:'590'
	// <0>
	// <0x6ebd04>|0x1c4|+0x22b:'592'
	// <0x6ebf2f>|0x3ef|-0x3b1:'592'
	// <0x6ebb7e>|0x03e|+0x18b:'593'
	// <0x6ebd09>|0x1c9|+0x245:'593'
	// ******
}

// STATE[STUB]
// void vostok::animation::mixing::n_ary_tree_transition_tree_constructor::remove_weight_synchronization_group(vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*)
void n_ary_tree_transition_tree_constructor::remove_weight_synchronization_group( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end )
{
	// FUNCTION BODY
	// <0x6ec275>|0x005|+0x020:'600'
	// <0x6ec295>|0x025|+0x004:'601'
	// <0>
	// <1>
	// <0x6ec299>|0x029|+0x003:'604'
	// <0x6ec29c>|0x02c|+0x014:'605'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ec2b0>|0x040|+0x01e:'610'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_animation_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::add_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node* const)
n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::add_animation( n_ary_tree_animation_node& animation, n_ary_tree_animation_node* weight_driving_animation )
{
	// LOCALS
	// float 							animation_interval_time
	// u32 								time_scale_operands_count
	// n_ary_tree_animation_node* 		result
	// u32 								to_operands_count
	// u32 								animation_interval_id
	// n_ary_tree_weight_node 			temp
	// u32 								operands_offset
	// n_ary_tree_node_comparer 		comparer
	// n_ary_tree_base_node** 			new_operands
	// ******

	// CALL SITE INFO
	// <0x6eb84a> -> bool < unknown >()
	// <0x6eb859> -> float < unknown >() const
	// <0x6eb8bf> -> bool < unknown >()
	// <0x6eb953> -> bool < unknown >()
	// <0x6eb9c8> -> float < unknown >() const
	// <0x6eba85> -> float < unknown >() const
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6eb810>|0x000|+0x025:'618'
	// <0>
	// <0x6eb835>|0x025|+0x007:'620'
	// <0x6eb83c>|0x02c|+0x003:'621'
	// <0x6eb83f>|0x02f|+0x033:'622'
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
	// <0x6eb872>|0x062|+0x034:'638'
	// <0x6eb8a6>|0x096|+0x01f:'639'
	// <0x6eb8c5>|0x0b5|+0x005:'640'
	// <0>
	// <1>
	// <2>
	// <0x6eb8ca>|0x0ba|+0x009:'644'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6eb8d3>|0x0c3|+0x018:'652'
	// <0>
	// <1>
	// <2>
	// <0x6eb8eb>|0x0db|+0x003:'656'
	// <0x6eb8ee>|0x0de|+0x0d1:'657'
	// <0x6eb9bf>|0x1af|-0x099:'657'
	// <0x6eb926>|0x116|+0x026:'658'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6eb94c>|0x13c|+0x12e:'666'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6eba7a>|0x26a|+0x01b:'673'
	// <0x6eba95>|0x285|+0x020:'674'
	// <0>
	// <0x6ebab5>|0x2a5|+0x00f:'676'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ebac4>|0x2b4|-0x101:'683'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6eb9c3>|0x1b3|+0x015:'689'
	// <0x6eb9d8>|0x1c8|+0x01d:'690'
	// <0>
	// <1>
	// <2>
	// <0x6eb9f5>|0x1e5|+0x104:'694'
	// <0x6ebaf9>|0x2e9|+0x015:'694'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ebb0e>|0x2fe|+0x021:'701'
	// ******
}

// STATE[STUB]
// void vostok::animation::mixing::n_ary_tree_transition_tree_constructor::add_weight_synchronization_group(vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*)
void n_ary_tree_transition_tree_constructor::add_weight_synchronization_group( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end )
{
	// FUNCTION BODY
	// <0x6ec22d>|0x00d|+0x00b:'709'
	// <0x6ec238>|0x018|+0x003:'710'
	// <0x6ec23b>|0x01b|+0x015:'711'
	// <0>
	// <1>
	// <2>
	// <0x6ec250>|0x030|+0x00f:'715'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_animation_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_weight_driving_animation(vostok::animation::mixing::n_ary_tree_animation_node&)
n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::new_weight_driving_animation( n_ary_tree_animation_node& animation )
{
	// LOCALS
	// float 							animation_interval_time
	// u32 								time_scale_operands_count
	// n_ary_tree_animation_node* 		result
	// u32 								weight_operands_count
	// n_ary_tree_base_node** 			operands_end
	// base_interpolator const& 		interpolator
	// u32 								animation_interval_id
	// n_ary_tree_weight_node 			temp
	// u32 								operands_offset
	// n_ary_tree_node_comparer 		comparer
	// n_ary_tree_base_node** 			operands_begin
	// ******

	// CALL SITE INFO
	// <0x6eb547> -> bool < unknown >()
	// <0x6eb562> -> float < unknown >() const
	// <0x6eb640> -> bool < unknown >()
	// <0x6eb6a9> -> float < unknown >() const
	// <0x6eb762> -> float < unknown >() const
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6eb529>|0x009|+0x004:'720'
	// <0>
	// <1>
	// <0x6eb52d>|0x00d|+0x029:'723'
	// <0>
	// <0x6eb556>|0x036|+0x025:'725'
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
	// <0x6eb57b>|0x05b|+0x02e:'740'
	// <0>
	// <0x6eb5a9>|0x089|+0x004:'742'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6eb5ad>|0x08d|+0x027:'749'
	// <0>
	// <1>
	// <2>
	// <0x6eb5d4>|0x0b4|+0x00b:'753'
	// <0x6eb5df>|0x0bf|+0x0c1:'754'
	// <0x6eb6a0>|0x180|-0x089:'754'
	// <0x6eb617>|0x0f7|+0x022:'755'
	// <0x6eb639>|0x119|+0x009:'756'
	// <0x6eb642>|0x122|+0x01e:'757'
	// <0x6eb660>|0x140|+0x002:'758'
	// <0x6eb662>|0x142|+0x0f7:'759'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6eb759>|0x239|+0x019:'766'
	// <0x6eb772>|0x252|+0x023:'767'
	// <0>
	// <0x6eb795>|0x275|+0x011:'769'
	// <0x6eb7a6>|0x286|-0x102:'770'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6eb6a4>|0x184|+0x015:'775'
	// <0x6eb6b9>|0x199|+0x020:'776'
	// <0>
	// <0x6eb6d9>|0x1b9|+0x0f7:'778'
	// <0x6eb7d0>|0x2b0|+0x00c:'778'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6eb7dc>|0x2bc|+0x01e:'786'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_time_scale_transition(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_base_node&, vostok::animation::mixing::n_ary_tree_base_node&)
n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale_transition(
	n_ary_tree_animation_node&		from_animation,
	n_ary_tree_animation_node&		to_animation,
	n_ary_tree_base_node&			from,
	n_ary_tree_base_node&			to
)
{
	// LOCALS
	// n_ary_tree_interpolator_selector interpolator_selector
	// n_ary_tree_time_scale_transition_node* result
	// n_ary_tree_base_node* 			time_scale_from
	// ******

	// CALL SITE INFO
	// <0x6ea2a2> -> float < unknown >() const
	// <0x6ea39a> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6ea20d>|0x00d|+0x026:'796'
	// <0x6ea233>|0x033|+0x067:'797'
	// <0>
	// <0x6ea29a>|0x09a|+0x003:'799'
	// <0x6ea29d>|0x09d|+0x015:'800'
	// <0x6ea2b2>|0x0b2|+0x066:'801'
	// <0>
	// <1>
	// <0x6ea318>|0x118|+0x009:'804'
	// <0x6ea321>|0x121|+0x007:'805'
	// <0>
	// <0x6ea328>|0x128|+0x030:'807'
	// <0x6ea358>|0x158|+0x029:'808'
	// <0>
	// <0x6ea381>|0x181|+0x00c:'810'
	// <0x6ea38d>|0x18d|+0x00f:'811'
	// <0x6ea39c>|0x19c|+0x00c:'812'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ea3a8>|0x1a8|-0x118:'819'
	// <0>
	// <1>
	// <0x6ea290>|0x090|+0x07e:'822'
	// <0x6ea30e>|0x10e|+0x0b9:'822'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_time_scale_transition(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_base_node&, float)
n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale_transition( n_ary_tree_animation_node& from_animation, n_ary_tree_base_node& from, float to )
{
	// LOCALS
	// n_ary_tree_interpolator_selector interpolator_selector
	// ******

	// CALL SITE INFO
	// <0x6ea0ba> -> void < unknown >( n_ary_tree_visitor& )
	// <0x6ea0c5> -> float < unknown >() const
	// ******

	return NULL;

	// FUNCTION BODY
	// <0>
	// <0x6ea09d>|0x00d|+0x01f:'831'
	// <0x6ea0bc>|0x02c|+0x019:'832'
	// <0x6ea0d5>|0x045|+0x00d:'833'
	// <0x6ea0e2>|0x052|+0x007:'834'
	// <0x6ea0e9>|0x059|+0x007:'835'
	// <0x6ea0f0>|0x060|+0x042:'836'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6ea132>|0x0a2|+0x005:'841'
	// <0x6ea137>|0x0a7|+0x007:'842'
	// <0>
	// <0x6ea13e>|0x0ae|+0x023:'844'
	// <0x6ea161>|0x0d1|+0x01c:'845'
	// <0>
	// <0x6ea17d>|0x0ed|+0x005:'847'
	// <0x6ea182>|0x0f2|+0x007:'848'
	// <0x6ea189>|0x0f9|+0x034:'849'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ea1bd>|0x12d|+0x031:'856'
	// <0>
	// <0x6ea1ee>|0x15e|-0x0c6:'858'
	// <0x6ea128>|0x098|+0x0c8:'859'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_time_scale_transition(const float, float, vostok::animation::mixing::n_ary_tree_base_node&)
n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_time_scale_transition( float animation_time, float from, n_ary_tree_base_node& to )
{
	// LOCALS
	// n_ary_tree_interpolator_selector interpolator_selector
	// ******

	// CALL SITE INFO
	// <0x6e9f47> -> float < unknown >() const
	// <0x6ea001> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6e9f3d>|0x00d|+0x003:'867'
	// <0x6e9f40>|0x010|+0x00d:'868'
	// <0x6e9f4d>|0x01d|+0x05a:'869'
	// <0>
	// <1>
	// <0x6e9fa7>|0x077|+0x005:'872'
	// <0x6e9fac>|0x07c|+0x007:'873'
	// <0>
	// <0x6e9fb3>|0x083|+0x031:'875'
	// <0>
	// <0x6e9fe4>|0x0b4|+0x010:'877'
	// <0x6e9ff4>|0x0c4|+0x00f:'878'
	// <0x6ea003>|0x0d3|+0x00c:'879'
	// <0>
	// <0x6ea00f>|0x0df|+0x005:'881'
	// <0x6ea014>|0x0e4|+0x007:'882'
	// <0x6ea01b>|0x0eb|+0x02e:'883'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ea049>|0x119|+0x031:'890'
	// <0>
	// <0x6ea07a>|0x14a|-0x0dd:'892'
	// <0x6e9f9d>|0x06d|+0x0df:'893'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_weight_transition(vostok::animation::mixing::n_ary_tree_base_node&, vostok::animation::mixing::n_ary_tree_base_node&)
n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_weight_transition( n_ary_tree_base_node& from, n_ary_tree_base_node& to )
{
	// LOCALS
	// n_ary_tree_interpolator_selector interpolator_selector
	// n_ary_tree_base_node* 			weight_from
	// ******

	// CALL SITE INFO
	// <0x6e9c23> -> float < unknown >() const
	// <0x6e9cb5> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6e9c19>|0x009|+0x00e:'900'
	// <0x6e9c27>|0x017|+0x036:'901'
	// <0>
	// <0x6e9c5d>|0x04d|+0x005:'903'
	// <0x6e9c62>|0x052|+0x007:'904'
	// <0>
	// <0x6e9c69>|0x059|+0x018:'906'
	// <0x6e9c81>|0x071|+0x01b:'907'
	// <0>
	// <0x6e9c9c>|0x08c|+0x00c:'909'
	// <0x6e9ca8>|0x098|+0x00f:'910'
	// <0x6e9cb7>|0x0a7|+0x00c:'911'
	// <0>
	// <0x6e9cc3>|0x0b3|+0x024:'913'
	// <0>
	// <0x6e9ce7>|0x0d7|-0x094:'915'
	// <0x6e9c53>|0x043|+0x096:'916'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_weight_transition(vostok::animation::base_interpolator const&, vostok::animation::mixing::n_ary_tree_base_node&, float)
n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_weight_transition( base_interpolator const& from_animation_interpolator, n_ary_tree_base_node& from, float to )
{
	// LOCALS
	// n_ary_tree_weight_node 			weight
	// ******

	// CALL SITE INFO
	// <0x6e9ad8> -> float < unknown >() const
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6e9aca>|0x00a|+0x01e:'924'
	// <0x6e9ae8>|0x028|+0x009:'925'
	// <0x6e9af1>|0x031|+0x005:'926'
	// <0x6e9af6>|0x036|+0x007:'927'
	// <0x6e9afd>|0x03d|+0x01a:'928'
	// <0x6e9b17>|0x057|+0x00c:'929'
	// <0>
	// <1>
	// <2>
	// <0x6e9b23>|0x063|+0x03c:'933'
	// <0x6e9b5f>|0x09f|+0x02a:'934'
	// <0>
	// <0x6e9b89>|0x0c9|+0x005:'936'
	// <0x6e9b8e>|0x0ce|+0x007:'937'
	// <0>
	// <0x6e9b95>|0x0d5|+0x014:'939'
	// <0>
	// <0x6e9ba9>|0x0e9|+0x016:'941'
	// <0>
	// <0x6e9bbf>|0x0ff|+0x005:'943'
	// <0x6e9bc4>|0x104|+0x007:'944'
	// <0x6e9bcb>|0x10b|+0x01a:'945'
	// <0>
	// <0x6e9be5>|0x125|-0x0cc:'947'
	// <0>
	// <1>
	// <0x6e9b19>|0x059|+0x066:'950'
	// <0x6e9b7f>|0x0bf|+0x082:'950'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_base_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_weight_transition(vostok::animation::base_interpolator const&, float, vostok::animation::mixing::n_ary_tree_base_node&)
n_ary_tree_base_node* n_ary_tree_transition_tree_constructor::new_weight_transition( base_interpolator const& to_animation_interpolator, float from, n_ary_tree_base_node& to )
{
	// LOCALS
	// n_ary_tree_interpolator_selector interpolator_selector
	// n_ary_tree_weight_node 			weight
	// ******

	// CALL SITE INFO
	// <0x6e9985> -> float < unknown >() const
	// <0x6e9a5c> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6e997d>|0x00d|+0x013:'958'
	// <0x6e9990>|0x020|+0x02a:'959'
	// <0>
	// <0x6e99ba>|0x04a|+0x00c:'961'
	// <0x6e99c6>|0x056|+0x02e:'962'
	// <0x6e99f4>|0x084|+0x029:'963'
	// <0>
	// <0x6e9a1d>|0x0ad|+0x005:'965'
	// <0x6e9a22>|0x0b2|+0x007:'966'
	// <0>
	// <0x6e9a29>|0x0b9|+0x01b:'968'
	// <0>
	// <0x6e9a44>|0x0d4|+0x00c:'970'
	// <0x6e9a50>|0x0e0|+0x00e:'971'
	// <0x6e9a5e>|0x0ee|+0x00c:'972'
	// <0>
	// <0x6e9a6a>|0x0fa|+0x005:'974'
	// <0x6e9a6f>|0x0ff|+0x007:'975'
	// <0x6e9a76>|0x106|+0x01a:'976'
	// <0>
	// <0x6e9a90>|0x120|-0x0e0:'978'
	// <0>
	// <1>
	// <0x6e99b0>|0x040|+0x063:'981'
	// <0x6e9a13>|0x0a3|+0x099:'981'
	// ******
}

// STATE[STUB]
// void vostok::animation::mixing::n_ary_tree_transition_tree_constructor::add_operands(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_base_node**, vostok::animation::mixing::n_ary_tree_base_node**, const bool)
void n_ary_tree_transition_tree_constructor::add_operands(
	n_ary_tree_animation_node&		from,
	n_ary_tree_animation_node&		to,
	n_ary_tree_base_node**			operands_begin,
	n_ary_tree_base_node**			operands_end,
	bool							skip_time_scale_node
)
{
	// LOCALS
	// base_interpolator const& 		from_interpolator
	// n_ary_tree_interpolator_selector interpolator_selector
	// n_ary_tree_base_node** 			i_e
	// n_ary_tree_base_node** 			j_e
	// n_ary_tree_node_comparer 		comparer
	// base_interpolator const* 		j_interpolator
	// base_interpolator const* 		i_interpolator
	// ******

	// CALL SITE INFO
	// <0x6ea890> -> bool < unknown >()
	// <0x6ea8a8> -> bool < unknown >()
	// <0x6ea913> -> bool < unknown >()
	// <0x6ea9c3> -> void < unknown >( n_ary_tree_visitor& )
	// <0x6ea9da> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6ea843>|0x003|+0x01f:'999'
	// <0>
	// <0x6ea862>|0x022|+0x023:'1001'
	// <0>
	// <0x6ea885>|0x045|+0x011:'1003'
	// <0x6ea896>|0x056|+0x018:'1004'
	// <0x6ea8ae>|0x06e|+0x007:'1005'
	// <0x6ea8b5>|0x075|+0x022:'1006'
	// <0x6ea8d7>|0x097|-0x008:'1006'
	// <0x6ea8cf>|0x08f|+0x00c:'1007'
	// <0x6ea8db>|0x09b|-0x009:'1007'
	// <0x6ea8d2>|0x092|+0x00c:'1008'
	// <0x6ea8de>|0x09e|-0x009:'1008'
	// <0>
	// <0x6ea8d5>|0x095|+0x00c:'1010'
	// <0x6ea8e1>|0x0a1|+0x002:'1010'
	// <0x6ea8e3>|0x0a3|+0x007:'1011'
	// <0x6ea8ea>|0x0aa|+0x017:'1012'
	// <0x6ea901>|0x0c1|-0x005:'1012'
	// <0x6ea8fc>|0x0bc|+0x003:'1013'
	// <0>
	// <1>
	// <0x6ea8ff>|0x0bf|+0x006:'1016'
	// <0x6ea905>|0x0c5|+0x014:'1017'
	// <0x6ea919>|0x0d9|+0x007:'1018'
	// <0x6ea920>|0x0e0|+0x01d:'1019'
	// <0x6ea93d>|0x0fd|+0x007:'1020'
	// <0>
	// <1>
	// <2>
	// <0x6ea944>|0x104|+0x010:'1024'
	// <0>
	// <0x6ea954>|0x114|+0x0f2:'1026'
	// <0x6eaa46>|0x206|-0x0de:'1026'
	// <0x6ea968>|0x128|+0x01f:'1027'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ea987>|0x147|+0x027:'1033'
	// <0>
	// <1>
	// <0x6ea9ae>|0x16e|+0x00b:'1036'
	// <0>
	// <1>
	// <0x6ea9b9>|0x179|+0x00c:'1039'
	// <0x6ea9c5>|0x185|+0x004:'1040'
	// <0>
	// <0x6ea9c9>|0x189|+0x013:'1042'
	// <0x6ea9dc>|0x19c|+0x004:'1043'
	// <0>
	// <0x6ea9e0>|0x1a0|+0x035:'1045'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6eaa15>|0x1d5|+0x014:'1052'
	// <0x6eaa29>|0x1e9|+0x003:'1053'
	// <0x6eaa2c>|0x1ec|-0x029:'1054'
	// <0>
	// <1>
	// <0x6eaa03>|0x1c3|+0x010:'1057'
	// <0x6eaa13>|0x1d3|+0x01b:'1058'
	// <0>
	// <1>
	// <0x6eaa2e>|0x1ee|+0x012:'1061'
	// <0x6eaa40>|0x200|-0x003:'1061'
	// <0x6eaa3d>|0x1fd|+0x006:'1062'
	// <0x6eaa43>|0x203|+0x00d:'1063'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6eaa50>|0x210|+0x010:'1070'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6eaa60>|0x220|+0x020:'1076'
	// <0>
	// <1>
	// <0x6eaa80>|0x240|+0x006:'1079'
	// <0x6eaa86>|0x246|+0x01f:'1080'
	// <0>
	// <1>
	// <2>
	// <0x6eaaa5>|0x265|+0x008:'1084'
	// <0>
	// <1>
	// <0x6eaaad>|0x26d|+0x020:'1087'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
std::pair< u32, u32 > computed_operands_count( n_ary_tree_animation_node& from, n_ary_tree_animation_node& to )
{
	// LOCALS
	// n_ary_tree_interpolator_selector interpolator_selector
	// n_ary_tree_base_node** 			j_e
	// n_ary_tree_node_comparer 		comparer
	// u32 								time_scale_nodes_count
	// n_ary_tree_base_node** 			i_e
	// ******

	// CALL SITE INFO
	// <0x6e9862> -> bool < unknown >()
	// <0x6e987f> -> bool < unknown >()
	// <0x6e9893> -> bool < unknown >()
	// <0x6e98ec> -> void < unknown >( n_ary_tree_visitor& )
	// <0x6e98fe> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6e982a>|0x00a|+0x003:'1103'
	// <0>
	// <0x6e982d>|0x00d|+0x00d:'1105'
	// <0>
	// <1>
	// <0x6e983a>|0x01a|+0x01d:'1108'
	// <0>
	// <0x6e9857>|0x037|+0x011:'1110'
	// <0>
	// <0x6e9868>|0x048|+0x00b:'1112'
	// <0x6e9873>|0x053|+0x012:'1113'
	// <0>
	// <1>
	// <0x6e9885>|0x065|+0x002:'1116'
	// <0x6e9887>|0x067|+0x012:'1117'
	// <0x6e9899>|0x079|+0x008:'1118'
	// <0x6e98a1>|0x081|+0x003:'1119'
	// <0>
	// <1>
	// <2>
	// <0x6e98a4>|0x084|+0x00c:'1123'
	// <0x6e98b0>|0x090|+0x07e:'1124'
	// <0x6e992e>|0x10e|-0x06e:'1124'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6e98c0>|0x0a0|+0x01e:'1129'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6e98de>|0x0be|+0x002:'1138'
	// <0>
	// <1>
	// <0x6e98e0>|0x0c0|+0x00e:'1141'
	// <0>
	// <1>
	// <0x6e98ee>|0x0ce|+0x012:'1144'
	// <0>
	// <1>
	// <0x6e9900>|0x0e0|+0x022:'1147'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6e9922>|0x102|+0x003:'1155'
	// <0x6e9925>|0x105|-0x009:'1156'
	// <0>
	// <1>
	// <0x6e991c>|0x0fc|+0x004:'1159'
	// <0>
	// <0x6e9920>|0x100|+0x007:'1161'
	// <0>
	// <1>
	// <0x6e9927>|0x107|+0x001:'1164'
	// <0x6e9928>|0x108|+0x003:'1165'
	// <0x6e992b>|0x10b|+0x00b:'1166'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6e9936>|0x116|+0x00a:'1173'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6e9940>|0x120|+0x008:'1180'
	// <0>
	// <1>
	// <0x6e9948>|0x128|+0x008:'1183'
	// <0x6e9950>|0x130|+0x003:'1184'
	// <0x6e9953>|0x133|+0x005:'1185'
	// <0>
	// <1>
	// <0x6e9958>|0x138|+0x008:'1188'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_animation_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::new_weight_driving_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&)
n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::new_weight_driving_animation( n_ary_tree_animation_node& new_weight_driving_animation, n_ary_tree_animation_node& new_driving_animation_in_previous_target )
{
	// LOCALS
	// float 							animation_interval_time
	// n_ary_tree_animation_node* 		result
	// u32 								animation_interval_id
	// u32 								operands_offset
	// u32 								time_scale_operands_count
	// n_ary_tree_base_node** 			operands
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6eb45d>|0x00d|+0x00f:'1196'
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
	// <0x6eb46c>|0x01c|+0x03c:'1213'
	// <0x6eb4a8>|0x058|+0x008:'1214'
	// <0>
	// <0x6eb4b0>|0x060|+0x012:'1216'
	// <0x6eb4c2>|0x072|+0x00c:'1217'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6eb4ce>|0x07e|+0x01c:'1225'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6eb4ea>|0x09a|+0x01d:'1234'
	// ******
}

// STATE[STUB]
// void vostok::animation::mixing::n_ary_tree_transition_tree_constructor::change_animation(vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node&, vostok::animation::mixing::n_ary_tree_animation_node* const, bool)
void n_ary_tree_transition_tree_constructor::change_animation(
	n_ary_tree_animation_node&		from,
	n_ary_tree_animation_node&		to,
	n_ary_tree_animation_node*		weight_driving_animation,
	bool							is_new_driving_animation
)
{
	// LOCALS
	// n_ary_tree_animation_node* 		result
	// u32 								animation_interval_id
	// n_ary_tree_base_node** 			from_end
	// float 							animation_interval_time
	// u32 								time_scale_operands_count
	// n_ary_tree_animation_node* 		result
	// u32 								right_multiplicands_count
	// n_ary_tree_base_node* 			weight_from
	// u32 								animation_interval_id
	// n_ary_tree_base_node** 			new_operands
	// u32 								left_multiplicands_count
	// u32 								operands_offset
	// n_ary_tree_base_node** 			to_end
	// n_ary_tree_base_node** 			multiplicands
	// float 							animation_interval_time
	// u32 								time_scale_operands_count
	// u32 								operands_offset
	// n_ary_tree_base_node** 			to_begin
	// ******

	// CALL SITE INFO
	// <0x6eaf10> -> bool < unknown >()
	// <0x6eaf27> -> bool < unknown >()
	// <0x6eafa9> -> bool < unknown >()
	// <0x6eafc0> -> bool < unknown >()
	// <0x6eb017> -> bool < unknown >()
	// <0x6eb05f> -> bool < unknown >()
	// <0x6eb0bb> -> bool < unknown >()
	// <0x6eb186> -> bool < unknown >()
	// <0x6eb1e6> -> bool < unknown >()
	// <0x6eb25b> -> bool < unknown >()
	// ******

	// FUNCTION BODY
	// <0x6eaeb4>|0x004|+0x031:'1244'
	// <0>
	// <0x6eaee5>|0x035|+0x003:'1246'
	// <0>
	// <1>
	// <0x6eaee8>|0x038|+0x01c:'1249'
	// <0>
	// <0x6eaf04>|0x054|+0x03b:'1251'
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
	// <0x6eaf3f>|0x08f|+0x031:'1265'
	// <0>
	// <0x6eaf70>|0x0c0|+0x014:'1267'
	// <0x6eaf84>|0x0d4|+0x010:'1268'
	// <0>
	// <0x6eaf94>|0x0e4|+0x008:'1270'
	// <0>
	// <0x6eaf9c>|0x0ec|+0x013:'1272'
	// <0x6eafaf>|0x0ff|+0x017:'1273'
	// <0x6eafc6>|0x116|+0x024:'1274'
	// <0>
	// <0x6eafea>|0x13a|+0x002:'1276'
	// <0>
	// <0x6eafec>|0x13c|+0x018:'1278'
	// <0>
	// <0x6eb004>|0x154|+0x002:'1280'
	// <0x6eb006>|0x156|+0x017:'1281'
	// <0x6eb01d>|0x16d|+0x027:'1282'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6eb044>|0x194|+0x00f:'1287'
	// <0x6eb053>|0x1a3|+0x012:'1288'
	// <0x6eb065>|0x1b5|+0x005:'1289'
	// <0>
	// <0x6eb06a>|0x1ba|+0x0c9:'1291'
	// <0>
	// <0x6eb133>|0x283|+0x005:'1293'
	// <0x6eb138>|0x288|+0x00b:'1294'
	// <0x6eb143>|0x293|-0x036:'1295'
	// <0>
	// <1>
	// <2>
	// <0x6eb10d>|0x25d|+0x024:'1299'
	// <0x6eb131>|0x281|-0x0b3:'1300'
	// <0>
	// <1>
	// <0x6eb07e>|0x1ce|+0x005:'1303'
	// <0x6eb083>|0x1d3|+0x00b:'1304'
	// <0x6eb08e>|0x1de|+0x00d:'1305'
	// <0>
	// <0x6eb09b>|0x1eb|+0x005:'1307'
	// <0x6eb0a0>|0x1f0|+0x00c:'1308'
	// <0>
	// <0x6eb0ac>|0x1fc|+0x034:'1310'
	// <0x6eb0e0>|0x230|+0x02b:'1311'
	// <0>
	// <0x6eb10b>|0x25b|+0x05d:'1313'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6eb168>|0x2b8|+0x013:'1318'
	// <0x6eb17b>|0x2cb|+0x011:'1319'
	// <0x6eb18c>|0x2dc|+0x005:'1320'
	// <0>
	// <0x6eb191>|0x2e1|+0x120:'1322'
	// <0>
	// <0x6eb2b1>|0x401|+0x005:'1324'
	// <0x6eb2b6>|0x406|+0x007:'1325'
	// <0x6eb2bd>|0x40d|-0x02c:'1326'
	// <0>
	// <1>
	// <2>
	// <0x6eb291>|0x3e1|+0x01e:'1330'
	// <0x6eb2af>|0x3ff|-0x109:'1331'
	// <0>
	// <1>
	// <0x6eb1a6>|0x2f6|+0x005:'1334'
	// <0x6eb1ab>|0x2fb|+0x007:'1335'
	// <0x6eb1b2>|0x302|+0x00d:'1336'
	// <0>
	// <0x6eb1bf>|0x30f|+0x009:'1338'
	// <0x6eb1c8>|0x318|+0x00c:'1339'
	// <0>
	// <0x6eb1d4>|0x324|+0x031:'1341'
	// <0x6eb205>|0x355|+0x033:'1342'
	// <0x6eb238>|0x388|+0x004:'1343'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6eb23c>|0x38c|+0x03c:'1352'
	// <0x6eb278>|0x3c8|+0x005:'1353'
	// <0x6eb27d>|0x3cd|+0x007:'1354'
	// <0>
	// <0x6eb284>|0x3d4|+0x063:'1356'
	// <0x6eb2e7>|0x437|+0x007:'1356'
	// <0x6eb2ee>|0x43e|+0x02c:'1357'
	// <0>
	// <1>
	// <0x6eb31a>|0x46a|+0x002:'1360'
	// <0x6eb31c>|0x46c|+0x005:'1361'
	// <0x6eb321>|0x471|+0x007:'1362'
	// <0x6eb328>|0x478|+0x00f:'1363'
	// <0x6eb337>|0x487|+0x029:'1364'
	// <0x6eb360>|0x4b0|+0x004:'1365'
	// <0>
	// <1>
	// <0x6eb364>|0x4b4|+0x02d:'1368'
	// <0>
	// <1>
	// <2>
	// <0x6eb391>|0x4e1|+0x00d:'1372'
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
	// <0x6eb39e>|0x4ee|+0x03f:'1389'
	// <0>
	// <1>
	// <0x6eb3dd>|0x52d|+0x014:'1392'
	// <0x6eb3f1>|0x541|+0x00c:'1393'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6eb3fd>|0x54d|-0x07b:'1401'
	// <0>
	// <0x6eb382>|0x4d2|+0x09a:'1403'
	// <0x6eb41c>|0x56c|-0x095:'1403'
	// <0x6eb387>|0x4d7|+0x0b2:'1404'
	// ******
}

// STATE[STUB]
// void vostok::animation::mixing::n_ary_tree_transition_tree_constructor::merge_weight_synchronization_groups(vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node&, bool)
void n_ary_tree_transition_tree_constructor::merge_weight_synchronization_groups(
	n_ary_tree_animation_node*		from_begin,
	n_ary_tree_animation_node*		from_end,
	n_ary_tree_animation_node*		to_begin,
	n_ary_tree_animation_node*		to_end,
	n_ary_tree_animation_node&		new_weight_driving_animation,
	bool							is_new_driving_animation
)
{
	// LOCALS
	// bool 							new_driving_animation_in_old_target_found
	// animation_comparer_equal_predicate equal_predicate
	// animation_comparer_predicate 	comparer
	// n_ary_tree_animation_node** 		i
	// ******

	// FUNCTION BODY
	// <0>
	// <0x6ec0a3>|0x003|+0x12d:'1416'
	// <0x6ec1d0>|0x130|-0x119:'1416'
	// <0x6ec0b7>|0x017|+0x005:'1417'
	// <0>
	// <1>
	// <2>
	// <0x6ec0bc>|0x01c|+0x01a:'1421'
	// <0>
	// <0x6ec0d6>|0x036|+0x02c:'1423'
	// <0x6ec102>|0x062|-0x022:'1423'
	// <0x6ec0e0>|0x040|+0x011:'1424'
	// <0x6ec0f1>|0x051|+0x00b:'1425'
	// <0x6ec0fc>|0x05c|+0x002:'1426'
	// <0>
	// <0x6ec0fe>|0x05e|+0x00c:'1428'
	// <0>
	// <1>
	// <2>
	// <0x6ec10a>|0x06a|+0x006:'1432'
	// <0x6ec110>|0x070|+0x001:'1433'
	// <0>
	// <0x6ec111>|0x071|+0x04b:'1435'
	// <0>
	// <1>
	// <0x6ec15c>|0x0bc|+0x00b:'1438'
	// <0>
	// <1>
	// <0x6ec167>|0x0c7|+0x065:'1441'
	// <0x6ec1cc>|0x12c|-0x057:'1441'
	// <0x6ec175>|0x0d5|+0x026:'1442'
	// <0>
	// <0x6ec19b>|0x0fb|+0x015:'1444'
	// <0x6ec1b0>|0x110|-0x025:'1445'
	// <0>
	// <1>
	// <0x6ec18b>|0x0eb|+0x00e:'1448'
	// <0>
	// <0x6ec199>|0x0f9|+0x019:'1450'
	// <0>
	// <1>
	// <0x6ec1b2>|0x112|+0x017:'1453'
	// <0x6ec1c9>|0x129|+0x009:'1454'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ec1d2>|0x132|+0x004:'1461'
	// <0x6ec1d6>|0x136|+0x019:'1462'
	// <0>
	// <0x6ec1ef>|0x14f|+0x005:'1464'
	// <0x6ec1f4>|0x154|+0x016:'1465'
	// ******
}

// STATE[STUB]
// void vostok::animation::mixing::n_ary_tree_transition_tree_constructor::merge_weight_asynchronous_groups(vostok::animation::mixing::n_ary_tree_animation_node* const, vostok::animation::mixing::n_ary_tree_animation_node* const, vostok::animation::mixing::n_ary_tree_animation_node* const, vostok::animation::mixing::n_ary_tree_animation_node* const)
void n_ary_tree_transition_tree_constructor::merge_weight_asynchronous_groups(
	n_ary_tree_animation_node*		from_begin,
	n_ary_tree_animation_node*		from_end,
	n_ary_tree_animation_node*		to_begin,
	n_ary_tree_animation_node*		to_end
)
{
	// LOCALS
	// n_ary_tree_node_comparer 		comparer
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0x6ebfec>|0x00c|+0x010:'1478'
	// <0>
	// <0x6ebffc>|0x01c|+0x002:'1480'
	// <0x6ebffe>|0x01e|+0x059:'1481'
	// <0x6ec057>|0x077|-0x051:'1481'
	// <0x6ec006>|0x026|+0x022:'1482'
	// <0x6ec028>|0x048|+0x00e:'1482'
	// <0>
	// <0x6ec036>|0x056|+0x00b:'1484'
	// <0x6ec041>|0x061|+0x003:'1485'
	// <0x6ec044>|0x064|-0x020:'1486'
	// <0>
	// <1>
	// <0x6ec024>|0x044|+0x009:'1489'
	// <0x6ec02d>|0x04d|+0x007:'1489'
	// <0>
	// <0x6ec034>|0x054|+0x012:'1491'
	// <0>
	// <1>
	// <0x6ec046>|0x066|+0x00b:'1494'
	// <0x6ec051>|0x071|+0x003:'1495'
	// <0x6ec054>|0x074|+0x025:'1496'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6ec079>|0x099|+0x007:'1503'
	// <0x6ec080>|0x0a0|-0x025:'1504'
	// <0>
	// <0x6ec05b>|0x07b|+0x005:'1506'
	// <0x6ec060>|0x080|+0x010:'1507'
	// <0x6ec070>|0x090|+0x024:'1508'
	// ******
}

// STATE[STUB]
// void vostok::animation::mixing::n_ary_tree_transition_tree_constructor::change_weight_synchronization_group(vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*, vostok::animation::mixing::n_ary_tree_animation_node*)
void n_ary_tree_transition_tree_constructor::change_weight_synchronization_group(
	n_ary_tree_animation_node*		from_begin,
	n_ary_tree_animation_node*		from_end,
	n_ary_tree_animation_node*		to_begin,
	n_ary_tree_animation_node*		to_end
)
{
	// FUNCTION BODY
	// <0x6ec2e7>|0x007|+0x009:'1517'
	// <0x6ec2f0>|0x010|+0x015:'1518'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6ec305>|0x025|+0x008:'1524'
	// <0>
	// <0x6ec30d>|0x02d|+0x01d:'1526'
	// <0>
	// <0x6ec32a>|0x04a|-0x00a:'1528'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6ec320>|0x040|+0x010:'1537'
	// <0x6ec330>|0x050|-0x032:'1537'
	// <0x6ec2fe>|0x01e|+0x04f:'1538'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::n_ary_tree_animation_node* vostok::animation::mixing::n_ary_tree_transition_tree_constructor::get_time_driving_animation(unsigned int) const
n_ary_tree_animation_node* n_ary_tree_transition_tree_constructor::get_time_driving_animation( u32 time_synchronization_group_id ) const
{
	return NULL;

	// FUNCTION BODY
	// <0x6e97f0>|0x000|+0x010:'1542'
	// <0>
	// <0x6e9800>|0x010|+0x012:'1544'
	// <0x6e9812>|0x022|-0x004:'1545'
	// <0>
	// <1>
	// <2>
	// <0x6e980e>|0x01e|+0x003:'1549'
	// <0x6e9811>|0x021|+0x004:'1550'
	// ******
}

// STATE[STUB]
// void vostok::animation::mixing::n_ary_tree_transition_tree_constructor::merge_trees(vostok::animation::mixing::n_ary_tree const&, vostok::animation::mixing::n_ary_tree const&)
void n_ary_tree_transition_tree_constructor::merge_trees( n_ary_tree const& from, n_ary_tree const& to )
{
	// LOCALS
	// n_ary_tree_animation_node* 		i_begin
	// float 							animation_interval_time
	// animation_state_params const& 	params
	// u16 								initial_event_types
	// float 							animation_time_threshold
	// float 							weight
	// animation_interval const* 		time_driving_animation_interval
	// bool 							is_freezed
	// ******

	// FUNCTION BODY
	// <0x6ec366>|0x006|+0x003:'1557'
	// <0x6ec369>|0x009|+0x017:'1558'
	// <0x6ec380>|0x020|+0x006:'1559'
	// <0x6ec386>|0x026|+0x01d:'1560'
	// <0x6ec3a3>|0x043|+0x0ab:'1561'
	// <0x6ec44e>|0x0ee|-0x0a3:'1561'
	// <0x6ec3ab>|0x04b|+0x00a:'1562'
	// <0x6ec3b5>|0x055|+0x00b:'1563'
	// <0x6ec3c0>|0x060|+0x01e:'1564'
	// <0x6ec3de>|0x07e|+0x003:'1565'
	// <0>
	// <1>
	// <0x6ec3e1>|0x081|-0x001:'1568'
	// <0x6ec3e0>|0x080|+0x003:'1569'
	// <0x6ec3e3>|0x083|+0x00b:'1569'
	// <0x6ec3ee>|0x08e|+0x01e:'1570'
	// <0x6ec40c>|0x0ac|+0x002:'1571'
	// <0>
	// <1>
	// <0x6ec40e>|0x0ae|+0x00c:'1574'
	// <0x6ec41a>|0x0ba|+0x01a:'1575'
	// <0x6ec434>|0x0d4|+0x053:'1576'
	// <0>
	// <1>
	// <0x6ec487>|0x127|+0x009:'1579'
	// <0x6ec490>|0x130|+0x00b:'1580'
	// <0x6ec49b>|0x13b|-0x043:'1581'
	// <0>
	// <1>
	// <0x6ec458>|0x0f8|+0x02d:'1584'
	// <0x6ec485>|0x125|-0x025:'1584'
	// <0x6ec460>|0x100|+0x00c:'1585'
	// <0x6ec46c>|0x10c|+0x04e:'1586'
	// <0>
	// <1>
	// <0x6ec4ba>|0x15a|+0x038:'1589'
	// <0x6ec4f2>|0x192|-0x02e:'1589'
	// <0x6ec4c4>|0x164|+0x008:'1590'
	// <0x6ec4cc>|0x16c|+0x01b:'1591'
	// <0>
	// <0x6ec4e7>|0x187|+0x008:'1593'
	// <0x6ec4ef>|0x18f|+0x00a:'1594'
	// <0>
	// <1>
	// <2>
	// <0x6ec4f9>|0x199|+0x00f:'1598'
	// <0>
	// <0x6ec508>|0x1a8|+0x00b:'1600'
	// <0x6ec513>|0x1b3|+0x00c:'1601'
	// <0>
	// <0x6ec51f>|0x1bf|+0x041:'1603'
	// <0>
	// <0x6ec560>|0x200|+0x002:'1605'
	// <0x6ec562>|0x202|+0x00e:'1606'
	// <0x6ec570>|0x210|+0x00f:'1607'
	// <0>
	// <0x6ec57f>|0x21f|+0x011:'1609'
	// <0x6ec590>|0x230|+0x003:'1610'
	// <0x6ec593>|0x233|+0x007:'1611'
	// <0>
	// <1>
	// <2>
	// <0x6ec59a>|0x23a|+0x00a:'1615'
	// <0x6ec5a4>|0x244|+0x006:'1616'
	// <0x6ec5aa>|0x24a|+0x014:'1617'
	// <0x6ec5be>|0x25e|+0x009:'1618'
	// <0x6ec5c7>|0x267|+0x013:'1619'
	// <0x6ec5da>|0x27a|+0x003:'1620'
	// <0>
	// <0x6ec5dd>|0x27d|+0x008:'1622'
	// <0>
	// <0x6ec5e5>|0x285|+0x00c:'1624'
	// <0x6ec5f1>|0x291|+0x008:'1625'
	// <0>
	// <1>
	// <2>
	// <0x6ec5f9>|0x299|+0x022:'1629'
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
	// <0x6ec61b>|0x2bb|+0x042:'1643'
	// <0>
	// <0x6ec65d>|0x2fd|+0x030:'1645'
	// <0>
	// <0x6ec68d>|0x32d|+0x004:'1647'
	// <0x6ec691>|0x331|+0x015:'1648'
	// <0>
	// <1>
	// <0x6ec6a6>|0x346|+0x017:'1651'
	// <0x6ec6bd>|0x35d|-0x00f:'1651'
	// <0x6ec6ae>|0x34e|+0x012:'1652'
	// <0x6ec6c0>|0x360|-0x00f:'1652'
	// <0x6ec6b1>|0x351|+0x012:'1653'
	// ******
}

// STATE[STUB]
n_ary_tree_transition_tree_constructor::n_ary_tree_transition_tree_constructor(
	mutable_buffer&					buffer,
	n_ary_tree const&				from,
	n_ary_tree const&				to,
	u32								animations_count,
	u32								animated_objects_count,
	u32								current_time_in_ms,
	subscribed_channel*&			channels_head,
	transform_functor_type const&	get_transform_functor
) :
	m_get_transform_functor			( get_transform_functor ),
	m_cloner						( *this, current_time_in_ms ),
	m_buffer						( buffer ),
	m_from							( from ),
	m_to							( to ),
	m_weight_root					( NULL ),
	m_time_root						( NULL ),
	m_time_driving_animations_begin	( NULL ),
	m_time_driving_animations_end	( NULL ),
	m_animation_states				( NULL ),
	// m_new_animation_state
	// m_animation_events
	m_animated_objects				( NULL ),
	m_new_animated_object			( NULL ),
	// m_new_animation_event
	m_reference_counter				( NULL ),
	m_previous_animation			( NULL ),
	m_channels_head					( channels_head ),
	m_current_time_in_ms			( current_time_in_ms ),
	m_animations_count				( animations_count ),
	m_animated_objects_count		( animated_objects_count )
{
	// FUNCTION BODY
	// <0>
	// <1>
	// <0x6ec760>|0x090|+0x005:'1699'
	// <0x6ec765>|0x095|+0x007:'1700'
	// <0x6ec76c>|0x09c|+0x009:'1701'
	// <0>
	// <0x6ec775>|0x0a5|+0x00d:'1703'
	// <0>
	// <1>
	// <0x6ec782>|0x0b2|+0x015:'1706'
	// <0x6ec797>|0x0c7|+0x006:'1707'
	// <0>
	// <0x6ec79d>|0x0cd|+0x002:'1709'
	// <0x6ec79f>|0x0cf|+0x00f:'1710'
	// <0x6ec7ae>|0x0de|+0x006:'1711'
	// <0>
	// <1>
	// <0x6ec7b4>|0x0e4|+0x013:'1714'
	// <0x6ec7c7>|0x0f7|+0x006:'1715'
	// <0>
	// <0x6ec7cd>|0x0fd|+0x005:'1717'
	// <0>
	// <1>
	// <0x6ec7d2>|0x102|+0x015:'1720'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// claude@NOTE: structure matches (single `return n_ary_tree( <11 members> )`, mirror of
// the matched sibling n_ary_tree_transition_constructor::computed_tree). Byte residual: the
// 11-arg n_ary_tree constructor is still an empty STUB (mixing_n_ary_tree.cpp), so the base
// INLINES it to nothing - only the implicit intrusive_ptr member-ctor survives, dropping all
// 11 arg pushes + the out-of-line `call`. Reappears once that constructor gets a real body.
n_ary_tree n_ary_tree_transition_tree_constructor::computed_tree( )
{
	return
		n_ary_tree(
			m_weight_root,
			m_time_root,
			m_cloner.interpolators(),
			m_animation_states,
			m_animation_events,
			m_animated_objects,
			m_reference_counter,
			m_animations_count,
			m_animated_objects_count,
			m_cloner.interpolators_count(),
			m_current_time_in_ms
		);
}

} // namespace mixing
} // namespace animation
} // namespace vostok