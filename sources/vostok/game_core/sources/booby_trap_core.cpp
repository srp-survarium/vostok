////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/booby_trap_core.h>

namespace survarium {

// STATE[STUB]
// survarium::booby_trap_core::booby_trap_core()
booby_trap_core::booby_trap_core( )
{
	// FUNCTION BODY
	// <0x59be6a>|0x0da|+0x00e:'26'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::~booby_trap_core()
booby_trap_core::~booby_trap_core( )
{
	// FUNCTION BODY
	// <0x59bed4>|0x054|+0x01c:'31'
	// <0x59bef0>|0x070|+0x01c:'32'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::load(vostok::configs::binary_config_value const&)
void booby_trap_core::load( configs::binary_config_value const& config )
{
	// FUNCTION BODY
	// <0x59bf69>|0x009|+0x00c:'37'
	// <0x59bf75>|0x015|+0x00c:'38'
	// <0x59bf81>|0x021|+0x00c:'39'
	// <0x59bf8d>|0x02d|+0x00c:'40'
	// <0x59bf99>|0x039|+0x00c:'41'
	// <0>
	// <0x59bfa5>|0x045|+0x016:'43'
	// <0x59bfbb>|0x05b|+0x047:'44'
	// <0x59c002>|0x0a2|+0x032:'45'
	// <0>
	// <0x59c034>|0x0d4|+0x019:'47'
	// <0x59c04d>|0x0ed|+0x047:'48'
	// <0x59c094>|0x134|+0x032:'49'
	// <0>
	// <0x59c0c6>|0x166|+0x014:'51'
	// <0>
	// <0x59c0da>|0x17a|+0x019:'53'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::load_aabb(vostok::configs::binary_config_value const&)
void booby_trap_core::load_aabb( configs::binary_config_value const& __formal )
{
	// FUNCTION BODY
	// <0x59b110>|0x000|+0x007:'63'	{
	// <0>
	// <0x59b117>|0x007|      :'65'	}
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::set_transform(vostok::math::float4x4 const&)
void booby_trap_core::set_transform( float4x4 const& transform )
{
	// CALL SITE INFO
	// <0x59b3fd> -> void <unknown>(float4x4 const&)
	// <0x59b41e> -> void <unknown>(float4x4 const&)
	// ******

	// FUNCTION BODY
	// <0x59b3cb>|0x00b|+0x013:'69'
	// <0>
	// <0x59b3de>|0x01e|+0x021:'71'
	// <0x59b3ff>|0x03f|+0x021:'72'
	// <0>
	// <0x59b420>|0x060|+0x00c:'74'
	// <0x59b42c>|0x06c|+0x016:'75'
	// <0x59b442>|0x082|+0x012:'76'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::apply_damage(survarium::hit_initiator const* const, survarium::hit_receiver* const)
void booby_trap_core::apply_damage( hit_initiator const* initiator, hit_receiver* receiver )
{
	// LOCALS
	// buffer_vector<booby_trap_set_core::apply_damage> const& damage_parameters
	// booby_trap_set_core::apply_damage const* end
	// booby_trap_set_core::apply_damage const* it
	// collision::bone_collision_data bcd<1>
	// ******

	// SKIPPED BLOCKS
	// <0x59ba07><1>
	// ******

	// TYPEDEFS
	// typedef
	// 	buffer_vector<booby_trap_set_core::apply_damage>
	// 	damage_parameters_type;

	// ******

	// CALL SITE INFO
	// <0x59ba4c> -> void <unknown>(hit_initiator const* const, collision::bone_collision_data const&, pcstr, const float, const float, bullet* const)
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x59b9d0>|0x010|+0x015:'86'
	// <0x59b9e5>|0x025|+0x008:'87'
	// <0x59b9ed>|0x02d|+0x009:'88'
	// <0x59b9f6>|0x036|+0x013:'89'
	// <0>
	// <0x59ba09>|0x049|+0x013:'91'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x59ba1c>|0x05c|+0x034:'101'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::on_enter(vostok::buffer_vector<vostok::physics::base_physics_object *> const&)
void booby_trap_core::on_enter( buffer_vector<physics::base_physics_object *> const& objects )
{
	// LOCALS
	// physics::base_physics_object** end
	// physics::base_physics_object** it
	// hit_receiver* 				receiver<1>
	// hit_initiator const* 		initiator<1>
	// ******

	// CALL SITE INFO
	// <0x59bb77> -> base_player* <unknown>()
	// <0x59bbae> -> hit_receiver* <unknown>()
	// <0x59bc10> -> void <unknown>(booby_trap_state)
	// ******

	// FUNCTION BODY
	// <0x59bb19>|0x009|+0x00c:'107'
	// <0x59bb25>|0x015|+0x00c:'108'
	// <0>
	// <0x59bb31>|0x021|+0x008:'110'
	// <0x59bb39>|0x029|+0x009:'111'
	// <0x59bb42>|0x032|+0x017:'112'
	// <0>
	// <0x59bb59>|0x049|+0x041|[1]:'114'
	// <0x59bb9a>|0x08a|+0x019:'115'
	// <0>
	// <0x59bbb3>|0x0a3|+0x00c:'117'
	// <0x59bbbf>|0x0af|+0x00c:'118'
	// <0>
	// <0x59bbcb>|0x0bb|+0x016:'120'
	// <0x59bbe1>|0x0d1|+0x005:'121'
	// <0>
	// <0x59bbe6>|0x0d6|+0x013:'123'
	// <0x59bbf9>|0x0e9|+0x019:'124'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::tick(const unsigned int, const unsigned int)
void booby_trap_core::tick( u32 time_delta_ms, u32 current_time_ms )
{
	// FUNCTION BODY
	// <0x59bd19>|0x009|+0x00c:'129'
	// <0>
	// <0x59bd25>|0x015|+0x00c:'131'
	// <0>
	// <0x59bd31>|0x021|+0x00e:'133'
	// <0>
	// <0x59bd3f>|0x02f|+0x00e:'135'
	// <0>
	// <0x59bd4d>|0x03d|+0x002:'137'
	// <0>
	// <0x59bd4f>|0x03f|+0x015:'139'
	// <0>
	// <1>
	// <2>
	// <0x59bd64>|0x054|+0x009:'143'
	// <0>
	// <0x59bd6d>|0x05d|+0x010:'145'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::insert(vostok::physics::world*, vostok::math::float4x4 const&, survarium::scheduler&)
void booby_trap_core::insert( physics::world* world, float4x4 const& transform, scheduler& scheduler )
{
	// CALL SITE INFO
	// <0x59ba90> -> void <unknown>(float4x4 const&)
	// <0x59baa1> -> void <unknown>(scheduler&)
	// <0x59bb08> -> void <unknown>(booby_trap_state)
	// ******

	// FUNCTION BODY
	// <0x59ba69>|0x009|+0x00c:'151'
	// <0x59ba75>|0x015|+0x00c:'152'
	// <0>
	// <0x59ba81>|0x021|+0x011:'154'
	// <0>
	// <0x59ba92>|0x032|+0x011:'156'
	// <0>
	// <0x59baa3>|0x043|+0x00c:'158'
	// <0x59baaf>|0x04f|+0x012:'159'
	// <0x59bac1>|0x061|+0x012:'160'
	// <0x59bad3>|0x073|+0x016:'161'
	// <0x59bae9>|0x089|+0x012:'162'
	// <0>
	// <0x59bafb>|0x09b|+0x00f:'164'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::remove(survarium::scheduler&)
void booby_trap_core::remove( scheduler& scheduler )
{
	// CALL SITE INFO
	// <0x59b3a4> -> void <unknown>(scheduler&)
	// <0x59b3b3> -> void <unknown>(booby_trap_state)
	// ******

	// FUNCTION BODY
	// <0x59b389>|0x009|+0x00c:'169'
	// <0>
	// <0x59b395>|0x015|+0x011:'171'
	// <0>
	// <0x59b3a6>|0x026|+0x00f:'173'
	// ******
}

// STATE[STUB]
// bool survarium::booby_trap_core::use_initialize(survarium::usable_object_user_data*)
bool booby_trap_core::use_initialize( usable_object_user_data* user )
{
	// LOCALS
	// base_player* 				user_player
	// ******

	// CALL SITE INFO
	// <0x59b607> -> base_player* <unknown>()
	// ******

	return false;

	// FUNCTION BODY
	// <0x59b5c9>|0x009|+0x00c:'178'
	// <0x59b5d5>|0x015|+0x00c:'179'
	// <0>
	// <0x59b5e1>|0x021|+0x013:'181'
	// <0x59b5f4>|0x034|+0x004:'182'
	// <0>
	// <0x59b5f8>|0x038|+0x014:'184'
	// <0x59b60c>|0x04c|+0x00c:'185'
	// <0>
	// <0x59b618>|0x058|+0x019:'187'
	// <0x59b631>|0x071|+0x004:'188'
	// <0>
	// <0x59b635>|0x075|+0x011:'190'
	// <0x59b646>|0x086|+0x023:'191'
	// <0x59b669>|0x0a9|+0x00c:'192'
	// <0>
	// <0x59b675>|0x0b5|+0x002:'194'
	// ******
}

// STATE[STUB]
// bool survarium::booby_trap_core::use_execute(survarium::usable_object_user_data*)
bool booby_trap_core::use_execute( usable_object_user_data* user )
{
	// LOCALS
	// u32 							defuse_time_ms
	// float 						engineer_factor
	// u32 							config_defuse_time
	// u32 							passed_ms
	// ******

	// CALL SITE INFO
	// <0x59b53f> -> void <unknown>()
	// ******

	return false;

	// FUNCTION BODY
	// <0x59b469>|0x009|+0x00c:'199'
	// <0x59b475>|0x015|+0x00c:'200'
	// <0>
	// <0x59b481>|0x021|+0x00c:'202'
	// <0x59b48d>|0x02d|+0x00c:'203'
	// <0>
	// <0x59b499>|0x039|+0x00f:'205'
	// <0x59b4a8>|0x048|+0x00f:'206'
	// <0x59b4b7>|0x057|+0x011:'207'
	// <0x59b4c8>|0x068|+0x022:'208'
	// <0>
	// <0x59b4ea>|0x08a|+0x032:'210'
	// <0>
	// <0x59b51c>|0x0bc|+0x00e:'212'
	// <0>
	// <1>
	// <0x59b52a>|0x0ca|+0x017:'215'
	// <0x59b541>|0x0e1|+0x004:'216'
	// <0>
	// <1>
	// <0x59b545>|0x0e5|+0x002:'219'
	// ******
}

// STATE[STUB]
// bool survarium::booby_trap_core::use_finalize(survarium::usable_object_user_data*)
bool booby_trap_core::use_finalize( usable_object_user_data* user )
{
	return false;

	// FUNCTION BODY
	// <0x59b559>|0x009|+0x00c:'224'
	// <0x59b565>|0x015|+0x00c:'225'
	// <0>
	// <1>
	// <0x59b571>|0x021|+0x00c:'228'
	// <0>
	// <0x59b57d>|0x02d|+0x00c:'230'
	// <0x59b589>|0x039|+0x00a:'231'
	// <0x59b593>|0x043|+0x00a:'232'
	// <0x59b59d>|0x04d|+0x00f:'233'
	// <0x59b5ac>|0x05c|+0x002:'234'
	// ******
}

// STATE[STUB]
// char const* survarium::booby_trap_core::use_info(survarium::usable_object_user_data*)
pcstr booby_trap_core::use_info( usable_object_user_data* user )
{
	// LOCALS
	// base_player* 				user_player
	// ******

	// CALL SITE INFO
	// <0x59b334> -> base_player* <unknown>()
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x59b319>|0x009|+0x00c:'239'
	// <0>
	// <0x59b325>|0x015|+0x014:'241'
	// <0x59b339>|0x029|+0x00c:'242'
	// <0>
	// <0x59b345>|0x035|+0x02c:'244'
	// ******
}

// STATE[STUB]
// bool survarium::booby_trap_core::can_defuse(survarium::base_player const*) const
bool booby_trap_core::can_defuse( base_player const* user ) const
{
	// LOCALS
	// base_player const* 			owner
	// ******

	// CALL SITE INFO
	// <0x59b2b3> -> base_player* <unknown>()
	// <0x59b2d7> -> game_team_id <unknown>() const
	// <0x59b2e6> -> game_team_id <unknown>() const
	// ******

	return false;

	// FUNCTION BODY
	// <0x59b27a>|0x00a|+0x00c:'249'
	// <0x59b286>|0x016|+0x00c:'250'
	// <0>
	// <0x59b292>|0x022|+0x026:'252'
	// <0x59b2b8>|0x048|+0x00c:'253'
	// <0>
	// <0x59b2c4>|0x054|+0x03b:'255'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::defuse_completed()
void booby_trap_core::defuse_completed( )
{
	// CALL SITE INFO
	// <0x59b104> -> void <unknown>(booby_trap_state)
	// ******

	// FUNCTION BODY
	// <0x59b0f7>|0x007|+0x00f:'260'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::on_state_timer_finished()
void booby_trap_core::on_state_timer_finished( )
{
	// CALL SITE INFO
	// <0x59b0be> -> void <unknown>(booby_trap_state)
	// <0x59b0dd> -> void <unknown>(booby_trap_core&)
	// ******

	// FUNCTION BODY
	// <0x59b099>|0x009|+0x018:'265'
	// <0>
	// <1>
	// <2>
	// <0x59b0b1>|0x021|+0x00f:'269'
	// <0x59b0c0>|0x030|+0x002:'270'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x59b0c2>|0x032|+0x01d:'275'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::switch_to_state(survarium::booby_trap_state)
void booby_trap_core::switch_to_state( booby_trap_state new_state )
{
	// STATICS
	// static <NoType> 				 = <0x59b9ac>;
	// ******

	// CALL SITE INFO
	// <0x59b8fc> -> void <unknown>(booby_trap_core&)
	// <0x59b921> -> void <unknown>(booby_trap_core&)
	// <0x59b972> -> void <unknown>(booby_trap_core&)
	// <0x59b994> -> void <unknown>(booby_trap_core&)
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58a871 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN15") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58a882 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN12") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58a8ad }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58a925 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// ******

	// FUNCTION BODY
	// <0x59b809>|0x009|+0x00c:'286'
	// <0>
	// <0x59b815>|0x015|+0x00c:'288'
	// <0>
	// <0x59b821>|0x021|+0x00e:'290'
	// <0x59b82f>|0x02f|+0x00e:'291'
	// <0x59b83d>|0x03d|+0x016:'292'
	// <0x59b853>|0x053|+0x00e:'293'
	// <0>
	// <1>
	// <0x59b861>|0x061|+0x010:'296'
	// <0>
	// <1>
	// <2>
	// <0x59b871>|0x071|+0x00c:'300'
	// <0x59b87d>|0x07d|+0x005:'301'
	// <0>
	// <1>
	// <2>
	// <0x59b882>|0x082|+0x00c:'305'
	// <0>
	// <0x59b88e>|0x08e|+0x01a:'307'
	// <0x59b8a8>|0x0a8|+0x005:'308'
	// <0>
	// <1>
	// <2>
	// <0x59b8ad>|0x0ad|+0x00c:'312'
	// <0x59b8b9>|0x0b9|+0x01a:'313'
	// <0>
	// <0x59b8d3>|0x0d3|+0x00c:'315'
	// <0>
	// <0x59b8df>|0x0df|+0x01f:'317'
	// <0x59b8fe>|0x0fe|+0x005:'318'
	// <0>
	// <1>
	// <0x59b903>|0x103|+0x020:'321'
	// <0x59b923>|0x123|+0x002:'322'
	// <0>
	// <1>
	// <2>
	// <0x59b925>|0x125|+0x00c:'326'
	// <0x59b931>|0x131|+0x01a:'327'
	// <0>
	// <0x59b94b>|0x14b|+0x00c:'329'
	// <0>
	// <0x59b957>|0x157|+0x01d:'331'
	// <0x59b974>|0x174|+0x002:'332'
	// <0>
	// <1>
	// <0x59b976>|0x176|+0x020:'335'
	// <0x59b996>|0x196|+0x002:'336'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x59b998>|0x198|+0x00c:'342'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::register_tick(survarium::scheduler&)
void booby_trap_core::register_tick( scheduler& scheduler )
{
	// FUNCTION BODY
	// <0x59bc30>|0x010|+0x0d4:'347'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::unregister_tick(survarium::scheduler&)
void booby_trap_core::unregister_tick( scheduler& scheduler )
{
	// FUNCTION BODY
	// <0x59b7e9>|0x009|+0x011:'352'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::serialize(vostok::network_core::udp_match_packet&) const
void booby_trap_core::serialize( network_core::udp_match_packet& packet ) const
{
	// CALL SITE INFO
	// <0x59b78b> -> void <unknown>(booby_trap_core const&, network_core::udp_match_packet&) const
	// ******

	// FUNCTION BODY
	// <0>
	// <0x59b75b>|0x00b|+0x00c:'358'
	// <0x59b767>|0x017|+0x026:'359'
	// <0>
	// <0x59b78d>|0x03d|+0x013:'361'
	// <0x59b7a0>|0x050|+0x015:'362'
	// <0x59b7b5>|0x065|+0x019:'363'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::deserialize(vostok::network_core::packet_reader&)
void booby_trap_core::deserialize( network_core::packet_reader& reader )
{
	// LOCALS
	// booby_trap_state 			state
	// float4x4 					transform
	// float3 						angles
	// float3 						position
	// ******

	// CALL SITE INFO
	// <0x59b727> -> void <unknown>(booby_trap_core&, float4x4 const&)
	// <0x59b744> -> void <unknown>(booby_trap_state)
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x59b68f>|0x00f|+0x00e:'370'
	// <0x59b69d>|0x01d|+0x00b:'371'
	// <0x59b6a8>|0x028|+0x00b:'372'
	// <0>
	// <0x59b6b3>|0x033|+0x00c:'374'
	// <0x59b6bf>|0x03f|+0x00c:'375'
	// <0>
	// <0x59b6cb>|0x04b|+0x034:'377'
	// <0x59b6ff>|0x07f|+0x02a:'378'
	// <0>
	// <0x59b729>|0x0a9|+0x006:'380'
	// <0x59b72f>|0x0af|+0x017:'381'
	// ******
}

// STATE[STUB]
// survarium::booby_trap_set_core const* survarium::booby_trap_core::owner() const
booby_trap_set_core const* booby_trap_core::owner( ) const
{
	return NULL;

	// FUNCTION BODY
	// <0x59b249>|0x009|+0x00c:'386'
	// <0x59b255>|0x015|+0x009:'387'
	// ******
}

// STATE[STUB]
// survarium::booby_trap_set_core* survarium::booby_trap_core::owner()
booby_trap_set_core* booby_trap_core::owner( )
{
	return NULL;

	// FUNCTION BODY
	// <0x59b219>|0x009|+0x00c:'392'
	// <0x59b225>|0x015|+0x009:'393'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::hit(survarium::hit_initiator const* const, const unsigned int, char const*, const float, const float, survarium::bullet* const)
void booby_trap_core::hit(
	hit_initiator const*	initiator,
	u32						bone_index,
	pcstr					damage_type,
	float					amount,
	float					armor_piercing,
	bullet*					bullet
)
{
	// CALL SITE INFO
	// <0x59b203> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x59b1a9>|0x009|+0x045:'412'
	// <0>
	// <0x59b1ee>|0x04e|+0x017:'414'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_core::hit(survarium::hit_initiator const* const, vostok::collision::bone_collision_data const&, char const*, const float, const float, survarium::bullet* const)
void booby_trap_core::hit(
	hit_initiator const*				initiator,
	collision::bone_collision_data const&	bone_data,
	pcstr								damage_type,
	float								amount,
	float								armor_piercing,
	bullet*								bullet
)
{
	// CALL SITE INFO
	// <0x59b193> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x59b12b>|0x00b|+0x053:'433'
	// <0>
	// <0x59b17e>|0x05e|+0x017:'435'
	// ******
}

} // namespace survarium
