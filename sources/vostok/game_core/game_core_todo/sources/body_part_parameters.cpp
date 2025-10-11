////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "body_part_parameters.h"

namespace survarium {

// STATE[STUB]
// survarium::body_part_parameters::body_part_parameters(char const*, const float, const float, const float, const bool, survarium::damage_model&, const unsigned char)
body_part_parameters::body_part_parameters(
	pcstr				name,
	float				health,
	float				regeneration_speed,
	float				regeneration_timeout,
	bool				can_be_assigned,
	damage_model&		owner,
	u8					damage_group)
{
	// FUNCTION BODY
	// <0x597105>|0x000|0x000:'29'
	// ******
}

// STATE[STUB]
// survarium::hit_type_parameters* survarium::body_part_parameters::pop_hit_type()
hit_type_parameters* body_part_parameters::pop_hit_type( )
{
	return NULL;
	// FUNCTION BODY
	// <0x596f99>|0x000|0x000:'34'
	// ******
}

// STATE[STUB]
// survarium::affects_threshold* survarium::body_part_parameters::pop_threshold()
affects_threshold* body_part_parameters::pop_threshold( )
{
	return NULL;
	// FUNCTION BODY
	// <0x596f59>|0x000|0x000:'39'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::add_hit_type(survarium::hit_type_parameters* const)
void body_part_parameters::add_hit_type( hit_type_parameters* new_hit_type )
{
	// FUNCTION BODY
	// <0x596f39>|0x000|0x000:'44'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::add_threshold(survarium::affects_threshold* const)
void body_part_parameters::add_threshold( affects_threshold* new_threshold )
{
	// FUNCTION BODY
	// <0x596f19>|0x000|0x000:'49'
	// ******
}

// STATE[STUB]
// survarium::hit_type_parameters* survarium::body_part_parameters::get_hit_parameters(char const*) const
hit_type_parameters* body_part_parameters::get_hit_parameters( pcstr hit_type ) const
{
	// LOCALS
	// find_hit_parameters_by_type_predicate find_predicate
	// ******

	return NULL;
	// FUNCTION BODY
	// <0x5971b9>|0x000|0x000:'69'
	// <0x5971c7>|0x00e|0x00e:'70'
	// ******
}

// STATE[STUB]
// void survarium::protect_damage_predicate::operator()(survarium::damage_protector* const)
void protect_damage_predicate::operator()( damage_protector* protector )
{
	// FUNCTION BODY
	// <0xc9eff>|0x000|0x000:'82'
	// <0xc9f2f>|0x030|0x030:'83'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::protect_affect_predicate::operator()(survarium::damage_protector* const)
void protect_affect_predicate::operator()( damage_protector* protector )
{
	// FUNCTION BODY
	// <0xc9f8f>|0x000|0x000:'101'
	// <0xc9fb9>|0x02a|0x02a:'102'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::hit_by_type(char const*, const unsigned int, const float, const float, const bool, survarium::damage_protector*)
void body_part_parameters::hit_by_type(
	pcstr					hit_type,
	u32						time_in_ms,
	float					amount,
	float					armor_piercing,
	bool					__formal,
	damage_protector*		prot)
{
	// LOCALS
	// float 						delta
	// float 						e_wnd
	// float 						arp_arm_coeff
	// hit_type_parameters* 		params
	// protect_damage_predicate 	p
	// ******

	// FUNCTION BODY
	// <0x59791f>|0x000|0x000:'112'
	// <0x597931>|0x012|0x012:'113'
	// 1
	// <0x59793d>|0x01e|0x00c:'115'
	// 1
	// 2
	// 3
	// <0x597947>|0x028|0x00a:'119'
	// <0x597966>|0x047|0x01f:'120'
	// <0x597973>|0x054|0x00d:'121'
	// <0x597975>|0x056|0x002:'122'
	// 1
	// <0x5979a6>|0x087|0x031:'124'
	// 1
	// <0x5979bd>|0x09e|0x017:'126'
	// 1
	// <0x597a2f>|0x110|0x072:'128'
	// <0x597a68>|0x149|0x039:'129'
	// <0x597a93>|0x174|0x02b:'130'
	// 1
	// <0x597aaa>|0x18b|0x017:'132'
	// <0x597ab0>|0x191|0x006:'133'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x597afc>|0x1dd|0x04c:'140'
	// <0x597b0e>|0x1ef|0x012:'141'
	// <0x597b26>|0x207|0x018:'142'
	// 1
	// <0x597b35>|0x216|0x00f:'144'
	// <0x597b53>|0x234|0x01e:'145'
	// 1
	// <0x597b62>|0x243|0x00f:'147'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::increase_health(const float)
void body_part_parameters::increase_health( float amount )
{
	// FUNCTION BODY
	// <0x596ec9>|0x000|0x000:'152'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::decrease_health(const float)
void body_part_parameters::decrease_health( float amount )
{
	// FUNCTION BODY
	// <0x596e79>|0x000|0x000:'157'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::regenerate(const unsigned int, const unsigned int)
void body_part_parameters::regenerate( u32 time_delta_ms, u32 current_time_in_ms )
{
	// LOCALS
	// u32 							regenerate_delta
	// float 						amount
	// u32 							next_regen_time<1>
	// u32 							regen_allowed<1>
	// ******

	// SKIPPED BLOCKS
	// <0x597879><1>
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x597869>|0x000|0x000:'167'
	// 1
	// <0x59786f>|0x006|0x006:'169'
	// 1
	// <0x59787b>|0x012|0x00c:'171'
	// 1
	// <0x597890>|0x027|0x015:'173'
	// <0x597898>|0x02f|0x008:'174'
	// 1
	// <0x59789a>|0x031|0x002:'176'
	// <0x5978a3>|0x03a|0x009:'177'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x5978b1>|0x048|0x00e:'183'
	// <0x5978d3>|0x06a|0x022:'184'
	// 1
	// <0x5978e2>|0x079|0x00f:'186'
	// <0x5978f7>|0x08e|0x015:'187'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::update_affects(const unsigned int)
void body_part_parameters::update_affects( u32 current_time_in_ms )
{
	// LOCALS
	// s32 							i<1>
	// std::pair<enum hit_affects_type_enum,u32>* it_affect<2>
	// ******

	// FUNCTION BODY
	// <0x5977a9>|0x000|0x000|[1]:'192'
	// 1
	// <0x5977d7>|0x02e|0x02e|[2]:'194'
	// <0x5977e6>|0x03d|0x00f:'195'
	// 1
	// <0x597804>|0x05b|0x01e:'197'
	// <0x597810>|0x067|0x00c:'198'
	// <0x59782f>|0x086|0x01f:'199'
	// <0x59784b>|0x0a2|0x01c:'200'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::cancel_affect_by_force(const survarium::hit_affects_type_enum)
void body_part_parameters::cancel_affect_by_force( hit_affects_type_enum affect )
{
	// LOCALS
	// s32 							i<1>
	// std::pair<enum hit_affects_type_enum,u32>* it_affect<2>
	// ******

	// FUNCTION BODY
	// <0x5976e9>|0x000|0x000|[1]:'206'
	// 1
	// <0x597717>|0x02e|0x02e|[2]:'208'
	// <0x597726>|0x03d|0x00f:'209'
	// 1
	// <0x597743>|0x05a|0x01d:'211'
	// <0x59774f>|0x066|0x00c:'212'
	// <0x59776e>|0x085|0x01f:'213'
	// <0x59778a>|0x0a1|0x01c:'214'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::check_affects(const unsigned int)
void body_part_parameters::check_affects( u32 current_time_in_ms )
{
	// LOCALS
	// affects_threshold* 			it_threshold<1>
	// ******

	// FUNCTION BODY
	// <0x597679>|0x000|0x000|[1]:'220'
	// 1
	// <0x597692>|0x019|0x019:'222'
	// 1
	// <0x5976bb>|0x042|0x029:'224'
	// <0x5976d4>|0x05b|0x019:'225'
	// 1
	// ******
}

// STATE[STUB]
// bool survarium::body_part_parameters::is_affect_applied(const survarium::hit_affects_type_enum)
bool body_part_parameters::is_affect_applied( hit_affects_type_enum affect )
{
	// LOCALS
	// u32 							i<1>
	// ******

	return false;
	// FUNCTION BODY
	// <0x596da9>|0x000|0x000|[1]:'231'
	// <0x596dd7>|0x02e|0x02e:'232'
	// <0x596df4>|0x04b|0x01d:'233'
	// 1
	// <0x596df8>|0x04f|0x004:'235'
	// ******
}

// STATE[STUB]
// bool survarium::body_part_parameters::has_affect_protector(const survarium::hit_affects_type_enum)
bool body_part_parameters::has_affect_protector( hit_affects_type_enum affect )
{
	// LOCALS
	// protect_affect_predicate 	p
	// ******

	return false;
	// FUNCTION BODY
	// <0x5973e9>|0x000|0x000:'240'
	// <0x59740f>|0x026|0x026:'241'
	// <0x597437>|0x04e|0x028:'242'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::apply_affects(survarium::affects_threshold const*, const unsigned int)
void body_part_parameters::apply_affects( affects_threshold const* threshold_reached, u32 current_time_in_ms )
{
	// LOCALS
	// hit_affects_type_enum const* it_begin
	// hit_affects_type_enum const* it_end
	// hit_affects_type_enum const* it<1>
	// ******

	// FUNCTION BODY
	// <0x597599>|0x000|0x000:'247'
	// <0x5975ae>|0x015|0x015:'248'
	// 1
	// <0x5975c3>|0x02a|0x015|[1]:'250'
	// <0x5975e0>|0x047|0x01d:'251'
	// 1
	// <0x59760a>|0x071|0x02a:'253'
	// <0x597629>|0x090|0x01f:'254'
	// <0x597664>|0x0cb|0x03b:'255'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::apply_affect_by_force(const survarium::hit_affects_type_enum, const survarium::affect_event_type_enum, const unsigned int)
void body_part_parameters::apply_affect_by_force( hit_affects_type_enum affect, affect_event_type_enum event_type, u32 current_time_in_ms )
{
	// LOCALS
	// std::pair<enum hit_affects_type_enum,u32>* it_affect<1>
	// u32 							i<2>
	// ******

	// FUNCTION BODY
	// <0x597459>|0x000|0x000:'260'
	// 1
	// <0x597472>|0x019|0x019:'262'
	// <0x597491>|0x038|0x01f:'263'
	// 1
	// 2
	// <0x5974cd>|0x074|0x03c:'266'
	// 1
	// <0x5974d7>|0x07e|0x00a|[1]:'268'
	// <0x5974e6>|0x08d|0x00f|[2]:'269'
	// 1
	// <0x597514>|0x0bb|0x02e:'271'
	// 1
	// <0x597531>|0x0d8|0x01d:'273'
	// <0x59753d>|0x0e4|0x00c:'274'
	// <0x59755a>|0x101|0x01d:'275'
	// 1
	// <0x597576>|0x11d|0x01c:'277'
	// 1
	// <0x597578>|0x11f|0x002:'279'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::fill_new_stats_item<vostok::ai::statistics_item<46,16> >(vostok::ai::statistics_item<46,16>&, const unsigned int) const
void body_part_parameters::fill_new_stats_item<vostok::ai::statistics_item<46,16> >( vostok::ai::statistics_item<46,16>& new_stats_item, u32 current_time_in_ms ) const
{
	// LOCALS
	// vostok::fixed_string<46> 	new_item
	// u32 							i<1>
	// u32 							remaining_time_in_ms<2>
	// ******

	// SKIPPED BLOCKS
	// <0xca49d><2>
	// ******

	// FUNCTION BODY
	// <0xca3d0>|0x000|0x000:'286'
	// <0xca3e1>|0x011|0x011:'287'
	// 1
	// <0xca3ee>|0x01e|0x00d:'289'
	// <0xca3f6>|0x026|0x008:'290'
	// <0xca432>|0x062|0x03c:'291'
	// 1
	// <0xca441>|0x071|0x00f:'293'
	// <0xca449>|0x079|0x008:'294'
	// <0xca456>|0x086|0x00d:'295'
	// 1
	// <0xca465>|0x095|0x00f|[1]:'297'
	// 1
	// <0xca4a3>|0x0d3|0x03e:'299'
	// <0xca4ab>|0x0db|0x008:'300'
	// <0xca512>|0x142|0x067:'301'
	// <0xca570>|0x1a0|0x05e:'302'
	// <0xca57f>|0x1af|0x00f:'303'
	// 1
	// <0xca584>|0x1b4|0x005:'305'
	// <0xca5b0>|0x1e0|0x02c:'306'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::dump_state(vostok::ai::npc_statistics&, const unsigned int) const
void body_part_parameters::dump_state( vostok::ai::npc_statistics& stats, u32 current_time_in_ms ) const
{
	// LOCALS
	// vostok::ai::statistics_item<46,16> new_stats_item
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::ai::statistics_item<46,16>
	// 	content_type;

	// ******

	// FUNCTION BODY
	// 1
	// <0x59714f>|0x000|0x000:'312'
	// <0x597165>|0x016|0x016:'313'
	// <0x59717b>|0x02c|0x016:'314'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::dump_state(boost::function<void __cdecl(unsigned int,float,float,char const *)>, const unsigned int) const
void body_part_parameters::dump_state( boost::function<void __cdecl(u32,float,float,pcstr)> callback, u32 index ) const
{
	// LOCALS
	// vostok::fixed_string<512> 	affects_str
	// u32 							i<1>
	// ******

	// FUNCTION BODY
	// <0x597300>|0x000|0x000:'326'
	// <0x59730b>|0x00b|0x00b|[1]:'327'
	// <0x597351>|0x051|0x046:'328'
	// <0x597390>|0x090|0x03f:'329'
	// <0x597392>|0x092|0x002:'330'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::reset()
void body_part_parameters::reset( )
{
	// FUNCTION BODY
	// <0x596d49>|0x000|0x000:'373'
	// <0x596d5b>|0x012|0x012:'374'
	// <0x596d68>|0x01f|0x00d:'375'
	// ******
}

// STATE[STUB]
// bool survarium::body_part_parameters::can_affect_death()
bool body_part_parameters::can_affect_death( )
{
	// LOCALS
	// affects_threshold* 			it_threshold<1>
	// hit_affects_type_enum const* threshold_affects<2>
	// u32 							threshold_affects_count<2>
	// u32 							i<3>
	// ******

	return false;
	// FUNCTION BODY
	// <0x596cc9>|0x000|0x000|[1]:'380'
	// 1
	// <0x596ce2>|0x019|0x019|[2]:'382'
	// <0x596cf7>|0x02e|0x015:'383'
	// 1
	// <0x596d00>|0x037|0x009|[3]:'385'
	// 1
	// <0x596d1a>|0x051|0x01a:'387'
	// <0x596d26>|0x05d|0x00c:'388'
	// <0x596d2a>|0x061|0x004:'389'
	// <0x596d2c>|0x063|0x002:'390'
	// <0x596d2e>|0x065|0x002:'391'
	// ******
}

// STATE[STUB]
// unsigned char survarium::body_part_parameters::get_health_in_percentage()
u8 body_part_parameters::get_health_in_percentage( )
{
	return 0;
	// FUNCTION BODY
	// <0x596c97>|0x000|0x000:'396'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::add_damage_protector(survarium::damage_protector*)
void body_part_parameters::add_damage_protector( damage_protector* protector )
{
	// FUNCTION BODY
	// <0x596e49>|0x000|0x000:'401'
	// <0x596e55>|0x00c|0x00c:'402'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::remove_damage_protector(survarium::damage_protector*)
void body_part_parameters::remove_damage_protector( damage_protector* protector )
{
	// FUNCTION BODY
	// <0x596e19>|0x000|0x000:'407'
	// <0x596e25>|0x00c|0x00c:'408'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::set_parameters(float, float)
void body_part_parameters::set_parameters( float max_health, float regeneration_speed )
{
	// FUNCTION BODY
	// <0x596c67>|0x000|0x000:'413'
	// <0x596c77>|0x010|0x010:'414'
	// ******
}

// STATE[STUB]
// void survarium::serialize_affect(vostok::network_core::udp_match_packet&, stlp_std::pair<enum survarium::hit_affects_type_enum,unsigned int> const&, int)
void serialize_affect( vostok::network_core::udp_match_packet& packet, std::pair<enum hit_affects_type_enum,u32> const& affect, s32 client_offset )
{
	// FUNCTION BODY
	// <0xc9ff6>|0x000|0x000:'419'
	// <0xca005>|0x00f|0x00f:'420'
	// ******
}

// STATE[STUB]
// void survarium::deserialize_affect(vostok::network_core::packet_reader&, stlp_std::pair<enum survarium::hit_affects_type_enum,unsigned int>&)
void deserialize_affect( vostok::network_core::packet_reader& reader, std::pair<enum hit_affects_type_enum,u32>& affect )
{
	// FUNCTION BODY
	// <0xca026>|0x000|0x000:'425'
	// <0xca036>|0x010|0x010:'426'
	// 1
	// <0xca044>|0x01e|0x00e:'428'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::serialize(vostok::network_core::udp_match_packet&, int) const
void body_part_parameters::serialize( vostok::network_core::udp_match_packet& packet, s32 client_offset ) const
{
	// FUNCTION BODY
	// <0x5971f9>|0x000|0x000:'433'
	// <0x59720e>|0x015|0x015:'434'
	// <0x59723e>|0x045|0x030:'435'
	// 1
	// <0x59725e>|0x065|0x020:'437'
	// ******
}

// STATE[STUB]
// void survarium::body_part_parameters::deserialize(vostok::network_core::packet_reader&)
void body_part_parameters::deserialize( vostok::network_core::packet_reader& reader )
{
	// LOCALS
	// u8 							affects_count
	// std::pair<enum hit_affects_type_enum,u32> affect<1>
	// ******

	// SKIPPED BLOCKS
	// <0x597bf1><1>
	// ******

	// FUNCTION BODY
	// <0x597b99>|0x000|0x000:'442'
	// <0x597bac>|0x013|0x013:'443'
	// <0x597bbd>|0x024|0x011:'444'
	// 1
	// <0x597bc8>|0x02f|0x00b:'446'
	// <0x597bd4>|0x03b|0x00c:'447'
	// 1
	// <0x597be0>|0x047|0x00c:'449'
	// 1
	// <0x597bf3>|0x05a|0x013:'451'
	// <0x597c01>|0x068|0x00e:'452'
	// 1
	// <0x597c11>|0x078|0x010:'454'
	// 1
	// ******
}

	/* TYPEDEFS

	typedef
		vostok::ai::statistics_item<46,16>
		part_info_type;

} // namespace survarium
