////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "damage_model.h"

namespace survarium {

// STATE[STUB]
// survarium::damage_model::damage_model(const survarium::affects_applying_type_enum)
damage_model::damage_model( affects_applying_type_enum affects_applying_type )
{
	// FUNCTION BODY
	// <0x6ffff4>|0x000|0x000:'22'
	// <0x700001>|0x00d|0x00d:'23'
	// 1
	// <0x70000e>|0x01a|0x00d:'25'
	// <0x70001b>|0x027|0x00d:'26'
	// 1
	// <0x700028>|0x034|0x00d:'28'
	// <0x7000e6>|0x0f2|0x0be:'29'
	// 1
	// <0x7001ad>|0x1b9|0x0c7:'31'
	// <0x7001c7>|0x1d3|0x01a:'32'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::~damage_model()
void damage_model::~damage_model( )
{
	// LOCALS
	// booster_damage_protector* 	p<1>
	// ******

	// FUNCTION BODY
	// <0x6ffde3>|0x000|0x000|[1]:'37'
	// <0x6ffdfa>|0x017|0x017:'38'
	// 1
	// <0x6ffe22>|0x03f|0x028:'40'
	// <0x6ffe36>|0x053|0x014:'41'
	// ******
}

// STATE[STUB]
// survarium::body_part_parameters* survarium::damage_model::pop_body_part()
body_part_parameters* damage_model::pop_body_part( )
{
	return NULL;
	// FUNCTION BODY
	// <0x6ff629>|0x000|0x000:'46'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::tick(const unsigned int, const unsigned int)
void damage_model::tick( u32 time_delta_ms, u32 current_time_in_ms )
{
	// LOCALS
	// regenerate_body_parts_predicate regeneration_predicate
	// ******

	// FUNCTION BODY
	// <0x6ffc69>|0x000|0x000:'68'
	// 1
	// <0x6ffc75>|0x00c|0x00c:'70'
	// <0x6ffc81>|0x018|0x00c:'71'
	// 1
	// 2
	// <0x6ffc83>|0x01a|0x002:'74'
	// <0x6ffc97>|0x02e|0x014:'75'
	// <0x6ffcbf>|0x056|0x028:'76'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::add_body_part(survarium::body_part_parameters* const)
void damage_model::add_body_part( body_part_parameters* new_body_part )
{
	// FUNCTION BODY
	// <0x6ff5f9>|0x000|0x000:'81'
	// ******
}

// STATE[STUB]
// survarium::body_part_parameters* survarium::damage_model::get_body_part(char const*) const
body_part_parameters* damage_model::get_body_part( pcstr part_name ) const
{
	// LOCALS
	// find_body_part_by_name_predicate find_predicate
	// ******

	return NULL;
	// FUNCTION BODY
	// <0x6ff739>|0x000|0x000:'101'
	// <0x6ff747>|0x00e|0x00e:'102'
	// ******
}

// STATE[STUB]
// bool survarium::damage_model::hit_body_part(const unsigned char, char const*, char const*, const float, const float, const unsigned int, survarium::bullet* const)
bool damage_model::hit_body_part(
	u8			initiator,
	pcstr		part_name,
	pcstr		damage_type,
	float		amount,
	float		armor_piercing,
	u32			time_in_ms,
	bullet*		bullet)
{
	// LOCALS
	// damage_protector* 			prot
	// body_part_parameters* 		last_hitted_body_part
	// body_part_parameters* 		part
	// pcstr 						hit_type
	// ******

	return false;
	// FUNCTION BODY
	// 1
	// <0x6ffafa>|0x000|0x000:'145'
	// 1
	// <0x6ffb2d>|0x033|0x033:'147'
	// <0x6ffb3c>|0x042|0x00f:'148'
	// 1
	// 2
	// 3
	// <0x6ffb42>|0x048|0x006:'152'
	// <0x6ffb86>|0x08c|0x044:'153'
	// 1
	// <0x6ffb8a>|0x090|0x004:'155'
	// <0x6ffb96>|0x09c|0x00c:'156'
	// <0x6ffb9c>|0x0a2|0x006:'157'
	// <0x6ffba8>|0x0ae|0x00c:'158'
	// <0x6ffbcd>|0x0d3|0x025:'159'
	// 1
	// <0x6ffbf1>|0x0f7|0x024:'161'
	// <0x6ffbf7>|0x0fd|0x006:'162'
	// 1
	// <0x6ffc00>|0x106|0x009:'164'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::apply_med_kit(char const*, const float)
void damage_model::apply_med_kit( pcstr part_name, float amount )
{
	// LOCALS
	// body_part_parameters* 		part
	// ******

	// FUNCTION BODY
	// <0x6ff8e9>|0x000|0x000:'169'
	// <0x6ff8f8>|0x00f|0x00f:'170'
	// <0x6ff904>|0x01b|0x00c:'171'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::fill_stats(vostok::ai::npc_statistics&, const unsigned int) const
void damage_model::fill_stats( vostok::ai::npc_statistics& stats, u32 current_time_in_ms ) const
{
	// LOCALS
	// dump_npc_body_part_state_predicate dump_predicate
	// vostok::ai::statistics_item<46,16> new_stats_item
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::ai::statistics_item<46,16>
	// 	content_type;

	// ******

	// FUNCTION BODY
	// 1
	// <0x6ff800>|0x000|0x000:'211'
	// <0x6ff80b>|0x00b|0x00b:'212'
	// <0x6ff81b>|0x01b|0x010:'213'
	// 1
	// <0x6ff86b>|0x06b|0x050:'215'
	// <0x6ff888>|0x088|0x01d:'216'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::dump_stats(boost::function<void __cdecl(unsigned int,float,float,char const *)>)
void damage_model::dump_stats( boost::function<void __cdecl(u32,float,float,pcstr)> callback )
{
	// LOCALS
	// body_part_parameters const* 	body_part
	// u32 							body_part_index
	// ******

	// FUNCTION BODY
	// <0x6ff92a>|0x000|0x000:'231'
	// <0x6ff936>|0x00c|0x00c:'232'
	// <0x6ff93d>|0x013|0x007:'233'
	// 1
	// <0x6ff943>|0x019|0x006:'235'
	// <0x6ff96b>|0x041|0x028:'236'
	// <0x6ff973>|0x049|0x008:'237'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::subscribe_on_affect(const survarium::hit_affects_type_enum, survarium::affect_subscriber* const)
void damage_model::subscribe_on_affect( hit_affects_type_enum affect_type, affect_subscriber* subscriber )
{
	// FUNCTION BODY
	// <0x6ffa39>|0x000|0x000:'242'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::unsubscribe_from_affect(const survarium::hit_affects_type_enum, survarium::affect_subscriber* const)
void damage_model::unsubscribe_from_affect( hit_affects_type_enum affect_type, affect_subscriber* subscriber )
{
	// LOCALS
	// vostok::intrusive_list<affect_subscriber,affect_subscriber *,32,vostok::threading::mutex,vostok::size_policy,vostok::no_debug_policy>& subscribers
	// ******

	// FUNCTION BODY
	// <0x6ff9f9>|0x000|0x000:'247'
	// <0x6ffa0c>|0x013|0x013:'248'
	// <0x6ffa18>|0x01f|0x00c:'249'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::notify_on_affect_event(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum)
void damage_model::notify_on_affect_event( pcstr body_part_name, hit_affects_type_enum affect_type, affect_event_type_enum event_type )
{
	// LOCALS
	// vostok::intrusive_list<affect_subscriber,affect_subscriber *,32,vostok::threading::mutex,vostok::size_policy,vostok::no_debug_policy>& subscribers<1>
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x6ff99f>|0x000|0x000|[1]:'294'
	// <0x6ff9b5>|0x016|0x016:'295'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::damage_model::reset()
void damage_model::reset( )
{
	// FUNCTION BODY
	// <0x6ff589>|0x000|0x000:'322'
	// 1
	// <0x6ff593>|0x00a|0x00a:'324'
	// <0x6ff59d>|0x014|0x00a:'325'
	// 1
	// <0x6ff5a7>|0x01e|0x00a:'327'
	// <0x6ff5b1>|0x028|0x00a:'328'
	// 1
	// <0x6ff5bb>|0x032|0x00a:'330'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::apply_affect(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum)
void damage_model::apply_affect( pcstr part_name, hit_affects_type_enum affect, affect_event_type_enum event_type )
{
	// LOCALS
	// body_part_parameters* 		part
	// ******

	// FUNCTION BODY
	// <0x6ffaa9>|0x000|0x000:'335'
	// <0x6ffab8>|0x00f|0x00f:'336'
	// <0x6ffac4>|0x01b|0x00c:'337'
	// ******
}

// STATE[STUB]
// unsigned char survarium::damage_model::get_total_health()
u8 damage_model::get_total_health( )
{
	// LOCALS
	// u8 							result_value
	// body_part_parameters* 		it_body_part<1>
	// ******

	return 0;
	// FUNCTION BODY
	// <0x6ff529>|0x000|0x000:'342'
	// 1
	// <0x6ff52d>|0x004|0x004|[1]:'344'
	// 1
	// <0x6ff549>|0x020|0x01c:'346'
	// <0x6ff56b>|0x042|0x022:'347'
	// <0x6ff576>|0x04d|0x00b:'348'
	// <0x6ff578>|0x04f|0x002:'349'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::cancel_affect(char const*, const survarium::hit_affects_type_enum)
void damage_model::cancel_affect( pcstr part_name, hit_affects_type_enum affect )
{
	// LOCALS
	// body_part_parameters* 		part
	// ******

	// FUNCTION BODY
	// <0x6ffa69>|0x000|0x000:'354'
	// <0x6ffa78>|0x00f|0x00f:'355'
	// <0x6ffa84>|0x01b|0x00c:'356'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::register_body_part_damage_protector(char const*, survarium::damage_protector*)
void damage_model::register_body_part_damage_protector( pcstr part_name, damage_protector* protector )
{
	// LOCALS
	// body_part_parameters* 		part
	// ******

	// FUNCTION BODY
	// <0x6ff7b9>|0x000|0x000:'375'
	// <0x6ff7c8>|0x00f|0x00f:'376'
	// <0x6ff7d4>|0x01b|0x00c:'377'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::unregister_body_part_damage_protector(char const*, survarium::damage_protector*)
void damage_model::unregister_body_part_damage_protector( pcstr part_name, damage_protector* protector )
{
	// LOCALS
	// body_part_parameters* 		part
	// ******

	// FUNCTION BODY
	// <0x6ff779>|0x000|0x000:'382'
	// <0x6ff788>|0x00f|0x00f:'383'
	// <0x6ff794>|0x01b|0x00c:'384'
	// ******
}

// STATE[STUB]
// void survarium::damage_model::add_damage_protector(char const*, float, float)
void damage_model::add_damage_protector( pcstr damage_type, float reduce, float absorb )
{
	// LOCALS
	// booster_damage_protector* 	protector
	// ******

	// FUNCTION BODY
	// <0x6ffcf0>|0x000|0x000:'389'
	// 1
	// <0x6ffd18>|0x028|0x028:'391'
	// 1
	// <0x6ffd1e>|0x02e|0x006:'393'
	// <0x6ffd33>|0x043|0x015:'394'
	// 1
	// <0x6ffd48>|0x058|0x015:'396'
	// 1
	// <0x6ffd4a>|0x05a|0x002:'398'
	// <0x6ffdb0>|0x0c0|0x066:'399'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::damage_model::on_broken_limb_affect(char const*, const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum)
void damage_model::on_broken_limb_affect( pcstr bodypart, hit_affects_type_enum affect, affect_event_type_enum type )
{
	// FUNCTION BODY
	// <0x6ff669>|0x000|0x000:'405'
	// 1
	// <0x6ff683>|0x01a|0x01a:'407'
	// 1
	// <0x6ff69a>|0x031|0x017:'409'
	// 1
	// <0x6ff6b4>|0x04b|0x01a:'411'
	// 1
	// <0x6ff6c8>|0x05f|0x014:'413'
	// 1
	// <0x6ff6e2>|0x079|0x01a:'415'
	// 1
	// <0x6ff6f6>|0x08d|0x014:'417'
	// 1
	// <0x6ff710>|0x0a7|0x01a:'419'
	// 1
	// 2
	// 3
	// 4
	// 5
	// ******
}

// STATE[STUB]
// void survarium::damage_model::deserialize(vostok::network_core::packet_reader&)
void damage_model::deserialize( vostok::network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x6ffc19>|0x000|0x000:'434'
	// ******
}

	/* TYPEDEFS

	typedef
		vostok::ai::statistics_item<46,16>
		part_info_type;

} // namespace survarium
