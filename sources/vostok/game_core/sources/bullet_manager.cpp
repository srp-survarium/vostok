////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/bullet_manager.h>

#include <vostok/game_core/sources/temp_include_all.h>

namespace survarium {

// STATE[STUB]
// survarium::bullet_manager::bullet_manager(survarium::game_material_manager*, vostok::physics::world*, survarium::bullet_manager_engine*)
bullet_manager::bullet_manager( game_material_manager* material_manager, physics::world* physics_world, bullet_manager_engine* engine ) :
	m_bullets				( NULL, 10 ),
	m_mt_stack_allocator	( NULL, 10 )
{
	IncludeAll all;
	// FUNCTION BODY
	// <0x5a29f3>|0x123|+0x008:'45'
	// <0x5a29fb>|0x12b|+0x008:'46'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::~bullet_manager()
bullet_manager::~bullet_manager( )
{
	// FUNCTION BODY
	// <0x5a2529>|0x009|+0x014:'53'
	// <0x5a253d>|0x01d|+0x00b:'54'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::initialize()
void bullet_manager::initialize( )
{
	// FUNCTION BODY
	// <0x5a28b7>|0x007|+0x00d:'59'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::register_console_commands()
void bullet_manager::register_console_commands( )
{
	// STATICS
	// static console_commands::cc_delegate set_max_bullets_cc = <0x4c2b670>;
	// static console_commands::cc_delegate displace_all_bullets_cc = <0x4c2b6d0>;
	// ******

	// FUNCTION BODY
	// <0x5a275e>|0x00e|+0x085:'73'
	// <0x5a27e3>|0x093|+0x0bf:'74'
	// ******
}

struct redundant_bullet_predicate {
	inline	explicit	redundant_bullet_predicate	( bullet_manager& bullet_manager_ ) : bullet_manager( &bullet_manager_ ) { }

			bool		operator()					( bullet* bullet )
	{
		return false;

		// FUNCTION BODY
		// <0xbe619>|0x009|+0x02e:'86'
		// <0xbe647>|0x037|+0x004:'87'
		// <0>
		// <0xbe64b>|0x03b|+0x00e:'89'
		// <0xbe659>|0x049|+0x002:'90'
		// ******
	}

public:
	/* 0x0000 */	survarium::bullet_manager*		bullet_manager;
}; // struct redundant_bullet_predicate

// STATE[STUB]
// void survarium::bullet_manager::free_bullet(survarium::bullet*)
void bullet_manager::free_bullet( bullet* bullet )
{
	// CALL SITE INFO
	// <0x5a1426> -> bool <unknown>(bullet*)
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5a13f9>|0x009|+0x018:'103'
	// <0x5a1411>|0x021|+0x017:'104'
	// <0>
	// <0x5a1428>|0x038|+0x025:'106'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::tick(unsigned int)
void bullet_manager::tick( u32 current_time_in_ms )
{
	// LOCALS
	// u32 							bullets_count
	// u32 							granularity<1>
	// u32 							start_index<1>
	// u32 							n<1>
	// u32 							i<2>
	// bullet_manager::bullet_functor* functor<2>
	// ******

	// FUNCTION BODY
	// <0x5a1d8f>|0x00f|+0x017:'111'
	// <0x5a1da6>|0x026|+0x00a:'112'
	// <0>
	// <0x5a1db0>|0x030|+0x007|[1]:'114'
	// <0x5a1db7>|0x037|+0x009:'115'
	// <0x5a1dc0>|0x040|+0x00d:'116'
	// <0x5a1dcd>|0x04d|+0x01e|[2]:'117'
	// <0x5a1deb>|0x06b|+0x0a0:'118'
	// <0>
	// <0x5a1e8b>|0x10b|+0x009:'120'
	// <0x5a1e94>|0x114|+0x023:'121'
	// <0>
	// <0x5a1eb7>|0x137|+0x005:'123'
	// <0>
	// <0x5a1ebc>|0x13c|+0x016:'125'
	// <0>
	// <0x5a1ed2>|0x152|+0x011|[2]:'127'
	// <0x5a1ee3>|0x163|+0x00c:'128'
	// <0x5a1eef>|0x16f|+0x008:'129'
	// <0x5a1ef7>|0x177|+0x025:'130'
	// <0x5a1f1c>|0x19c|+0x002:'131'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5a1f1e>|0x19e|+0x06e:'140'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::fire(vostok::math::float3 const&, vostok::math::float3 const&, vostok::resources::resource_ptr<survarium::weapon_ammunition,vostok::resources::unmanaged_intrusive_base> const&, survarium::weapon_core const&, unsigned int, survarium::hit_initiator const* const, survarium::hit_receiver const* const, bool)
void bullet_manager::fire(
	float3 const&						position,
	float3 const&						velocity,
	resources::resource_ptr<weapon_ammunition,resources::unmanaged_intrusive_base> const&	wa,
	weapon_core const&					wc,
	u32									current_time_in_ms,
	hit_initiator const*				initiator,
	hit_receiver const*					ignorable_object,
	bool								tracer
)
{
	// FUNCTION BODY
	// <0x5a1d39>|0x009|+0x00c:'146'
	// <0x5a1d45>|0x015|+0x033:'147'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::add_decal(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&, float, vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&, bool)
void bullet_manager::add_decal(
	resources::resource_ptr<resources::unmanaged_resource,resources::unmanaged_intrusive_base> const&	decal,
	float								size,
	float3 const&						position,
	float3 const&						direction,
	float3 const&						normal,
	bool								is_front_face
)
{
	// LOCALS
	// bullet_manager::bullet_functor* functor<1>
	// ******

	// SKIPPED BLOCKS
	// <0x5a19aa><1>
	// ******

	// FUNCTION BODY
	// <0x5a1990>|0x010|+0x020:'152'
	// <0>
	// <0x5a19b0>|0x030|+0x04d:'154'
	// <0>
	// <0x5a19fd>|0x07d|+0x00f:'156'
	// <0x5a1a0c>|0x08c|+0x019:'157'
	// <0x5a1a25>|0x0a5|+0x019:'158'
	// <0x5a1a3e>|0x0be|+0x019:'159'
	// <0x5a1a57>|0x0d7|+0x00d:'160'
	// <0x5a1a64>|0x0e4|+0x009:'161'
	// <0x5a1a6d>|0x0ed|+0x067:'162'
	// <0x5a1ad4>|0x154|+0x0b6:'163'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::add_decal_impl(survarium::bullet_manager::bullet_functor* const)
void bullet_manager::add_decal_impl( bullet_manager::bullet_functor* functor )
{
	// FUNCTION BODY
	// <0x5a1277>|0x007|+0x036:'178'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::add_decal_impl(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&, float, vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&, bool)
void bullet_manager::add_decal_impl(
	resources::resource_ptr<resources::unmanaged_resource,resources::unmanaged_intrusive_base> const&	decal,
	float								size,
	float3 const&						position,
	float3 const&						direction,
	float3 const&						normal,
	bool								is_front_face
)
{
	// LOCALS
	// float 						depth
	// ******

	// CALL SITE INFO
	// <0x5a1246> -> void <unknown>(resources::resource_ptr<resources::unmanaged_resource,resources::unmanaged_intrusive_base> const&, u32, float, float, float3 const&, float3 const&, float3 const&, bool)
	// ******

	// FUNCTION BODY
	// <0x5a11e9>|0x009|+0x00d:'183'
	// <0>
	// <0x5a11f6>|0x016|+0x052:'185'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5a1248>|0x068|+0x00e:'193'
	// <0x5a1256>|0x076|+0x00a:'194'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::play_sound_impl(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float3 const&)
void bullet_manager::play_sound_impl( resources::resource_ptr<resources::unmanaged_resource,resources::unmanaged_intrusive_base> const& sound, float3 const& position )
{
	// CALL SITE INFO
	// <0x5a11cf> -> void <unknown>(resources::resource_ptr<resources::unmanaged_resource,resources::unmanaged_intrusive_base> const&, float3 const&)
	// ******

	// FUNCTION BODY
	// <0x5a11b7>|0x007|+0x01a:'212'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::play_particle(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&)
void bullet_manager::play_particle(
	resources::resource_ptr<resources::unmanaged_resource,resources::unmanaged_intrusive_base> const&	sound,
	float3 const&						position,
	float3 const&						direction,
	float3 const&						normal
)
{
	// LOCALS
	// bullet_manager::bullet_functor* functor<1>
	// ******

	// FUNCTION BODY
	// <0x5a16f0>|0x010|+0x010:'217'
	// <0>
	// <0x5a1700>|0x020|+0x049|[1]:'219'
	// <0>
	// <0x5a1749>|0x069|+0x00f:'221'
	// <0x5a1758>|0x078|+0x019:'222'
	// <0x5a1771>|0x091|+0x019:'223'
	// <0x5a178a>|0x0aa|+0x00b:'224'
	// <0x5a1795>|0x0b5|+0x019:'225'
	// <0x5a17ae>|0x0ce|+0x00b:'226'
	// <0x5a17b9>|0x0d9|+0x103:'227'
	// <0x5a18bc>|0x1dc|+0x0bc:'228'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::play_particle_impl(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&)
void bullet_manager::play_particle_impl(
	resources::resource_ptr<resources::unmanaged_resource,resources::unmanaged_intrusive_base> const&	particle,
	float3 const&						position,
	float3 const&						direction,
	float3 const&						normal
)
{
	// CALL SITE INFO
	// <0x5a11a8> -> void <unknown>(resources::resource_ptr<resources::unmanaged_resource,resources::unmanaged_intrusive_base> const&, float3 const&, float3 const&, float3 const&)
	// ******

	// FUNCTION BODY
	// <0x5a1187>|0x007|+0x023:'234'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::update_tracer(survarium::bullet*, vostok::math::float3 const&, vostok::math::float3 const&, const float)
void bullet_manager::update_tracer(
	bullet*				bullet,
	float3 const&		position,
	float3 const&		direction,
	float				length
)
{
	// LOCALS
	// bullet_manager::bullet_functor* functor<1>
	// ******

	// SKIPPED BLOCKS
	// <0x5a14ba><1>
	// ******

	// FUNCTION BODY
	// <0x5a14b0>|0x010|+0x010:'239'
	// <0>
	// <0x5a14c0>|0x020|+0x04d:'241'
	// <0>
	// <1>
	// <0x5a150d>|0x06d|+0x019:'244'
	// <0x5a1526>|0x086|+0x019:'245'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5a153f>|0x09f|+0x0da:'251'
	// <0x5a1619>|0x179|+0x0bc:'252'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::update_tracer_impl(const unsigned short, vostok::math::float3 const&, vostok::math::float3 const&, const float)
void bullet_manager::update_tracer_impl(
	u16					tracer_idx,
	float3 const&		position,
	float3 const&		direction,
	float				length
)
{
	// CALL SITE INFO
	// <0x5a116e> -> void <unknown>(const u16, float3 const&, float3 const&, const float)
	// ******

	// FUNCTION BODY
	// <0x5a1147>|0x007|+0x029:'258'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::tick_bullets(unsigned int, unsigned int, unsigned int)
void bullet_manager::tick_bullets( u32 start_index, u32 end_index, u32 current_time_in_ms )
{
	// LOCALS
	// bullet** 					end
	// bullet** 					current
	// ******

	// FUNCTION BODY
	// <0x5a1cd9>|0x009|+0x014:'325'
	// <0x5a1ced>|0x01d|+0x014:'326'
	// <0>
	// <0x5a1d01>|0x031|+0x013:'328'
	// <0x5a1d14>|0x044|+0x010:'329'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::destroy_all_bullets(char const*)
void bullet_manager::destroy_all_bullets( pcstr args )
{
	// FUNCTION BODY
	// <0>
	// <0x5a13a9>|0x009|+0x017:'335'
	// <0x5a13c0>|0x020|+0x01c:'336'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::set_max_bullets(char const*)
void bullet_manager::set_max_bullets( pcstr args )
{
	// LOCALS
	// s32 							new_bullets_count
	// ******

	// FUNCTION BODY
	// <0>
	// <0x5a2719>|0x009|+0x01a:'342'
	// <0x5a2733>|0x023|+0x002:'343'
	// <0>
	// <0x5a2735>|0x025|+0x00c:'345'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::allocate_bullets_memory(unsigned int)
void bullet_manager::allocate_bullets_memory( u32 new_max_bullets_count )
{
	// LOCALS
	// resources::creation_request 	request
	// ******

	// FUNCTION BODY
	// <0x5a259f>|0x00f|+0x00c:'352'
	// <0>
	// <0x5a25ab>|0x01b|+0x01f:'354'
	// <0>
	// <0x5a25ca>|0x03a|+0x017:'356'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5a25e1>|0x051|+0x06b:'363'
	// <0>
	// <0x5a264c>|0x0bc|+0x005:'365'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5a2651>|0x0c1|+0x0ab:'372'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::bullets_memory_allocated(vostok::resources::queries_result&)
void bullet_manager::bullets_memory_allocated( resources::queries_result& queries )
{
	// LOCALS
	// resources::resource_ptr<resources::unmanaged_allocation_resource,resources::unmanaged_intrusive_base> new_bullets_memory_ptr
	// bullet_manager::bullet_functor_mt_allocator new_mt_allocator<1>
	// bool 						is_realocation<1>
	// pbyte 						pointer<1>
	// memory::single_size_buffer_allocator<128,threading::simple_lock> new_bullets_allocator<2>
	// buffer_vector<bullet *> 		new_bullets_list<2>
	// u32 							i<3>
	// bullet* 						old_bullet<4>
	// buffer_vector<bullet *> 		new_bullets_list<2>
	// ******

	// FUNCTION BODY
	// <0>
	// <0x5a1fb0>|0x010|+0x00c:'379'
	// <0x5a1fbc>|0x01c|+0x034:'380'
	// <0>
	// <0x5a1ff0>|0x050|+0x017|[1]:'382'
	// <0>
	// <0x5a2007>|0x067|+0x055:'384'
	// <0x5a205c>|0x0bc|+0x00c:'385'
	// <0>
	// <0x5a2068>|0x0c8|+0x046|[3]:'387'
	// <0x5a20ae>|0x10e|+0x00d:'388'
	// <0>
	// <0x5a20bb>|0x11b|+0x019:'390'
	// <0>
	// <0x5a20d4>|0x134|+0x012:'392'
	// <0x5a20e6>|0x146|+0x032:'393'
	// <0>
	// <0x5a2118>|0x178|+0x021:'395'
	// <0>
	// <0x5a2139>|0x199|+0x025|[4]:'397'
	// <0x5a215e>|0x1be|+0x082:'398'
	// <0x5a21e0>|0x240|+0x043:'399'
	// <0x5a2223>|0x283|+0x005:'400'
	// <0>
	// <0x5a2228>|0x288|+0x030:'402'
	// <0x5a2258>|0x2b8|+0x066:'403'
	// <0x5a22be>|0x31e|+0x035:'404'
	// <0x5a22f3>|0x353|+0x005:'405'
	// <0>
	// <0x5a22f8>|0x358|+0x017|[2]:'407'
	// <0x5a230f>|0x36f|+0x00e:'408'
	// <0>
	// <0x5a231d>|0x37d|+0x09a:'410'
	// <0>
	// <0x5a23b7>|0x417|+0x012:'412'
	// <0x5a23c9>|0x429|+0x032:'413'
	// <0x5a23fb>|0x45b|+0x066:'414'
	// <0x5a2461>|0x4c1|+0x02d:'415'
	// <0>
	// <0x5a248e>|0x4ee|+0x012:'417'
	// <0x5a24a0>|0x500|+0x01c:'418'
	// <0x5a24bc>|0x51c|+0x033:'419'
	// <0x5a24ef>|0x54f|+0x008:'420'
	// <0>
	// <1>
	// <2>
	// <0x5a24f7>|0x557|+0x012:'424'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::emit_bullet(vostok::math::float3 const&, vostok::math::float3 const&, float, vostok::resources::resource_ptr<survarium::weapon_ammunition,vostok::resources::unmanaged_intrusive_base> const&, survarium::weapon_core const&, unsigned int, survarium::hit_initiator const* const, survarium::hit_receiver const* const, bool)
void bullet_manager::emit_bullet(
	float3 const&						position,
	float3 const&						velocity,
	float								air_resistance,
	resources::resource_ptr<weapon_ammunition,resources::unmanaged_intrusive_base> const&	wa,
	weapon_core const&					wc,
	u32									current_time_in_ms,
	hit_initiator const*				initiator,
	hit_receiver const*					ignorable_object,
	bool								tracer
)
{
	// LOCALS
	// bullet* 						new_bullet
	// ******

	// CALL SITE INFO
	// <0x5a1ca2> -> bool <unknown>(bullet*)
	// ******

	// FUNCTION BODY
	// <0x5a1ba9>|0x009|+0x044:'488'
	// <0x5a1bed>|0x04d|+0x008:'489'
	// <0>
	// <0x5a1bf5>|0x055|+0x079:'491'
	// <0x5a1c6e>|0x0ce|+0x00c:'492'
	// <0>
	// <0x5a1c7a>|0x0da|+0x011:'494'
	// <0x5a1c8b>|0x0eb|+0x019:'495'
	// <0>
	// <0x5a1ca4>|0x104|+0x00c:'497'
	// <0x5a1cb0>|0x110|+0x00c:'498'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::destroy_bullet(survarium::bullet** const&)
void bullet_manager::destroy_bullet( bullet**& destroying_bullet_iterator )
{
	// LOCALS
	// bullet* 						destroying_bullet
	// ******

	// FUNCTION BODY
	// <0x5a1339>|0x009|+0x00a:'509'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5a1343>|0x013|+0x01b:'516'
	// <0x5a135e>|0x02e|+0x02e:'517'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::destroy_one_bullet()
void bullet_manager::destroy_one_bullet( )
{
	// FUNCTION BODY
	// <0x5a1469>|0x009|+0x017:'530'
	// <0x5a1480>|0x020|+0x01a:'531'
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
	// ******
}

// STATE[STUB]
// bool survarium::bullet_manager::is_inside_collision_db(vostok::math::float3 const&) const
bool bullet_manager::is_inside_collision_db( float3 const& position ) const
{
	// CALL SITE INFO
	// <0x5a12de> -> math::aabb <unknown>() const
	// ******

	return false;

	// FUNCTION BODY
	// <0x5a12c9>|0x009|+0x054:'557'
	// ******
}

} // namespace survarium
