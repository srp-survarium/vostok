////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_parameters_cook.h"

namespace survarium {

// STATE[STUB]
// void survarium::player_parameters_modifyer::apply(survarium::base_player*)
void player_parameters_modifyer::apply( base_player* player )
{
	// LOCALS
	// inventory& 					invent
	// player_stamina& 				stamn
	// std::priv::_Rb_tree_iterator<std::pair<vostok::fixed_string<16> const ,body_part_parameters_modifyer>,std::priv::_ConstMapTraitsT<std::pair<vostok::fixed_string<16> const ,body_part_parameters_modifyer> > > body_part_it_e
	// std::priv::_Rb_tree_iterator<std::pair<vostok::fixed_string<16> const ,body_part_parameters_modifyer>,std::priv::_ConstMapTraitsT<std::pair<vostok::fixed_string<16> const ,body_part_parameters_modifyer> > > body_part_it
	// body_part_parameters* 		bp
	// bodypart_health_regen_scale_predicate hr_predicate
	// vostok::resources::resource_ptr<damage_model,vostok::resources::unmanaged_intrusive_base> damage_model
	// std::priv::_Rb_tree_iterator<std::pair<vostok::fixed_string<16> const ,hit_type_parameters_modifyer>,std::priv::_ConstMapTraitsT<std::pair<vostok::fixed_string<16> const ,hit_type_parameters_modifyer> > > hit_type_it<1>
	// std::priv::_Rb_tree_iterator<std::pair<vostok::fixed_string<16> const ,hit_type_parameters_modifyer>,std::priv::_ConstMapTraitsT<std::pair<vostok::fixed_string<16> const ,hit_type_parameters_modifyer> > > hit_type_it_e<1>
	// vostok::fixed_string<16> 	body_part_name<1>
	// body_part_parameters* 		current_body_part_parameters<1>
	// body_part_parameters_modifyer& current_body_part_modifyer<1>
	// vostok::fixed_string<16> 	hit_type_name<2>
	// hit_type_parameters* 		current_hit_type_parameters<2>
	// hit_type_parameters_modifyer& current_hit_type_modifyer<2>
	// u32 							i<1>
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> item<2>
	// weapon_core* 				wc<2>
	// dispersion_calculator& 		dc<2>
	// float 						health<2>
	// float 						regen<2>
	// float 						anomaly_scale<2>
	// u32 							i<3>
	// ******

	// STATICS
	// static pcstr[4] 				anomaly_damage_types = <0xa8ade8>;
	// ******

	// SKIPPED BLOCKS
	// <0x5abcc2><1>
	// <0x5abdcd><2>
	// <0x5abe5e><2>
	// <0x5ac09b><2>
	// <0x5ac25c><2>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59ac35 }, type_index: TypeIndex(0x916d) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59ae3e }, type_index: TypeIndex(0x3dfa0) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59aeb8 }, type_index: TypeIndex(0x6b7d) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59b011 }, type_index: TypeIndex(0x9176) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59b2d9 }, type_index: TypeIndex(0x916d) })
	// ******

	// FUNCTION BODY
	// <0x5abc2a>|0x000|0x000:'40'
	// 1
	// <0x5abc4b>|0x021|0x021:'42'
	// <0x5abc7c>|0x052|0x031:'43'
	// 1
	// <0x5abca1>|0x077|0x025:'45'
	// 1
	// <0x5abcc8>|0x09e|0x027:'47'
	// 1
	// <0x5abcd6>|0x0ac|0x00e:'49'
	// 1
	// <0x5abcf1>|0x0c7|0x01b:'51'
	// 1
	// 2
	// <0x5abd09>|0x0df|0x018:'54'
	// 1
	// <0x5abd62>|0x138|0x059:'56'
	// <0x5abd8d>|0x163|0x02b:'57'
	// 1
	// <0x5abdac>|0x182|0x01f:'59'
	// 1
	// <0x5abdcf>|0x1a5|0x023:'61'
	// 1
	// <0x5abddd>|0x1b3|0x00e:'63'
	// 1
	// <0x5abdf1>|0x1c7|0x014:'65'
	// 1
	// <0x5abe03>|0x1d9|0x012:'67'
	// <0x5abe28>|0x1fe|0x025:'68'
	// <0x5abe2a>|0x200|0x002:'69'
	// 1
	// 2
	// <0x5abe2f>|0x205|0x005:'72'
	// 1
	// <0x5abe48>|0x21e|0x019|[1]:'74'
	// 1
	// <0x5abe64>|0x23a|0x01c:'76'
	// 1
	// <0x5abe80>|0x256|0x01c:'78'
	// <0x5abe8f>|0x265|0x00f:'79'
	// 1
	// <0x5abe99>|0x26f|0x00a:'81'
	// 1
	// <0x5abebd>|0x293|0x024:'83'
	// 1
	// <0x5abec7>|0x29d|0x00a:'85'
	// <0x5abf50>|0x326|0x089:'86'
	// 1
	// 2
	// <0x5abf5d>|0x333|0x00d:'89'
	// 1
	// <0x5abf68>|0x33e|0x00b:'91'
	// <0x5abf94>|0x36a|0x02c:'92'
	// <0x5abfc0>|0x396|0x02c:'93'
	// 1
	// 2
	// <0x5abfcd>|0x3a3|0x00d:'96'
	// 1
	// 2
	// <0x5ac006>|0x3dc|0x039:'99'
	// 1
	// 2
	// <0x5ac016>|0x3ec|0x010:'102'
	// 1
	// 2
	// <0x5ac054>|0x42a|0x03e:'105'
	// 1
	// 2
	// <0x5ac080>|0x456|0x02c:'108'
	// <0x5ac097>|0x46d|0x017:'109'
	// 1
	// <0x5ac09d>|0x473|0x006:'111'
	// <0x5ac0de>|0x4b4|0x041:'112'
	// 1
	// <0x5ac0ee>|0x4c4|0x010:'114'
	// 1
	// <0x5ac107>|0x4dd|0x019:'116'
	// <0x5ac10c>|0x4e2|0x005:'117'
	// 1
	// 2
	// <0x5ac195>|0x56b|0x089:'120'
	// <0x5ac1c8>|0x59e|0x033:'121'
	// 1
	// 2
	// <0x5ac20d>|0x5e3|0x045:'124'
	// 1
	// 2
	// <0x5ac243>|0x619|0x036:'127'
	// 1
	// <0x5ac262>|0x638|0x01f:'129'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x5ac288>|0x65e|0x026|[3]:'138'
	// 1
	// <0x5ac2b0>|0x686|0x028:'140'
	// <0x5ac2e7>|0x6bd|0x037:'141'
	// 1
	// 2
	// 3
	// <0x5ac2e9>|0x6bf|0x002:'145'
	// 1
	// 2
	// 3
	// ******
}

// STATE[STUB]
// float survarium::get_booster_value(survarium::boosters_enum, survarium::player_profile const&)
float get_booster_value( boosters_enum booster_id, player_profile const& profile )
{
	// LOCALS
	// u8 							i<1>
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x5ab294>|0x000|0x000|[1]:'153'
	// 1
	// <0x5ab2ab>|0x017|0x017:'155'
	// <0x5ab2bc>|0x028|0x011:'156'
	// <0x5ab2c9>|0x035|0x00d:'157'
	// <0x5ab2cb>|0x037|0x002:'158'
	// ******
}

// STATE[STUB]
// survarium::player_parameters_modifyer_cook::player_parameters_modifyer_cook()
player_parameters_modifyer_cook::player_parameters_modifyer_cook( )
{
	// FUNCTION BODY
	// <0x5ab315>|0x000|0x000:'165'
	// ******
}

// STATE[STUB]
// void survarium::player_parameters_modifyer_cook::translate_query(vostok::resources::query_result_for_cook&)
void player_parameters_modifyer_cook::translate_query( vostok::resources::query_result_for_cook& parent )
{
	// LOCALS
	// player_parameters_cooker_data* cooker_data
	// profile_slot const* 			slot
	// player_parameters_modifyer* 	cooked_resource
	// u32 							i<1>
	// profile_slot_enum 			current_slot<2>
	// dictionary_item 				curr_item<3>
	// vostok::configs::binary_config_value const& current_item_config<3>
	// u32 							count<4>
	// vostok::configs::binary_config_value const* body_it_end<4>
	// vostok::configs::binary_config_value const* body_it<4>
	// vostok::fixed_string<16> 	body_part_name<5>
	// body_part_parameters_modifyer body_part_modifyer_from_cfg<5>
	// vostok::configs::binary_config_value const& current_body_part_cfg<5>
	// body_part_parameters_modifyer& current_body_part_modifyer<5>
	// vostok::configs::binary_config_value const* hit_type_it<6>
	// vostok::configs::binary_config_value const* hit_type_it_end<6>
	// vostok::fixed_string<16> 	hit_type_name<7>
	// hit_type_parameters_modifyer hit_type_modifyer_from_cfg<7>
	// vostok::configs::binary_config_value const& current_hit_type_cfg<7>
	// ******

	// SKIPPED BLOCKS
	// <0x5ab52d><2>
	// <0x5ab555><3>
	// <0x5ab59f><4>
	// <0x5ab6a1><4>
	// <0x5ab6f8><5>
	// <0x5ab95b><6>
	// <0x5ab9b8><7>
	// ******

	// FUNCTION BODY
	// <0x5ab37b>|0x000|0x000:'170'
	// <0x5ab382>|0x007|0x007:'171'
	// 1
	// <0x5ab395>|0x01a|0x013:'173'
	// 1
	// <0x5ab404>|0x089|0x06f:'175'
	// <0x5ab41d>|0x0a2|0x019:'176'
	// <0x5ab436>|0x0bb|0x019:'177'
	// <0x5ab44f>|0x0d4|0x019:'178'
	// <0x5ab468>|0x0ed|0x019:'179'
	// <0x5ab481>|0x106|0x019:'180'
	// <0x5ab49a>|0x11f|0x019:'181'
	// <0x5ab4b3>|0x138|0x019:'182'
	// <0x5ab4cc>|0x151|0x019:'183'
	// <0x5ab4e5>|0x16a|0x019:'184'
	// <0x5ab4fe>|0x183|0x019:'185'
	// 1
	// 2
	// 3
	// <0x5ab517>|0x19c|0x019|[1]:'189'
	// 1
	// <0x5ab533>|0x1b8|0x01c:'191'
	// <0x5ab539>|0x1be|0x006:'192'
	// <0x5ab54e>|0x1d3|0x015:'193'
	// 1
	// <0x5ab55b>|0x1e0|0x00d:'195'
	// <0x5ab57a>|0x1ff|0x01f:'196'
	// 1
	// <0x5ab58d>|0x212|0x013:'198'
	// 1
	// 2
	// <0x5ab5a1>|0x226|0x014:'201'
	// 1
	// <0x5ab5cc>|0x251|0x02b:'203'
	// 1
	// 2
	// <0x5ab5fd>|0x282|0x031:'206'
	// 1
	// <0x5ab611>|0x296|0x014:'208'
	// <0x5ab647>|0x2cc|0x036:'209'
	// 1
	// <0x5ab67d>|0x302|0x036:'211'
	// 1
	// <0x5ab68f>|0x314|0x012:'213'
	// 1
	// <0x5ab6a7>|0x32c|0x018:'215'
	// <0x5ab6c1>|0x346|0x01a:'216'
	// 1
	// <0x5ab6db>|0x360|0x01a:'218'
	// 1
	// <0x5ab6fe>|0x383|0x023:'220'
	// <0x5ab715>|0x39a|0x017:'221'
	// 1
	// <0x5ab721>|0x3a6|0x00c:'223'
	// 1
	// <0x5ab738>|0x3bd|0x017:'225'
	// <0x5ab78e>|0x413|0x056:'226'
	// 1
	// <0x5ab7e4>|0x469|0x056:'228'
	// 1
	// <0x5ab863>|0x4e8|0x07f:'230'
	// <0x5ab89a>|0x51f|0x037:'231'
	// 1
	// <0x5ab8d4>|0x559|0x03a:'233'
	// <0x5ab8d6>|0x55b|0x002:'234'
	// 1
	// <0x5ab92b>|0x5b0|0x055:'236'
	// 1
	// <0x5ab946>|0x5cb|0x01b:'238'
	// 1
	// <0x5ab961>|0x5e6|0x01b:'240'
	// <0x5ab97e>|0x603|0x01d:'241'
	// 1
	// <0x5ab99b>|0x620|0x01d:'243'
	// 1
	// <0x5ab9be>|0x643|0x023:'245'
	// 1
	// 2
	// 3
	// <0x5ab9ca>|0x64f|0x00c:'249'
	// 1
	// <0x5ab9e1>|0x666|0x017:'251'
	// <0x5ab9fe>|0x683|0x01d:'252'
	// <0x5aba1b>|0x6a0|0x01d:'253'
	// 1
	// <0x5aba38>|0x6bd|0x01d:'255'
	// 1
	// <0x5ababb>|0x740|0x083:'257'
	// <0x5abaf2>|0x777|0x037:'258'
	// <0x5abb2c>|0x7b1|0x03a:'259'
	// 1
	// <0x5abb66>|0x7eb|0x03a:'261'
	// <0x5abb68>|0x7ed|0x002:'262'
	// 1
	// 2
	// <0x5abb9c>|0x821|0x034:'265'
	// 1
	// <0x5abbac>|0x831|0x010:'267'
	// <0x5abbb7>|0x83c|0x00b:'268'
	// 1
	// 2
	// 3
	// 4
	// <0x5abbbc>|0x841|0x005:'273'
	// <0x5abbf2>|0x877|0x036:'274'
	// ******
}

// STATE[STUB]
// void survarium::player_parameters_modifyer_cook::delete_resource(vostok::resources::resource_base*)
void player_parameters_modifyer_cook::delete_resource( vostok::resources::resource_base* resource )
{
	// FUNCTION BODY
	// <0x5ab339>|0x000|0x000:'279'
	// ******
}

} // namespace survarium
