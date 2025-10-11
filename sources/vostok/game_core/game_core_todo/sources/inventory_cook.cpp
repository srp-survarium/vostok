////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "inventory_cook.h"

namespace survarium {

// STATE[STUB]
// survarium::inventory_cook::inventory_cook()
inventory_cook::inventory_cook( )
{
	// STATICS
	// static damage_model_cook 	s_damage_model_cook = <0x4c27230>;
	// static weapon_ammunition_cook s_weapon_ammunition_cook = <0x4c27210>;
	// static items_cook 			s_items_cook = <0x4c27250>;
	// ******

	// FUNCTION BODY
	// <0x764e45>|0x000|0x000:'29'
	// <0x764e51>|0x00c|0x00c:'30'
	// <0x764e80>|0x03b|0x02f:'31'
	// <0x764eb1>|0x06c|0x031:'32'
	// ******
}

// STATE[STUB]
// void survarium::inventory_cook::translate_query(vostok::resources::query_result_for_cook&)
void inventory_cook::translate_query( vostok::resources::query_result_for_cook& parent )
{
	// LOCALS
	// inventory_cooker_data* 		cooker_data
	// profile_slot* 				slot
	// vostok::buffer_vector<vostok::variant<32> const *> user_data
	// pcstr 						resource_name
	// vostok::buffer_vector<vostok::resources::request> requests
	// vostok::variant<32>[19] 		v
	// u32 							i
	// profile_slot_enum 			current
	// u32 							i
	// profile_slot_enum 			current
	// u32 							i
	// profile_slot_enum 			current
	// item_types_enum 				item_type
	// dictionary_item const& 		desc
	// vostok::resources::class_id_enum class_id
	// booby_trap_set_cook_data 	booby_trap_set_data
	// ******

	// FUNCTION BODY
	// 1
	// <0x765350>|0x000|0x000:'38'
	// <0x765363>|0x013|0x013:'39'
	// <0x76536f>|0x01f|0x00c:'40'
	// 1
	// <0x765390>|0x040|0x021:'42'
	// <0x7653b1>|0x061|0x021:'43'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x7653c6>|0x076|0x015|[1]:'49'
	// 1
	// <0x7653ee>|0x09e|0x028|[2]:'51'
	// <0x765401>|0x0b1|0x013:'52'
	// <0x76541c>|0x0cc|0x01b:'53'
	// 1
	// <0x765428>|0x0d8|0x00c:'55'
	// <0x76544b>|0x0fb|0x023:'56'
	// <0x765475>|0x125|0x02a:'57'
	// 1
	// <0x76548d>|0x13d|0x018:'59'
	// 1
	// <0x765492>|0x142|0x005|[2]:'61'
	// 1
	// <0x7654ba>|0x16a|0x028|[3]:'63'
	// <0x7654cd>|0x17d|0x013:'64'
	// <0x7654e8>|0x198|0x01b:'65'
	// 1
	// <0x7654f4>|0x1a4|0x00c:'67'
	// <0x765517>|0x1c7|0x023:'68'
	// <0x765541>|0x1f1|0x02a:'69'
	// 1
	// <0x765559>|0x209|0x018:'71'
	// 1
	// <0x76555e>|0x20e|0x005|[3]:'73'
	// 1
	// <0x765586>|0x236|0x028|[4]:'75'
	// <0x765599>|0x249|0x013:'76'
	// <0x7655b4>|0x264|0x01b:'77'
	// 1
	// <0x7655c4>|0x274|0x010|[5]:'79'
	// <0x7655df>|0x28f|0x01b:'80'
	// 1
	// <0x765615>|0x2c5|0x036:'82'
	// 1
	// 2
	// <0x765638>|0x2e8|0x023:'85'
	// 1
	// 2
	// 3
	// <0x76566c>|0x31c|0x034|[6]:'89'
	// 1
	// <0x765676>|0x326|0x00a:'91'
	// 1
	// 2
	// <0x765688>|0x338|0x012:'94'
	// <0x765699>|0x349|0x011:'95'
	// <0x7656a7>|0x357|0x00e:'96'
	// <0x7656c3>|0x373|0x01c:'97'
	// <0x7656e7>|0x397|0x024:'98'
	// 1
	// 2
	// 3
	// <0x7656e9>|0x399|0x002:'102'
	// <0x7656f3>|0x3a3|0x00a:'103'
	// 1
	// 2
	// 3
	// <0x7656f5>|0x3a5|0x002:'107'
	// <0x7656ff>|0x3af|0x00a:'108'
	// 1
	// 2
	// 3
	// <0x765701>|0x3b1|0x002:'112'
	// 1
	// 2
	// 3
	// <0x76570b>|0x3bb|0x00a:'116'
	// 1
	// <0x765736>|0x3e6|0x02b:'118'
	// <0x76573f>|0x3ef|0x009:'119'
	// 1
	// <0x765757>|0x407|0x018:'121'
	// 1
	// <0x76575c>|0x40c|0x005:'123'
	// 1
	// <0x76576e>|0x41e|0x012:'125'
	// <0x765794>|0x444|0x026:'126'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x7657ac>|0x45c|0x018:'135'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::inventory_cook::on_subresources_loaded(vostok::resources::queries_result&, survarium::inventory_cooker_data*)
void inventory_cook::on_subresources_loaded( vostok::resources::queries_result& data, inventory_cooker_data* cooker_data )
{
	// LOCALS
	// inventory* 					result
	// profile_slot* 				slot
	// vostok::resources::query_result_for_cook* parent
	// u32 							result_index
	// u32 							i<1>
	// profile_slot_enum 			current<2>
	// vostok::resources::resource_ptr<weapon_core,vostok::resources::unmanaged_intrusive_base> wpn<3>
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> iitem<3>
	// u32 							i<2>
	// profile_slot_enum 			current<3>
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> iitem<4>
	// u32 							i<3>
	// profile_slot_enum 			current<4>
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> iitem<5>
	// ******

	// FUNCTION BODY
	// <0x764f40>|0x000|0x000:'141'
	// <0x764f4c>|0x00c|0x00c:'142'
	// <0x764fa6>|0x066|0x05a:'143'
	// 1
	// 2
	// 3
	// <0x764fad>|0x06d|0x007|[1]:'147'
	// 1
	// <0x764fc9>|0x089|0x01c|[2]:'149'
	// <0x764fd6>|0x096|0x00d:'150'
	// 1
	// <0x764feb>|0x0ab|0x015:'152'
	// 1
	// <0x764ff8>|0x0b8|0x00d|[3]:'154'
	// <0x765004>|0x0c4|0x00c:'155'
	// <0x76503a>|0x0fa|0x036:'156'
	// <0x765043>|0x103|0x009:'157'
	// <0x76504f>|0x10f|0x00c:'158'
	// <0x76507a>|0x13a|0x02b:'159'
	// <0x765093>|0x153|0x019:'160'
	// <0x7650a6>|0x166|0x013:'161'
	// <0x7650b6>|0x176|0x010:'162'
	// <0x7650c6>|0x186|0x010:'163'
	// 1
	// 2
	// <0x7650cb>|0x18b|0x005|[2]:'166'
	// 1
	// <0x7650e7>|0x1a7|0x01c|[3]:'168'
	// <0x7650f4>|0x1b4|0x00d:'169'
	// 1
	// <0x765109>|0x1c9|0x015:'171'
	// 1
	// <0x765116>|0x1d6|0x00d|[4]:'173'
	// <0x765122>|0x1e2|0x00c:'174'
	// <0x765177>|0x237|0x055:'175'
	// <0x765180>|0x240|0x009:'176'
	// <0x7651b0>|0x270|0x030:'177'
	// <0x7651c0>|0x280|0x010:'178'
	// <0x7651c8>|0x288|0x008:'179'
	// 1
	// 2
	// <0x7651cd>|0x28d|0x005|[3]:'182'
	// 1
	// <0x7651e9>|0x2a9|0x01c|[4]:'184'
	// <0x7651f6>|0x2b6|0x00d:'185'
	// 1
	// <0x76520b>|0x2cb|0x015:'187'
	// 1
	// <0x765218>|0x2d8|0x00d:'189'
	// 1
	// <0x765231>|0x2f1|0x019|[5]:'191'
	// <0x765286>|0x346|0x055:'192'
	// <0x7652b6>|0x376|0x030:'193'
	// <0x7652c6>|0x386|0x010:'194'
	// <0x7652ce>|0x38e|0x008:'195'
	// 1
	// <0x7652d7>|0x397|0x009:'197'
	// 1
	// <0x7652dc>|0x39c|0x005:'199'
	// <0x7652e7>|0x3a7|0x00b:'200'
	// <0x76531a>|0x3da|0x033:'201'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::inventory_cook::delete_resource(vostok::resources::resource_base*)
void inventory_cook::delete_resource( vostok::resources::resource_base* resource )
{
	// LOCALS
	// inventory* 					inventory
	// ******

	// FUNCTION BODY
	// <0x764ef9>|0x000|0x000:'207'
	// <0x764eff>|0x006|0x006:'208'
	// ******
}

} // namespace survarium
