////////////////////////////////////////////////////////////////////////////
//	Created		: 04.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "mixing_n_ary_tree_comparer.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_time_scale_transition_node.h"
#include <vostok/animation/mixing_n_ary_tree.h>
#include "mixing_n_ary_tree_node_comparer.h"
#include "mixing_n_ary_tree_size_calculator.h"
#include "interpolator_size_calculator.h"
#include "mixing_n_ary_tree_interpolator_selector.h"
#include "mixing_n_ary_tree_time_scale_calculator.h"
#include "mixing_n_ary_tree_transition_tree_constructor_impl.h"
#include "mixing_n_ary_tree_target_time_scale_calculator.h"

#include <vostok/animation/mixing_animated_object_holder.h>

namespace vostok {
namespace animation {
namespace mixing {

// STATE[UNCHECKED]
void n_ary_tree_comparer::increase_buffer_size( n_ary_tree_base_node& node )
{
	n_ary_tree_size_calculator	calculator( this );
	node.accept					( calculator );

	// FUNCTION BODY[0x56d880]: 2
	// <0>
	// <0x56d883>|0x003|+0x028:'51'
	// ******
}

// STATE[UNCHECKED]
bool n_ary_tree_comparer::equal( ) const
{
	return						m_equal;

	// FUNCTION BODY[0x56d860]: 1
	// <0x56d860>|0x000|+0x003:'63'
	// ******
}

// STATE[STUB]
u32 n_ary_tree_comparer::needed_buffer_size( ) const
{
	return						m_needed_buffer_size;

	// FUNCTION BODY[0x56d850]: 1
	// <0x56d850>|0x000|+0x003:'68'
	// ******
}

// STATE[STUB]
comparison_result_enum animation_comparer_predicate::operator()(
	n_ary_tree_animation_node const& left,
	n_ary_tree_animation_node const& right
) const
{
	// FUNCTION BODY[0x56e2e0]: 78
	// <0x56e2e2>|0x002|+0x013:'76'
	// <0x56e2f5>|0x015|+0x006:'77'
	// <0>
	// <0x56e2fb>|0x01b|+0x004:'79'
	// <0x56e2ff>|0x01f|+0x009:'80'
	// <0>
	// <0x56e308>|0x028|+0x008:'82'
	// <0x56e310>|0x030|+0x002:'83'
	// <0>
	// <1>
	// <0x56e312>|0x032|+0x002:'86'
	// <0>
	// <0x56e314>|0x034|+0x008:'88'
	// <0x56e31c>|0x03c|+0x002:'89'
	// <0>
	// <1>
	// <0x56e31e>|0x03e|+0x002:'92'
	// <0>
	// <0x56e320>|0x040|+0x008:'94'
	// <0x56e328>|0x048|+0x002:'95'
	// <0>
	// <1>
	// <0x56e32a>|0x04a|+0x002:'98'
	// <0>
	// <0x56e32c>|0x04c|+0x008:'100'
	// <0x56e334>|0x054|+0x002:'101'
	// <0>
	// <1>
	// <0x56e336>|0x056|+0x002:'104'
	// <0>
	// <0x56e338>|0x058|+0x011:'106'
	// <0x56e349>|0x069|+0x00a:'107'
	// <0x56e353>|0x073|+0x009:'108'
	// <0x56e35c>|0x07c|+0x011:'109'
	// <0x56e36d>|0x08d|+0x002:'110'
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
	// <0x56e36f>|0x08f|+0x008:'126'
	// <0x56e377>|0x097|+0x002:'127'
	// <0>
	// <1>
	// <0x56e379>|0x099|+0x002:'130'
	// <0>
	// <0x56e37b>|0x09b|+0x017:'132'
	// <0>
	// <1>
	// <0x56e392>|0x0b2|+0x008:'135'
	// <0x56e39a>|0x0ba|+0x006:'136'
	// <0>
	// <0x56e3a0>|0x0c0|-0x017:'138'
	// <0x56e389>|0x0a9|+0x02e:'139'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56e3b7>|0x0d7|+0x006:'145'
	// <0x56e3bd>|0x0dd|+0x00a:'146'
	// <0x56e3c7>|0x0e7|+0x002:'147'
	// <0>
	// <0x56e3c9>|0x0e9|+0x008:'149'
	// <0x56e3d1>|0x0f1|+0x008:'150'
	// <0>
	// <1>
	// <0x56e3d9>|0x0f9|-0x0d4:'153'
	// <0x56e305>|0x025|+0x08a:'154'
	// <0x56e38f>|0x0af|+0x04d:'154'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::process_interpolators( n_ary_tree const& from, n_ary_tree const& to )
{
	// LOCALS
	// const u32 						from_interpolators_count
	// base_interpolator const* const* const to_interpolators_begin
	// interpolator_size_calculator 	size_calculator
	// ******

	// CALL SITE INFO
	// <0x56e022> -> void < unknown >( interpolator_visitor& ) const
	// ******

	// FUNCTION BODY[0x56df70]: 29
	// <0x56df76>|0x006|+0x004:'161'
	// <0>
	// <0x56df7a>|0x00a|+0x008:'163'
	// <0x56df82>|0x012|+0x006:'164'
	// <0>
	// <1>
	// <0x56df88>|0x018|+0x00f:'167'
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
	// <0x56df97>|0x027|+0x04a:'180'
	// <0>
	// <1>
	// <0x56dfe1>|0x071|+0x021:'183'
	// <0>
	// <0x56e002>|0x092|+0x011:'185'
	// <0x56e013>|0x0a3|+0x018:'186'
	// <0x56e02b>|0x0bb|-0x014:'186'
	// <0x56e017>|0x0a7|+0x017:'187'
	// <0>
	// <0x56e02e>|0x0be|+0x00a:'189'
	// ******
}

// STATE[STUB]
bool n_ary_tree_comparer::new_time_scale( n_ary_tree_animation_node& new_time_driving_animation )
{
	// LOCALS
	// n_ary_tree_time_scale_calculator time_scale_calculator
	// const float 						directional_time_scale_factor
	// const float 						new_time_driving_animation_target_time_scale
	// const float 						time_scale_factor
	// n_ary_tree_base_node* const 		target_time_scale_node
	// const float 						new_driving_animation_length
	// ******

	// CALL SITE INFO
	// <0x56dd64> -> bool < unknown >()
	// <0x56dd78> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56dd85> -> bool < unknown >()
	// <0x56ddbb> -> float < unknown >() const
	// <0x56ddd9> -> bool < unknown >()
	// <0x56de12> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56de7e> -> bool < unknown >()
	// <0x56df39> -> bool < unknown >()
	// ******

	return false;

	// FUNCTION BODY[0x56dc60]: 92
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56dc60>|0x000|+0x02a:'199'
	// <0x56dc8a>|0x02a|-0x00a:'199'
	// <0x56dc80>|0x020|+0x01d:'200'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56dc9d>|0x03d|-0x00c:'206'
	// <0x56dc91>|0x031|+0x217:'207'
	// <0x56dea8>|0x248|-0x203:'207'
	// <0>
	// <0x56dca5>|0x045|+0x00b:'209'
	// <0x56dcb0>|0x050|+0x00e:'210'
	// <0x56dcbe>|0x05e|+0x002:'211'
	// <0x56dcc0>|0x060|+0x002:'212'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x56dcc2>|0x062|+0x003:'221'
	// <0x56dcc5>|0x065|+0x015:'222'
	// <0>
	// <1>
	// <0x56dcda>|0x07a|+0x00b:'225'
	// <0>
	// <1>
	// <0x56dce5>|0x085|+0x004:'228'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56dce9>|0x089|+0x024:'233'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x56dd0d>|0x0ad|+0x040:'240'
	// <0x56dd4d>|0x0ed|+0x046:'241'
	// <0x56dd93>|0x133|-0x03a:'241'
	// <0x56dd59>|0x0f9|+0x03e:'242'
	// <0x56dd97>|0x137|+0x004:'242'
	// <0x56dd9b>|0x13b|-0x031:'243'
	// <0x56dd6a>|0x10a|+0x010:'244'
	// <0x56dd7a>|0x11a|+0x023:'245'
	// <0x56dd9d>|0x13d|+0x008:'245'
	// <0>
	// <0x56dda5>|0x145|+0x011:'247'
	// <0x56ddb6>|0x156|+0x015:'248'
	// <0>
	// <1>
	// <0x56ddcb>|0x16b|+0x04b:'251'
	// <0x56de16>|0x1b6|-0x034:'251'
	// <0x56dde2>|0x182|+0x004:'252'
	// <0x56dde6>|0x186|+0x032:'253'
	// <0x56de18>|0x1b8|+0x004:'253'
	// <0>
	// <1>
	// <0x56de1c>|0x1bc|+0x046:'256'
	// <0>
	// <1>
	// <0x56de62>|0x202|+0x022:'259'
	// <0>
	// <0x56de84>|0x224|+0x004:'261'
	// <0x56de88>|0x228|+0x004:'262'
	// <0>
	// <1>
	// <0x56de8c>|0x22c|+0x004:'265'
	// <0x56de90>|0x230|+0x010:'266'
	// <0>
	// <1>
	// <0x56dea0>|0x240|+0x014:'269'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56deb4>|0x254|+0x017:'274'
	// <0>
	// <0x56decb>|0x26b|+0x008:'276'
	// <0>
	// <0x56ded3>|0x273|+0x040:'278'
	// <0x56df13>|0x2b3|+0x03e:'279'
	// <0x56df51>|0x2f1|-0x0bd:'279'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56de94>|0x234|-0x201:'285'
	// <0x56dc93>|0x033|+0x203:'286'
	// <0x56de96>|0x236|+0x014:'286'
	// <0x56deaa>|0x24a|+0x09d:'286'
	// <0x56df47>|0x2e7|+0x00f:'286'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::new_animation(
	n_ary_tree_animation_node&		animation,
	u32&							time_scale_operands_count,
	u32&							operands_offset
)
{
	// FUNCTION BODY[0x56e250]: 19
	// <0>
	// <0x56e25a>|0x00a|+0x016:'295'
	// <0x56e270>|0x020|+0x00b:'296'
	// <0x56e27b>|0x02b|+0x00a:'297'
	// <0x56e285>|0x035|+0x006:'298'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x56e28b>|0x03b|+0x003:'305'
	// <0>
	// <1>
	// <0x56e28e>|0x03e|+0x003:'308'
	// <0>
	// <0x56e291>|0x041|+0x022:'310'
	// <0x56e2b3>|0x063|+0x004:'311'
	// <0x56e2b7>|0x067|+0x01d:'312'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::new_weight_transition( float from, float to )
{
	// FUNCTION BODY[0x56d870]: 6
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56d870>|0x000|+0x004:'321'
	// <0x56d874>|0x004|+0x004:'322'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::remove_animation(
	n_ary_tree_animation_node&			animation,
	n_ary_tree_animation_node const*	weight_driving_animation,
	bool								is_new_driving_animation
)
{
	// LOCALS
	// u32 								operands_offset
	// u32 								operands_offset
	// u32 								time_scale_operands_count
	// u32 								time_scale_operands_count
	// const u32 						operands_count
	// ******

	// CALL SITE INFO
	// <0x56ec0d> -> float < unknown >() const
	// <0x56ec43> -> bool < unknown >()
	// <0x56ecbf> -> bool < unknown >()
	// <0x56ecf1> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56ed27> -> bool < unknown >()
	// <0x56edb0> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56edc3> -> bool < unknown >()
	// <0x56edff> -> bool < unknown >()
	// <0x56ee38> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56ee56> -> bool < unknown >()
	// <0x56ee81> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY[0x56ebf0]: 88
	// <0x56ebf5>|0x005|+0x011:'331'
	// <0x56ec06>|0x016|+0x01b:'332'
	// <0>
	// <1>
	// <0x56ec21>|0x031|+0x015:'335'
	// <0x56ec36>|0x046|+0x01c:'336'
	// <0x56ec52>|0x062|+0x003:'337'
	// <0>
	// <0x56ec55>|0x065|+0x00e:'339'
	// <0x56ec63>|0x073|+0x00d:'340'
	// <0x56ec70>|0x080|+0x00e:'341'
	// <0x56ec7e>|0x08e|+0x004:'342'
	// <0x56ec82>|0x092|+0x008:'343'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x56ec8a>|0x09a|+0x010:'351'
	// <0x56ec9a>|0x0aa|+0x004:'352'
	// <0>
	// <1>
	// <2>
	// <0x56ec9e>|0x0ae|+0x010:'356'
	// <0>
	// <1>
	// <0x56ecae>|0x0be|+0x009:'359'
	// <0x56ecb7>|0x0c7|+0x015:'360'
	// <0>
	// <1>
	// <0x56eccc>|0x0dc|+0x02e:'363'
	// <0>
	// <1>
	// <0x56ecfa>|0x10a|+0x017:'366'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56ed11>|0x121|+0x004:'371'
	// <0>
	// <1>
	// <0x56ed15>|0x125|+0x024:'374'
	// <0x56ed39>|0x149|+0x008:'375'
	// <0x56ed41>|0x151|-0x0c5:'376'
	// <0x56ec7c>|0x08c|+0x0d4:'377'
	// <0x56ed50>|0x160|+0x00e:'377'
	// <0x56ed5e>|0x16e|+0x004:'378'
	// <0x56ed62>|0x172|+0x004:'379'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x56ed66>|0x176|+0x010:'387'
	// <0>
	// <1>
	// <0x56ed76>|0x186|+0x00e:'390'
	// <0>
	// <0x56ed84>|0x194|+0x00b:'392'
	// <0x56ed8f>|0x19f|+0x023:'393'
	// <0>
	// <0x56edb2>|0x1c2|+0x004:'395'
	// <0>
	// <0x56edb6>|0x1c6|+0x021:'397'
	// <0x56edd7>|0x1e7|+0x0b9:'398'
	// <0>
	// <0x56ee90>|0x2a0|-0x042:'400'
	// <0>
	// <1>
	// <2>
	// <0x56ee4e>|0x25e|-0x068:'404'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56ede6>|0x1f6|+0x00a:'409'
	// <0x56edf0>|0x200|+0x003:'410'
	// <0x56edf3>|0x203|+0x021:'411'
	// <0x56ee14>|0x224|+0x02d:'412'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56ee41>|0x251|+0x042:'418'
	// <0x56ee83>|0x293|+0x011:'418'
	// <0x56ee94>|0x2a4|-0x18c:'418'
	// <0x56ed08>|0x118|+0x13d:'419'
	// <0x56ee45>|0x255|+0x042:'419'
	// <0x56ee87>|0x297|+0x011:'419'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::remove_weight_synchronization_group( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end )
{
	// CALL SITE INFO
	// <0x56f143> -> float < unknown >() const
	// ******

	// FUNCTION BODY[0x56f130]: 13
	// <0x56f137>|0x007|+0x01c:'426'
	// <0x56f153>|0x023|+0x00c:'427'
	// <0>
	// <1>
	// <2>
	// <0x56f15f>|0x02f|+0x014:'431'
	// <0x56f173>|0x043|+0x003:'432'
	// <0x56f176>|0x046|+0x010:'433'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56f186>|0x056|-0x02f:'438'
	// <0x56f157>|0x027|+0x04c:'439'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::add_animation(
	n_ary_tree_animation_node&			animation,
	n_ary_tree_animation_node* const	weight_driving_animation
)
{
	// LOCALS
	// u32 								time_scale_operands_count
	// u32 								operands_offset
	// ******

	// CALL SITE INFO
	// <0x56eb13> -> float < unknown >() const
	// <0x56eb5d> -> bool < unknown >()
	// <0x56ebb7> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56ebc9> -> float < unknown >() const
	// ******

	// FUNCTION BODY[0x56eaf0]: 21
	// <0>
	// <0x56eaf8>|0x008|+0x004:'447'
	// <0>
	// <0x56eafc>|0x00c|+0x00c:'449'
	// <0>
	// <0x56eb08>|0x018|+0x027:'451'
	// <0>
	// <1>
	// <0x56eb2f>|0x03f|+0x01a:'454'
	// <0>
	// <1>
	// <0x56eb49>|0x059|+0x02c:'457'
	// <0>
	// <0x56eb75>|0x085|+0x00a:'459'
	// <0>
	// <0x56eb7f>|0x08f|+0x007:'461'
	// <0x56eb86>|0x096|+0x00d:'462'
	// <0x56eb93>|0x0a3|+0x02d:'463'
	// <0>
	// <0x56ebc0>|0x0d0|+0x019:'465'
	// <0x56ebd9>|0x0e9|+0x008:'466'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::add_weight_synchronization_group( n_ary_tree_animation_node* begin, n_ary_tree_animation_node* end )
{
	// FUNCTION BODY[0x56f0f0]: 5
	// <0x56f0ff>|0x00f|+0x004:'474'
	// <0>
	// <0x56f103>|0x013|+0x00a:'476'
	// <0x56f10d>|0x01d|+0x004:'477'
	// <0x56f111>|0x021|+0x010:'478'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::new_weight_driving_animation( n_ary_tree_animation_node& animation )
{
	// LOCALS
	// u32 								time_scale_operands_count
	// n_ary_tree_weight_node 			temp
	// u32 								operands_offset
	// n_ary_tree_node_comparer 		comparer
	// base_interpolator const& 		interpolator
	// ******

	// CALL SITE INFO
	// <0x56e973> -> bool < unknown >()
	// <0x56e98d> -> float < unknown >() const
	// <0x56ea5a> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56ea6c> -> float < unknown >() const
	// <0x56ea96> -> float < unknown >() const
	// <0x56ead6> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY[0x56e950]: 39
	// <0x56e959>|0x009|+0x004:'483'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56e95d>|0x00d|+0x025:'488'
	// <0>
	// <0x56e982>|0x032|+0x027:'490'
	// <0>
	// <0x56e9a9>|0x059|+0x014:'492'
	// <0>
	// <1>
	// <0x56e9bd>|0x06d|+0x004:'495'
	// <0>
	// <0x56e9c1>|0x071|+0x015:'497'
	// <0>
	// <1>
	// <2>
	// <0x56e9d6>|0x086|+0x003:'501'
	// <0x56e9d9>|0x089|+0x08a:'502'
	// <0x56ea63>|0x113|-0x04f:'502'
	// <0x56ea14>|0x0c4|+0x022:'503'
	// <0x56ea36>|0x0e6|+0x057:'504'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x56ea8d>|0x13d|+0x019:'511'
	// <0x56eaa6>|0x156|+0x008:'512'
	// <0>
	// <0x56eaae>|0x15e|+0x004:'514'
	// <0x56eab2>|0x162|-0x04b:'515'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56ea67>|0x117|+0x015:'520'
	// <0x56ea7c>|0x12c|+0x008:'521'
	// <0x56ea84>|0x134|+0x05b:'522'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::new_time_scale_transition( n_ary_tree_base_node& from, n_ary_tree_base_node& to )
{
	// CALL SITE INFO
	// <0x56dbc2> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56dbd7> -> float < unknown >() const
	// <0x56dc0b> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56dc1d> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56dc43> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY[0x56db70]: 16
	// <0x56db79>|0x009|+0x024:'529'
	// <0x56db9d>|0x02d|+0x02f:'530'
	// <0>
	// <1>
	// <2>
	// <0x56dbcc>|0x05c|+0x003:'534'
	// <0>
	// <0x56dbcf>|0x05f|+0x003:'536'
	// <0x56dbd2>|0x062|+0x00b:'537'
	// <0x56dbdd>|0x06d|+0x038:'538'
	// <0>
	// <1>
	// <2>
	// <0x56dc15>|0x0a5|+0x00a:'542'
	// <0x56dc1f>|0x0af|+0x026:'543'
	// <0x56dc45>|0x0d5|-0x081:'544'
	// <0x56dbc4>|0x054|+0x049:'545'
	// <0x56dc0d>|0x09d|+0x03c:'545'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::new_time_scale_transition( n_ary_tree_base_node& from, float to )
{
	// LOCALS
	// n_ary_tree_interpolator_selector interpolator_selector
	// ******

	// CALL SITE INFO
	// <0x56db0e> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56db19> -> float < unknown >() const
	// <0x56db5d> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY[0x56daf0]: 15
	// <0>
	// <0x56daf3>|0x003|+0x01d:'553'
	// <0x56db10>|0x020|+0x019:'554'
	// <0>
	// <1>
	// <2>
	// <0x56db29>|0x039|+0x008:'558'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56db31>|0x041|+0x004:'563'
	// <0x56db35>|0x045|+0x02a:'564'
	// <0>
	// <0x56db5f>|0x06f|-0x032:'566'
	// <0x56db2d>|0x03d|+0x036:'567'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::new_time_scale_transition( float from, n_ary_tree_base_node& to )
{
	// CALL SITE INFO
	// <0x56da9f> -> float < unknown >() const
	// <0x56dad7> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56dae6> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY[0x56da90]: 13
	// <0>
	// <1>
	// <0x56da93>|0x003|+0x004:'576'
	// <0>
	// <0x56da97>|0x007|+0x003:'578'
	// <0x56da9a>|0x00a|+0x00b:'579'
	// <0x56daa5>|0x015|+0x038:'580'
	// <0>
	// <1>
	// <2>
	// <0x56dadd>|0x04d|+0x004:'584'
	// <0x56dae1>|0x051|+0x007:'585'
	// <0x56dae8>|0x058|-0x00f:'586'
	// <0x56dad9>|0x049|+0x013:'587'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::new_weight_transition( n_ary_tree_base_node& from, n_ary_tree_base_node& to )
{
	// CALL SITE INFO
	// <0x56da16> -> float < unknown >() const
	// <0x56da4a> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56da5c> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56da82> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY[0x56da00]: 10
	// <0x56da04>|0x004|+0x005:'594'
	// <0>
	// <0x56da09>|0x009|+0x013:'596'
	// <0x56da1c>|0x01c|+0x038:'597'
	// <0>
	// <1>
	// <2>
	// <0x56da54>|0x054|+0x00a:'601'
	// <0x56da5e>|0x05e|+0x026:'602'
	// <0x56da84>|0x084|-0x038:'603'
	// <0x56da4c>|0x04c|+0x03c:'604'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::new_weight_transition(
	base_interpolator const&	from_animation_interpolator,
	n_ary_tree_base_node&		from,
	float						to
)
{
	// LOCALS
	// n_ary_tree_weight_node 			weight
	// ******

	// CALL SITE INFO
	// <0x56d960> -> float < unknown >() const
	// <0x56d9a7> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY[0x56d950]: 14
	// <0x56d958>|0x008|+0x018:'612'
	// <0>
	// <0x56d970>|0x020|+0x00f:'614'
	// <0>
	// <1>
	// <2>
	// <0x56d97f>|0x02f|+0x02a:'618'
	// <0>
	// <0x56d9a9>|0x059|+0x042:'620'
	// <0x56d9eb>|0x09b|-0x077:'621'
	// <0>
	// <0x56d974>|0x024|+0x079:'623'
	// <0>
	// <0x56d9ed>|0x09d|-0x075:'625'
	// <0x56d978>|0x028|+0x07d:'626'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::new_weight_transition(
	base_interpolator const&	to_animation_interpolator,
	float						from,
	n_ary_tree_base_node&		to
)
{
	// LOCALS
	// n_ary_tree_weight_node 			weight
	// ******

	// CALL SITE INFO
	// <0x56d8db> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56d92e> -> float < unknown >() const
	// ******

	// FUNCTION BODY[0x56d8b0]: 12
	// <0x56d8b3>|0x003|+0x02a:'634'
	// <0x56d8dd>|0x02d|+0x00c:'635'
	// <0x56d8e9>|0x039|+0x037:'636'
	// <0x56d920>|0x070|+0x002:'637'
	// <0>
	// <0x56d922>|0x072|+0x004:'639'
	// <0>
	// <0x56d926>|0x076|+0x016:'641'
	// <0x56d93c>|0x08c|+0x002:'642'
	// <0>
	// <1>
	// <0x56d93e>|0x08e|+0x004:'645'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::add_operands(
	n_ary_tree_animation_node&		from,
	n_ary_tree_animation_node&		to,
	const bool						skip_time_scale_node
)
{
	// LOCALS
	// n_ary_tree_interpolator_selector interpolator_selector
	// n_ary_tree_base_node* const* const i_e
	// n_ary_tree_base_node* const* const j_e
	// n_ary_tree_node_comparer 		comparer
	// ******

	// CALL SITE INFO
	// <0x56e096> -> bool < unknown >()
	// <0x56e0a9> -> bool < unknown >()
	// <0x56e0ef> -> bool < unknown >()
	// <0x56e177> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56e185> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56e197> -> void < unknown >( n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY[0x56e050]: 86
	// <0>
	// <0x56e053>|0x003|+0x009:'655'
	// <0>
	// <0x56e05c>|0x00c|+0x004:'657'
	// <0x56e060>|0x010|+0x00c:'658'
	// <0>
	// <0x56e06c>|0x01c|+0x01e:'660'
	// <0>
	// <0x56e08a>|0x03a|+0x012:'662'
	// <0x56e09c>|0x04c|+0x013:'663'
	// <0x56e0af>|0x05f|+0x007:'664'
	// <0x56e0b6>|0x066|+0x00f:'665'
	// <0x56e0c5>|0x075|+0x003:'666'
	// <0>
	// <1>
	// <0x56e0c8>|0x078|+0x002:'669'
	// <0x56e0ca>|0x07a|+0x007:'670'
	// <0x56e0d1>|0x081|+0x00c:'671'
	// <0x56e0dd>|0x08d|+0x003:'672'
	// <0>
	// <1>
	// <0x56e0e0>|0x090|+0x002:'675'
	// <0x56e0e2>|0x092|+0x013:'676'
	// <0x56e0f5>|0x0a5|+0x007:'677'
	// <0x56e0fc>|0x0ac|+0x00b:'678'
	// <0x56e107>|0x0b7|+0x003:'679'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56e10a>|0x0ba|+0x0e7:'685'
	// <0x56e1f1>|0x1a1|-0x0c1:'685'
	// <0x56e130>|0x0e0|+0x018:'686'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56e148>|0x0f8|+0x031:'692'
	// <0>
	// <1>
	// <0x56e179>|0x129|+0x002:'695'
	// <0>
	// <1>
	// <0x56e17b>|0x12b|+0x00c:'698'
	// <0>
	// <1>
	// <0x56e187>|0x137|+0x012:'701'
	// <0x56e199>|0x149|+0x004:'702'
	// <0>
	// <0x56e19d>|0x14d|+0x02d:'704'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x56e1ca>|0x17a|+0x00d:'711'
	// <0x56e1d7>|0x187|+0x003:'712'
	// <0x56e1da>|0x18a|-0x01e:'713'
	// <0>
	// <1>
	// <0x56e1bc>|0x16c|+0x00c:'716'
	// <0x56e1c8>|0x178|+0x014:'717'
	// <0>
	// <1>
	// <0x56e1dc>|0x18c|+0x00f:'720'
	// <0x56e1eb>|0x19b|+0x003:'721'
	// <0x56e1ee>|0x19e|+0x00f:'722'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x56e1fd>|0x1ad|+0x006:'729'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56e203>|0x1b3|+0x01d:'735'
	// <0>
	// <1>
	// <0x56e220>|0x1d0|+0x006:'738'
	// <0x56e226>|0x1d6|+0x01c:'739'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::new_weight_driving_animation(
	n_ary_tree_animation_node&		new_weight_driving_animation,
	n_ary_tree_animation_node&		new_driving_animation_in_previous_target
)
{
	// LOCALS
	// interpolator_comparer 			interpolator_comparer
	// u32 								operands_offset
	// u32 								time_scale_operands_count
	// ******

	// CALL SITE INFO
	// <0x56e8a2> -> void < unknown >( interpolator_comparer&, base_interpolator const& ) const
	// ******

	// FUNCTION BODY[0x56e860]: 31
	// <0x56e868>|0x008|+0x006:'747'
	// <0x56e86e>|0x00e|+0x004:'748'
	// <0>
	// <0x56e872>|0x012|+0x01b:'750'
	// <0x56e88d>|0x02d|+0x004:'751'
	// <0>
	// <1>
	// <0x56e891>|0x031|+0x013:'754'
	// <0x56e8a4>|0x044|+0x019:'755'
	// <0>
	// <0x56e8bd>|0x05d|+0x006:'757'
	// <0>
	// <1>
	// <2>
	// <0x56e8c3>|0x063|+0x02d:'761'
	// <0>
	// <1>
	// <0x56e8f0>|0x090|+0x00d:'764'
	// <0>
	// <0x56e8fd>|0x09d|+0x007:'766'
	// <0>
	// <0x56e904>|0x0a4|+0x014:'768'
	// <0>
	// <1>
	// <0x56e918>|0x0b8|+0x011:'771'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56e929>|0x0c9|+0x014:'777'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::change_animation(
	n_ary_tree_animation_node&		from,
	n_ary_tree_animation_node&		to,
	bool							is_new_driving_animation
)
{
	// LOCALS
	// interpolator_comparer 			interpolator_comparer
	// u32 								left_multiplicands_count
	// n_ary_tree_base_node** 			to_begin
	// u32 								operands_offset
	// float 							left_weight
	// float 							right_weight
	// n_ary_tree_base_node** const 	to_end
	// u32 								time_scale_operands_count
	// u32 								operands_offset
	// n_ary_tree_base_node** const 	from_end
	// u32 								time_scale_operands_count
	// bool 							right_is_pure_weight
	// bool 							left_is_pure_weight
	// ******

	// CALL SITE INFO
	// <0x56e440> -> bool < unknown >()
	// <0x56e457> -> bool < unknown >()
	// <0x56e4a1> -> bool < unknown >()
	// <0x56e4b8> -> bool < unknown >()
	// <0x56e4f0> -> bool < unknown >()
	// <0x56e51e> -> bool < unknown >()
	// <0x56e568> -> bool < unknown >()
	// <0x56e5a6> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56e5cd> -> bool < unknown >()
	// <0x56e617> -> bool < unknown >()
	// <0x56e636> -> bool < unknown >()
	// <0x56e644> -> bool < unknown >()
	// <0x56e6bd> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56e73e> -> void < unknown >( n_ary_tree_visitor& )
	// <0x56e748> -> bool < unknown >()
	// <0x56e756> -> bool < unknown >()
	// <0x56e7b9> -> void < unknown >( interpolator_comparer&, base_interpolator const& ) const
	// ******

	// FUNCTION BODY[0x56e3e0]: 153
	// <0x56e3ed>|0x00d|+0x026:'786'
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
	// <0x56e413>|0x033|+0x004:'798'
	// <0>
	// <1>
	// <0x56e417>|0x037|+0x003:'801'
	// <0>
	// <1>
	// <0x56e41a>|0x03a|+0x01b:'804'
	// <0>
	// <0x56e435>|0x055|+0x03a:'806'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56e46f>|0x08f|+0x010:'811'
	// <0>
	// <1>
	// <0x56e47f>|0x09f|+0x00e:'814'
	// <0>
	// <0x56e48d>|0x0ad|+0x007:'816'
	// <0x56e494>|0x0b4|+0x013:'817'
	// <0x56e4a7>|0x0c7|+0x017:'818'
	// <0x56e4be>|0x0de|+0x011:'819'
	// <0>
	// <0x56e4cf>|0x0ef|+0x002:'821'
	// <0x56e4d1>|0x0f1|+0x00c:'822'
	// <0>
	// <0x56e4dd>|0x0fd|+0x002:'824'
	// <0x56e4df>|0x0ff|+0x017:'825'
	// <0x56e4f6>|0x116|+0x00e:'826'
	// <0>
	// <1>
	// <2>
	// <0x56e504>|0x124|+0x00f:'830'
	// <0x56e513>|0x133|+0x011:'831'
	// <0x56e524>|0x144|+0x005:'832'
	// <0>
	// <0x56e529>|0x149|+0x008:'834'
	// <0>
	// <1>
	// <0x56e531>|0x151|+0x13b:'837'
	// <0>
	// <1>
	// <0x56e66c>|0x28c|+0x012:'840'
	// <0x56e67e>|0x29e|-0x05a:'841'
	// <0>
	// <1>
	// <2>
	// <0x56e624>|0x244|+0x00a:'845'
	// <0x56e62e>|0x24e|+0x034:'846'
	// <0x56e662>|0x282|+0x005:'846'
	// <0x56e667>|0x287|-0x01d:'847'
	// <0x56e64a>|0x26a|+0x013:'848'
	// <0x56e65d>|0x27d|-0x10d:'849'
	// <0>
	// <1>
	// <2>
	// <0x56e550>|0x170|+0x00a:'853'
	// <0x56e55a>|0x17a|+0x024:'854'
	// <0x56e57e>|0x19e|+0x031:'855'
	// <0x56e5af>|0x1cf|+0x004:'856'
	// <0>
	// <1>
	// <2>
	// <0x56e5b3>|0x1d3|+0x00f:'860'
	// <0x56e5c2>|0x1e2|+0x011:'861'
	// <0x56e5d3>|0x1f3|+0x001:'862'
	// <0>
	// <0x56e5d4>|0x1f4|+0x008:'864'
	// <0>
	// <1>
	// <0x56e5dc>|0x1fc|+0x1a2:'867'
	// <0>
	// <1>
	// <0x56e77e>|0x39e|+0x012:'870'
	// <0x56e790>|0x3b0|-0x07b:'871'
	// <0>
	// <1>
	// <2>
	// <0x56e715>|0x335|+0x02b:'875'
	// <0x56e740>|0x360|+0x034:'876'
	// <0x56e774>|0x394|+0x005:'876'
	// <0x56e779>|0x399|-0x01d:'877'
	// <0x56e75c>|0x37c|+0x013:'878'
	// <0x56e76f>|0x38f|-0x174:'879'
	// <0>
	// <1>
	// <2>
	// <0x56e5fb>|0x21b|+0x00a:'883'
	// <0x56e605>|0x225|+0x083:'884'
	// <0x56e688>|0x2a8|+0x00d:'884'
	// <0x56e695>|0x2b5|+0x031:'885'
	// <0x56e6c6>|0x2e6|+0x004:'886'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x56e6ca>|0x2ea|+0x03d:'896'
	// <0x56e707>|0x327|+0x093:'897'
	// <0>
	// <0x56e79a>|0x3ba|+0x00e:'899'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56e7a8>|0x3c8|+0x013:'905'
	// <0>
	// <0x56e7bb>|0x3db|+0x019:'907'
	// <0>
	// <1>
	// <2>
	// <0x56e7d4>|0x3f4|+0x006:'911'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56e7da>|0x3fa|+0x02d:'916'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56e807>|0x427|+0x00d:'921'
	// <0>
	// <0x56e814>|0x434|+0x007:'923'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56e81b>|0x43b|+0x014:'929'
	// <0>
	// <1>
	// <0x56e82f>|0x44f|+0x011:'932'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56e840>|0x460|-0x135:'938'
	// <0x56e70b>|0x32b|+0x093:'939'
	// <0x56e79e>|0x3be|+0x0b6:'939'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::merge_weight_synchronization_groups(
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

	// FUNCTION BODY[0x56ef70]: 51
	// <0>
	// <0x56ef73>|0x003|+0x12c:'951'
	// <0x56f09f>|0x12f|-0x118:'951'
	// <0x56ef87>|0x017|+0x005:'952'
	// <0>
	// <1>
	// <2>
	// <0x56ef8c>|0x01c|+0x01a:'956'
	// <0>
	// <0x56efa6>|0x036|+0x02c:'958'
	// <0x56efd2>|0x062|-0x022:'958'
	// <0x56efb0>|0x040|+0x011:'959'
	// <0x56efc1>|0x051|+0x00b:'960'
	// <0x56efcc>|0x05c|+0x002:'961'
	// <0>
	// <0x56efce>|0x05e|+0x00c:'963'
	// <0>
	// <1>
	// <2>
	// <0x56efda>|0x06a|+0x006:'967'
	// <0x56efe0>|0x070|+0x001:'968'
	// <0>
	// <0x56efe1>|0x071|+0x04d:'970'
	// <0>
	// <1>
	// <0x56f02e>|0x0be|+0x00b:'973'
	// <0>
	// <1>
	// <0x56f039>|0x0c9|+0x062:'976'
	// <0x56f09b>|0x12b|-0x055:'976'
	// <0x56f046>|0x0d6|+0x028:'977'
	// <0>
	// <0x56f06e>|0x0fe|+0x00e:'979'
	// <0x56f07c>|0x10c|-0x022:'980'
	// <0>
	// <1>
	// <0x56f05a>|0x0ea|+0x00f:'983'
	// <0>
	// <0x56f069>|0x0f9|+0x01b:'985'
	// <0>
	// <1>
	// <0x56f084>|0x114|+0x014:'988'
	// <0x56f098>|0x128|+0x009:'989'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x56f0a1>|0x131|+0x004:'996'
	// <0x56f0a5>|0x135|+0x019:'997'
	// <0>
	// <0x56f0be>|0x14e|+0x005:'999'
	// <0x56f0c3>|0x153|+0x015:'1000'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::merge_weight_asynchronous_groups(
	n_ary_tree_animation_node* const	from_begin,
	n_ary_tree_animation_node* const	from_end,
	n_ary_tree_animation_node* const	to_begin,
	n_ary_tree_animation_node* const	to_end
)
{
	// LOCALS
	// n_ary_tree_node_comparer 		comparer
	// ******

	// FUNCTION BODY[0x56eeb0]: 33
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56eeb4>|0x004|+0x01a:'1015'
	// <0x56eece>|0x01e|+0x05a:'1016'
	// <0x56ef28>|0x078|-0x052:'1016'
	// <0x56eed6>|0x026|+0x030:'1017'
	// <0>
	// <0x56ef06>|0x056|+0x00b:'1019'
	// <0x56ef11>|0x061|+0x003:'1020'
	// <0x56ef14>|0x064|-0x019:'1021'
	// <0>
	// <1>
	// <0x56eefb>|0x04b|+0x009:'1024'
	// <0>
	// <0x56ef04>|0x054|+0x012:'1026'
	// <0>
	// <1>
	// <0x56ef16>|0x066|+0x00c:'1029'
	// <0x56ef22>|0x072|+0x003:'1030'
	// <0x56ef25>|0x075|+0x024:'1031'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x56ef49>|0x099|+0x007:'1038'
	// <0x56ef50>|0x0a0|-0x024:'1039'
	// <0>
	// <0x56ef2c>|0x07c|+0x004:'1041'
	// <0x56ef30>|0x080|+0x010:'1042'
	// <0x56ef40>|0x090|+0x022:'1043'
	// ******
}

// STATE[STUB]
void n_ary_tree_comparer::change_weight_synchronization_group(
	n_ary_tree_animation_node*		from_begin,
	n_ary_tree_animation_node*		from_end,
	n_ary_tree_animation_node*		to_begin,
	n_ary_tree_animation_node*		to_end
)
{
	// FUNCTION BODY[0x56f1b0]: 19
	// <0x56f1b7>|0x007|+0x008:'1052'
	// <0x56f1bf>|0x00f|+0x016:'1053'
	// <0>
	// <1>
	// <2>
	// <0x56f1d5>|0x025|+0x009:'1057'
	// <0>
	// <0x56f1de>|0x02e|+0x01d:'1059'
	// <0>
	// <0x56f1fb>|0x04b|-0x00a:'1061'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x56f1f1>|0x041|+0x010:'1070'
	// <0x56f201>|0x051|-0x034:'1070'
	// <0x56f1cd>|0x01d|+0x055:'1071'
	// ******
}

// STATE[UNCHECKED]
void n_ary_tree_comparer::merge_trees( n_ary_tree const& from, n_ary_tree const& to )
{
	n_ary_tree_animation_node* i_begin		= from.weight_root();
	n_ary_tree_animation_node* i_end		= synchronization_group_end( i_begin );
	n_ary_tree_animation_node* j_begin		= to.weight_root();
	n_ary_tree_animation_node* j_end		= synchronization_group_end( j_begin );
	while ( i_begin && j_begin ) {
		if ( (*i_begin).weight_synchronization_group_id() < (*j_begin).weight_synchronization_group_id() ) {
			m_equal							= false;
			remove_weight_synchronization_group	( i_begin, i_end );
			get_next_synchronization_group		( i_begin, i_end );
			continue;
		}

		if ( (*i_begin).weight_synchronization_group_id() > (*j_begin).weight_synchronization_group_id() ) {
			m_equal							= false;
			add_weight_synchronization_group	( j_begin, j_end );
			get_next_synchronization_group		( j_begin, j_end );
			continue;
		}

		change_weight_synchronization_group	( i_begin, i_end, j_begin, j_end );
		get_next_synchronization_group		( i_begin, i_end );
		get_next_synchronization_group		( j_begin, j_end );
	}

	while ( i_begin ) {
		m_equal								= false;
		remove_weight_synchronization_group	( i_begin, i_end );
		get_next_synchronization_group		( i_begin, i_end );
	}

	while ( j_begin ) {
		m_equal								= false;
		add_weight_synchronization_group	( j_begin, j_end );
		get_next_synchronization_group		( j_begin, j_end );
	}


	// FUNCTION BODY[0x56f230]: 33
	// <0x56f233>|0x003|+0x003:'1078'
	// <0x56f236>|0x006|+0x019:'1079'
	// <0x56f24f>|0x01f|+0x007:'1080'
	// <0x56f256>|0x026|+0x01a:'1081'	synchronization_group_end( j_begin );
	// <0x56f270>|0x040|+0x048:'1082'	while
	// <0x56f2b8>|0x088|-0x040:'1082'
	// <0x56f278>|0x048|+0x00e:'1083'		if ( (*i_begin).weight_synchroni
	// <0x56f286>|0x056|+0x00b:'1084'
	// <0x56f291>|0x061|+0x01b:'1085'
	// <0x56f2ac>|0x07c|+0x067:'1086'
	// <0>
	// <1>
	// <0x56f313>|0x0e3|+0x002:'1089'
	// <0x56f315>|0x0e5|+0x030:'1090'
	// <0x56f345>|0x115|-0x091:'1091'
	// <0x56f2b4>|0x084|+0x0ae:'1092'
	// <0x56f362>|0x132|+0x005:'1092'
	// <0>
	// <1>
	// <0x56f367>|0x137|+0x00f:'1095'
	// <0x56f376>|0x146|+0x01f:'1096'
	// <0x56f395>|0x165|+0x022:'1097'
	// <0>
	// <1>
	// <0x56f3b7>|0x187|+0x009:'1100'
	// <0x56f3c0>|0x190|+0x004:'1101'
	// <0x56f3c4>|0x194|+0x00b:'1102'
	// <0x56f3cf>|0x19f|-0x113:'1103'
	// <0>
	// <1>
	// <0x56f2bc>|0x08c|+0x055:'1106'
	// <0x56f311>|0x0e1|-0x04d:'1106'
	// <0>
	// <0x56f2c4>|0x094|+0x030:'1108'
	// <0x56f2f4>|0x0c4|+0x104:'1109'
	// <0>
	// ******
}

// STATE[UNCHECKED]
 n_ary_tree_comparer::n_ary_tree_comparer( n_ary_tree const& from, n_ary_tree const& to, u32 current_time_in_ms ) :
	m_animations_count		( 0 ),
	m_animated_objects_count( 0 ),
	m_current_time_in_ms	( current_time_in_ms ),
	m_from					( from ),
	m_to					( to ),
	m_equal					( true ),
	m_needed_buffer_size	( 4 ) // sushi@TODO
{

	process_interpolators( from, to );

	m_animated_objects = ( animated_object_holder* )ALLOCA( sizeof( animated_object_holder ) * ( to.animated_objects_count() + from.animated_objects_count() ) );
	m_animated_objects_end = m_animated_objects;
	merge_trees( from, to );
	m_animated_objects_count = m_animated_objects_end - m_animated_objects;

	m_needed_buffer_size += m_animations_count * sizeof( animation_state );

	// FUNCTION BODY[0x56f410]: 14
	// <0>
	// <1>
	// <0x56f426>|0x016|+0x01b:'1134'
	// <0>
	// <1>
	// <0x56f441>|0x031|+0x013:'1137'
	// <0>
	// <1>
	// <0x56f454>|0x044|+0x00e:'1140'
	// <0>
	// <1>
	// <2>
	// <0x56f462>|0x052|+0x022:'1144'
	// <0x56f484>|0x074|+0x00b:'1145'
	// ******
}

} // namespace mixing
} // namespace animation
} // namespace vostok