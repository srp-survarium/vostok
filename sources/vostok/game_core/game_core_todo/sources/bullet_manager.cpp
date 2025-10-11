////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bullet_manager.h"

namespace survarium {

// STATE[STUB]
// survarium::bullet_manager::bullet_manager(survarium::game_material_manager*, vostok::physics::world*, survarium::bullet_manager_engine*)
bullet_manager::bullet_manager( game_material_manager* material_manager, vostok::physics::world* physics_world, bullet_manager_engine* engine )
{
	// FUNCTION BODY
	// <0x5a29f3>|0x000|0x000:'45'
	// <0x5a29fb>|0x008|0x008:'46'
	// 1
	// 2
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::~bullet_manager()
void bullet_manager::~bullet_manager( )
{
	// FUNCTION BODY
	// <0x5a2529>|0x000|0x000:'53'
	// <0x5a253d>|0x014|0x014:'54'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::initialize()
void bullet_manager::initialize( )
{
	// FUNCTION BODY
	// <0x5a28b7>|0x000|0x000:'59'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::register_console_commands()
void bullet_manager::register_console_commands( )
{
	// STATICS
	// static vostok::console_commands::cc_delegate set_max_bullets_cc = <0x4c2b670>;
	// static vostok::console_commands::cc_delegate displace_all_bullets_cc = <0x4c2b6d0>;
	// ******

	// FUNCTION BODY
	// <0x5a275e>|0x000|0x000:'73'
	// <0x5a27e3>|0x085|0x085:'74'
	// ******
}

// STATE[STUB]
// bool survarium::redundant_bullet_predicate::operator()(survarium::bullet*)
bool redundant_bullet_predicate::operator()( bullet* bullet )
{
	return false;
	// FUNCTION BODY
	// <0xbe619>|0x000|0x000:'86'
	// <0xbe647>|0x02e|0x02e:'87'
	// 1
	// <0xbe64b>|0x032|0x004:'89'
	// <0xbe659>|0x040|0x00e:'90'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::free_bullet(survarium::bullet*)
void bullet_manager::free_bullet( bullet* bullet )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x590426 }, type_index: TypeIndex(0xaf1e) })
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x5a13f9>|0x000|0x000:'103'
	// <0x5a1411>|0x018|0x018:'104'
	// 1
	// <0x5a1428>|0x02f|0x017:'106'
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
	// <0x5a1d8f>|0x000|0x000:'111'
	// <0x5a1da6>|0x017|0x017:'112'
	// 1
	// <0x5a1db0>|0x021|0x00a|[1]:'114'
	// <0x5a1db7>|0x028|0x007:'115'
	// <0x5a1dc0>|0x031|0x009:'116'
	// <0x5a1dcd>|0x03e|0x00d|[2]:'117'
	// <0x5a1deb>|0x05c|0x01e:'118'
	// 1
	// <0x5a1e8b>|0x0fc|0x0a0:'120'
	// <0x5a1e94>|0x105|0x009:'121'
	// 1
	// <0x5a1eb7>|0x128|0x023:'123'
	// 1
	// <0x5a1ebc>|0x12d|0x005:'125'
	// 1
	// <0x5a1ed2>|0x143|0x016|[2]:'127'
	// <0x5a1ee3>|0x154|0x011:'128'
	// <0x5a1eef>|0x160|0x00c:'129'
	// <0x5a1ef7>|0x168|0x008:'130'
	// <0x5a1f1c>|0x18d|0x025:'131'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x5a1f1e>|0x18f|0x002:'140'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::fire(vostok::math::float3 const&, vostok::math::float3 const&, vostok::resources::resource_ptr<survarium::weapon_ammunition,vostok::resources::unmanaged_intrusive_base> const&, survarium::weapon_core const&, unsigned int, survarium::hit_initiator const* const, survarium::hit_receiver const* const, bool)
void bullet_manager::fire(
	vostok::math::float3 const&			position,
	vostok::math::float3 const&			velocity,
	vostok::resources::resource_ptr<weapon_ammunition,vostok::resources::unmanaged_intrusive_base> const&	wa,
	weapon_core const&					wc,
	u32									current_time_in_ms,
	hit_initiator const*				initiator,
	hit_receiver const*					ignorable_object,
	bool								tracer)
{
	// FUNCTION BODY
	// <0x5a1d39>|0x000|0x000:'146'
	// <0x5a1d45>|0x00c|0x00c:'147'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::add_decal(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&, float, vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&, bool)
void bullet_manager::add_decal(
	vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&	decal,
	float								size,
	vostok::math::float3 const&			position,
	vostok::math::float3 const&			direction,
	vostok::math::float3 const&			normal,
	bool								is_front_face)
{
	// LOCALS
	// bullet_manager::bullet_functor* functor<1>
	// ******

	// SKIPPED BLOCKS
	// <0x5a19aa><1>
	// ******

	// FUNCTION BODY
	// <0x5a1990>|0x000|0x000:'152'
	// 1
	// <0x5a19b0>|0x020|0x020:'154'
	// 1
	// <0x5a19fd>|0x06d|0x04d:'156'
	// <0x5a1a0c>|0x07c|0x00f:'157'
	// <0x5a1a25>|0x095|0x019:'158'
	// <0x5a1a3e>|0x0ae|0x019:'159'
	// <0x5a1a57>|0x0c7|0x019:'160'
	// <0x5a1a64>|0x0d4|0x00d:'161'
	// <0x5a1a6d>|0x0dd|0x009:'162'
	// <0x5a1ad4>|0x144|0x067:'163'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::add_decal_impl(survarium::bullet_manager::bullet_functor* const)
void bullet_manager::add_decal_impl( bullet_manager::bullet_functor* functor )
{
	// FUNCTION BODY
	// <0x5a1277>|0x000|0x000:'178'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::add_decal_impl(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&, float, vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&, bool)
void bullet_manager::add_decal_impl(
	vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&	decal,
	float								size,
	vostok::math::float3 const&			position,
	vostok::math::float3 const&			direction,
	vostok::math::float3 const&			normal,
	bool								is_front_face)
{
	// LOCALS
	// float 						depth
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x590246 }, type_index: TypeIndex(0xaf1b) })
	// ******

	// FUNCTION BODY
	// <0x5a11e9>|0x000|0x000:'183'
	// 1
	// <0x5a11f6>|0x00d|0x00d:'185'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x5a1248>|0x05f|0x052:'193'
	// <0x5a1256>|0x06d|0x00e:'194'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::play_sound_impl(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float3 const&)
void bullet_manager::play_sound_impl( vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const& sound, vostok::math::float3 const& position )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5901cf }, type_index: TypeIndex(0xaf1a) })
	// ******

	// FUNCTION BODY
	// <0x5a11b7>|0x000|0x000:'212'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::play_particle(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&)
void bullet_manager::play_particle(
	vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&	sound,
	vostok::math::float3 const&			position,
	vostok::math::float3 const&			direction,
	vostok::math::float3 const&			normal)
{
	// LOCALS
	// bullet_manager::bullet_functor* functor<1>
	// ******

	// FUNCTION BODY
	// <0x5a16f0>|0x000|0x000:'217'
	// 1
	// <0x5a1700>|0x010|0x010|[1]:'219'
	// 1
	// <0x5a1749>|0x059|0x049:'221'
	// <0x5a1758>|0x068|0x00f:'222'
	// <0x5a1771>|0x081|0x019:'223'
	// <0x5a178a>|0x09a|0x019:'224'
	// <0x5a1795>|0x0a5|0x00b:'225'
	// <0x5a17ae>|0x0be|0x019:'226'
	// <0x5a17b9>|0x0c9|0x00b:'227'
	// <0x5a18bc>|0x1cc|0x103:'228'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::play_particle_impl(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&)
void bullet_manager::play_particle_impl(
	vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const&	particle,
	vostok::math::float3 const&			position,
	vostok::math::float3 const&			direction,
	vostok::math::float3 const&			normal)
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5901a8 }, type_index: TypeIndex(0xaf1c) })
	// ******

	// FUNCTION BODY
	// <0x5a1187>|0x000|0x000:'234'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::update_tracer(survarium::bullet*, vostok::math::float3 const&, vostok::math::float3 const&, const float)
void bullet_manager::update_tracer(
	bullet*							bullet,
	vostok::math::float3 const&		position,
	vostok::math::float3 const&		direction,
	float							length)
{
	// LOCALS
	// bullet_manager::bullet_functor* functor<1>
	// ******

	// SKIPPED BLOCKS
	// <0x5a14ba><1>
	// ******

	// FUNCTION BODY
	// <0x5a14b0>|0x000|0x000:'239'
	// 1
	// <0x5a14c0>|0x010|0x010:'241'
	// 1
	// 2
	// <0x5a150d>|0x05d|0x04d:'244'
	// <0x5a1526>|0x076|0x019:'245'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x5a153f>|0x08f|0x019:'251'
	// <0x5a1619>|0x169|0x0da:'252'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::update_tracer_impl(const unsigned short, vostok::math::float3 const&, vostok::math::float3 const&, const float)
void bullet_manager::update_tracer_impl(
	u16								tracer_idx,
	vostok::math::float3 const&		position,
	vostok::math::float3 const&		direction,
	float							length)
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59016e }, type_index: TypeIndex(0xaf1f) })
	// ******

	// FUNCTION BODY
	// <0x5a1147>|0x000|0x000:'258'
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
	// <0x5a1cd9>|0x000|0x000:'325'
	// <0x5a1ced>|0x014|0x014:'326'
	// 1
	// <0x5a1d01>|0x028|0x014:'328'
	// <0x5a1d14>|0x03b|0x013:'329'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::destroy_all_bullets(char const*)
void bullet_manager::destroy_all_bullets( pcstr args )
{
	// FUNCTION BODY
	// 1
	// <0x5a13a9>|0x000|0x000:'335'
	// <0x5a13c0>|0x017|0x017:'336'
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
	// 1
	// <0x5a2719>|0x000|0x000:'342'
	// <0x5a2733>|0x01a|0x01a:'343'
	// 1
	// <0x5a2735>|0x01c|0x002:'345'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::allocate_bullets_memory(unsigned int)
void bullet_manager::allocate_bullets_memory( u32 new_max_bullets_count )
{
	// LOCALS
	// vostok::resources::creation_request request
	// ******

	// FUNCTION BODY
	// <0x5a259f>|0x000|0x000:'352'
	// 1
	// <0x5a25ab>|0x00c|0x00c:'354'
	// 1
	// <0x5a25ca>|0x02b|0x01f:'356'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x5a25e1>|0x042|0x017:'363'
	// 1
	// <0x5a264c>|0x0ad|0x06b:'365'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x5a2651>|0x0b2|0x005:'372'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::bullets_memory_allocated(vostok::resources::queries_result&)
void bullet_manager::bullets_memory_allocated( vostok::resources::queries_result& queries )
{
	// LOCALS
	// vostok::resources::resource_ptr<vostok::resources::unmanaged_allocation_resource,vostok::resources::unmanaged_intrusive_base> new_bullets_memory_ptr
	// bullet_manager::bullet_functor_mt_allocator new_mt_allocator<1>
	// bool 						is_realocation<1>
	// u8* 							pointer<1>
	// vostok::memory::single_size_buffer_allocator<128,vostok::threading::simple_lock> new_bullets_allocator<2>
	// vostok::buffer_vector<bullet *> new_bullets_list<2>
	// u32 							i<3>
	// bullet* 						old_bullet<4>
	// vostok::buffer_vector<bullet *> new_bullets_list<2>
	// ******

	// FUNCTION BODY
	// 1
	// <0x5a1fb0>|0x000|0x000:'379'
	// <0x5a1fbc>|0x00c|0x00c:'380'
	// 1
	// <0x5a1ff0>|0x040|0x034|[1]:'382'
	// 1
	// <0x5a2007>|0x057|0x017:'384'
	// <0x5a205c>|0x0ac|0x055:'385'
	// 1
	// <0x5a2068>|0x0b8|0x00c|[3]:'387'
	// <0x5a20ae>|0x0fe|0x046:'388'
	// 1
	// <0x5a20bb>|0x10b|0x00d:'390'
	// 1
	// <0x5a20d4>|0x124|0x019:'392'
	// <0x5a20e6>|0x136|0x012:'393'
	// 1
	// <0x5a2118>|0x168|0x032:'395'
	// 1
	// <0x5a2139>|0x189|0x021|[4]:'397'
	// <0x5a215e>|0x1ae|0x025:'398'
	// <0x5a21e0>|0x230|0x082:'399'
	// <0x5a2223>|0x273|0x043:'400'
	// 1
	// <0x5a2228>|0x278|0x005:'402'
	// <0x5a2258>|0x2a8|0x030:'403'
	// <0x5a22be>|0x30e|0x066:'404'
	// <0x5a22f3>|0x343|0x035:'405'
	// 1
	// <0x5a22f8>|0x348|0x005|[2]:'407'
	// <0x5a230f>|0x35f|0x017:'408'
	// 1
	// <0x5a231d>|0x36d|0x00e:'410'
	// 1
	// <0x5a23b7>|0x407|0x09a:'412'
	// <0x5a23c9>|0x419|0x012:'413'
	// <0x5a23fb>|0x44b|0x032:'414'
	// <0x5a2461>|0x4b1|0x066:'415'
	// 1
	// <0x5a248e>|0x4de|0x02d:'417'
	// <0x5a24a0>|0x4f0|0x012:'418'
	// <0x5a24bc>|0x50c|0x01c:'419'
	// <0x5a24ef>|0x53f|0x033:'420'
	// 1
	// 2
	// 3
	// <0x5a24f7>|0x547|0x008:'424'
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::emit_bullet(vostok::math::float3 const&, vostok::math::float3 const&, float, vostok::resources::resource_ptr<survarium::weapon_ammunition,vostok::resources::unmanaged_intrusive_base> const&, survarium::weapon_core const&, unsigned int, survarium::hit_initiator const* const, survarium::hit_receiver const* const, bool)
void bullet_manager::emit_bullet(
	vostok::math::float3 const&			position,
	vostok::math::float3 const&			velocity,
	float								air_resistance,
	vostok::resources::resource_ptr<weapon_ammunition,vostok::resources::unmanaged_intrusive_base> const&	wa,
	weapon_core const&					wc,
	u32									current_time_in_ms,
	hit_initiator const*				initiator,
	hit_receiver const*					ignorable_object,
	bool								tracer)
{
	// LOCALS
	// bullet* 						new_bullet
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x590ca2 }, type_index: TypeIndex(0xaf1e) })
	// ******

	// FUNCTION BODY
	// <0x5a1ba9>|0x000|0x000:'488'
	// <0x5a1bed>|0x044|0x044:'489'
	// 1
	// <0x5a1bf5>|0x04c|0x008:'491'
	// <0x5a1c6e>|0x0c5|0x079:'492'
	// 1
	// <0x5a1c7a>|0x0d1|0x00c:'494'
	// <0x5a1c8b>|0x0e2|0x011:'495'
	// 1
	// <0x5a1ca4>|0x0fb|0x019:'497'
	// <0x5a1cb0>|0x107|0x00c:'498'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
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
	// <0x5a1339>|0x000|0x000:'509'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x5a1343>|0x00a|0x00a:'516'
	// <0x5a135e>|0x025|0x01b:'517'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// ******
}

// STATE[STUB]
// void survarium::bullet_manager::destroy_one_bullet()
void bullet_manager::destroy_one_bullet( )
{
	// FUNCTION BODY
	// <0x5a1469>|0x000|0x000:'530'
	// <0x5a1480>|0x017|0x017:'531'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// 15
	// 16
	// 17
	// 18
	// 19
	// 20
	// 21
	// ******
}

// STATE[STUB]
// bool survarium::bullet_manager::is_inside_collision_db(vostok::math::float3 const&) const
bool bullet_manager::is_inside_collision_db( vostok::math::float3 const& position ) const
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5902de }, type_index: TypeIndex(0x9477) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x5a12c9>|0x000|0x000:'557'
	// ******
}

	/* TYPEDEFS

	typedef
		long
		counter_type;

	typedef
		vostok::memory::multi_threading_single_size_allocator_policy<vostok::memory::single_size_buffer_allocator<128,vostok::threading::simple_lock>::node>::free_list_type
		free_list_type;

} // namespace survarium
