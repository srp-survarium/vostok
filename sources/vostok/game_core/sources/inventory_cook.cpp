////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/inventory_cook.h>

namespace survarium {

// STATE[STUB]
inventory_cook::inventory_cook( ) : resources::translate_query_cook( resources::inventory_class, reuse_false, use_any_thread_id )
{
	// STATICS
	// static damage_model_cook 	s_damage_model_cook = <0x4c27230>;
	// static weapon_ammunition_cook s_weapon_ammunition_cook = <0x4c27210>;
	// static items_cook 			s_items_cook = <0x4c27250>;
	// ******

	// FUNCTION BODY
	// <0x764e45>|0x035|+0x00c:'29'
	// <0x764e51>|0x041|+0x02f:'30'
	// <0x764e80>|0x070|+0x031:'31'
	// <0x764eb1>|0x0a1|+0x030:'32'
	// ******
}

// STATE[STUB]
// void survarium::inventory_cook::translate_query(vostok::resources::query_result_for_cook&)
void inventory_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// inventory_cooker_data* 		cooker_data
	// profile_slot* 				slot
	// buffer_vector<variant<32> const *> user_data
	// pcstr 						resource_name
	// buffer_vector<resources::request> requests
	// variant<32>[19] 				v
	// u32 							i
	// profile_slot_enum 			current
	// u32 							i
	// profile_slot_enum 			current
	// u32 							i
	// profile_slot_enum 			current
	// item_types_enum 				item_type
	// dictionary_item const& 		desc
	// resources::class_id_enum 	class_id
	// booby_trap_set_cook_data 	booby_trap_set_data
	// ******

	// FUNCTION BODY
	// <0>
	// <0x765350>|0x020|+0x013:'38'
	// <0x765363>|0x033|+0x00c:'39'
	// <0x76536f>|0x03f|+0x021:'40'
	// <0>
	// <0x765390>|0x060|+0x021:'42'
	// <0x7653b1>|0x081|+0x015:'43'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7653c6>|0x096|+0x028|[1]:'49'
	// <0>
	// <0x7653ee>|0x0be|+0x013|[2]:'51'
	// <0x765401>|0x0d1|+0x01b:'52'
	// <0x76541c>|0x0ec|+0x00c:'53'
	// <0>
	// <0x765428>|0x0f8|+0x023:'55'
	// <0x76544b>|0x11b|+0x02a:'56'
	// <0x765475>|0x145|+0x018:'57'
	// <0>
	// <0x76548d>|0x15d|+0x005:'59'
	// <0>
	// <0x765492>|0x162|+0x028|[2]:'61'
	// <0>
	// <0x7654ba>|0x18a|+0x013|[3]:'63'
	// <0x7654cd>|0x19d|+0x01b:'64'
	// <0x7654e8>|0x1b8|+0x00c:'65'
	// <0>
	// <0x7654f4>|0x1c4|+0x023:'67'
	// <0x765517>|0x1e7|+0x02a:'68'
	// <0x765541>|0x211|+0x018:'69'
	// <0>
	// <0x765559>|0x229|+0x005:'71'
	// <0>
	// <0x76555e>|0x22e|+0x028|[3]:'73'
	// <0>
	// <0x765586>|0x256|+0x013|[4]:'75'
	// <0x765599>|0x269|+0x01b:'76'
	// <0x7655b4>|0x284|+0x010:'77'
	// <0>
	// <0x7655c4>|0x294|+0x01b|[5]:'79'
	// <0x7655df>|0x2af|+0x036:'80'
	// <0>
	// <0x765615>|0x2e5|+0x023:'82'
	// <0>
	// <1>
	// <0x765638>|0x308|+0x034:'85'
	// <0>
	// <1>
	// <2>
	// <0x76566c>|0x33c|+0x00a|[6]:'89'
	// <0>
	// <0x765676>|0x346|+0x012:'91'
	// <0>
	// <1>
	// <0x765688>|0x358|+0x011:'94'
	// <0x765699>|0x369|+0x00e:'95'
	// <0x7656a7>|0x377|+0x01c:'96'
	// <0x7656c3>|0x393|+0x024:'97'
	// <0x7656e7>|0x3b7|+0x002:'98'
	// <0>
	// <1>
	// <2>
	// <0x7656e9>|0x3b9|+0x00a:'102'
	// <0x7656f3>|0x3c3|+0x002:'103'
	// <0>
	// <1>
	// <2>
	// <0x7656f5>|0x3c5|+0x00a:'107'
	// <0x7656ff>|0x3cf|+0x002:'108'
	// <0>
	// <1>
	// <2>
	// <0x765701>|0x3d1|+0x00a:'112'
	// <0>
	// <1>
	// <2>
	// <0x76570b>|0x3db|+0x02b:'116'
	// <0>
	// <0x765736>|0x406|+0x009:'118'
	// <0x76573f>|0x40f|+0x018:'119'
	// <0>
	// <0x765757>|0x427|+0x005:'121'
	// <0>
	// <0x76575c>|0x42c|+0x012:'123'
	// <0>
	// <0x76576e>|0x43e|+0x026:'125'
	// <0x765794>|0x464|+0x018:'126'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x7657ac>|0x47c|+0x0db:'135'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::inventory_cook::on_subresources_loaded(vostok::resources::queries_result&, survarium::inventory_cooker_data*)
void inventory_cook::on_subresources_loaded( resources::queries_result& data, inventory_cooker_data* cooker_data )
{
	// LOCALS
	// inventory* 					result
	// profile_slot* 				slot
	// resources::query_result_for_cook* parent
	// u32 							result_index
	// u32 							i<1>
	// profile_slot_enum 			current<2>
	// resources::resource_ptr<weapon_core,resources::unmanaged_intrusive_base> wpn<3>
	// resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> iitem<3>
	// u32 							i<2>
	// profile_slot_enum 			current<3>
	// resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> iitem<4>
	// u32 							i<3>
	// profile_slot_enum 			current<4>
	// resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> iitem<5>
	// ******

	// FUNCTION BODY
	// <0x764f40>|0x010|+0x00c:'141'
	// <0x764f4c>|0x01c|+0x05a:'142'
	// <0x764fa6>|0x076|+0x007:'143'
	// <0>
	// <1>
	// <2>
	// <0x764fad>|0x07d|+0x01c|[1]:'147'
	// <0>
	// <0x764fc9>|0x099|+0x00d|[2]:'149'
	// <0x764fd6>|0x0a6|+0x015:'150'
	// <0>
	// <0x764feb>|0x0bb|+0x00d:'152'
	// <0>
	// <0x764ff8>|0x0c8|+0x00c|[3]:'154'
	// <0x765004>|0x0d4|+0x036:'155'
	// <0x76503a>|0x10a|+0x009:'156'
	// <0x765043>|0x113|+0x00c:'157'
	// <0x76504f>|0x11f|+0x02b:'158'
	// <0x76507a>|0x14a|+0x019:'159'
	// <0x765093>|0x163|+0x013:'160'
	// <0x7650a6>|0x176|+0x010:'161'
	// <0x7650b6>|0x186|+0x010:'162'
	// <0x7650c6>|0x196|+0x005:'163'
	// <0>
	// <1>
	// <0x7650cb>|0x19b|+0x01c|[2]:'166'
	// <0>
	// <0x7650e7>|0x1b7|+0x00d|[3]:'168'
	// <0x7650f4>|0x1c4|+0x015:'169'
	// <0>
	// <0x765109>|0x1d9|+0x00d:'171'
	// <0>
	// <0x765116>|0x1e6|+0x00c|[4]:'173'
	// <0x765122>|0x1f2|+0x055:'174'
	// <0x765177>|0x247|+0x009:'175'
	// <0x765180>|0x250|+0x030:'176'
	// <0x7651b0>|0x280|+0x010:'177'
	// <0x7651c0>|0x290|+0x008:'178'
	// <0x7651c8>|0x298|+0x005:'179'
	// <0>
	// <1>
	// <0x7651cd>|0x29d|+0x01c|[3]:'182'
	// <0>
	// <0x7651e9>|0x2b9|+0x00d|[4]:'184'
	// <0x7651f6>|0x2c6|+0x015:'185'
	// <0>
	// <0x76520b>|0x2db|+0x00d:'187'
	// <0>
	// <0x765218>|0x2e8|+0x019:'189'
	// <0>
	// <0x765231>|0x301|+0x055|[5]:'191'
	// <0x765286>|0x356|+0x030:'192'
	// <0x7652b6>|0x386|+0x010:'193'
	// <0x7652c6>|0x396|+0x008:'194'
	// <0x7652ce>|0x39e|+0x009:'195'
	// <0>
	// <0x7652d7>|0x3a7|+0x005:'197'
	// <0>
	// <0x7652dc>|0x3ac|+0x00b:'199'
	// <0x7652e7>|0x3b7|+0x033:'200'
	// <0x76531a>|0x3ea|+0x00c:'201'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::inventory_cook::delete_resource(vostok::resources::resource_base*)
void inventory_cook::delete_resource( resources::resource_base* resource )
{
	// LOCALS
	// inventory* 					inventory
	// ******

	// FUNCTION BODY
	// <0x764ef9>|0x009|+0x006:'207'
	// <0x764eff>|0x00f|+0x026:'208'
	// ******
}

} // namespace survarium
