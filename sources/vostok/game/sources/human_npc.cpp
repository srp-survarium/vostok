////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "human_npc.h"
#include "game_world.h"				// base game_object_ + ref members source off game_world
#include "animated_model_instance.h"	// resource_ptr member dtor needs complete type
#include "animation_space_graph.h"		// resource_ptr member dtor needs complete type

namespace survarium {

// TU-local (canonical headers/hit_object.h; owner mapping in
// temp/triage_log.md) - the on_hit_event parameter type
struct hit_object {
	inline		hit_object	( ) { /* no source */ }

public:
	/* 0x0000 */	ai::game_object*	m_source;
	/* 0x0004 */	float3				m_position;
	/* 0x0010 */	u16					m_target_bone;
	/* 0x0014 */	float				m_power;
}; // struct hit_object

STATIC_SIZE_ASSERT(hit_object, 0x18);

// TU statics (compiler-generated dynamic initializers / atexit
// destructors); a matcher recovers their types/initializers from the asm.
/*
// STATE[STUB]
void `dynamic atexit destructor for 's_npc_debug_draw_command''( )
{
	// FUNCTION BODY[0x7f0a40]
	// <0x7d9070>|0x000|      :'35'	{
	// ******
}
*/

// STATE[STUB]
 human_npc::npc_game_attributes::npc_game_attributes( )
{
	// FUNCTION BODY[0x5be6d0]: 0
	// <0x5be6d0>|0x000|+0x11f:'52'	{
	// <0x5be7ef>|0x11f|      :'53'	}
	// ******
}

// STATE[STUB]
human_npc::npc_game_attributes& human_npc::npc_game_attributes::operator=( human_npc::npc_game_attributes& other )
{
	return *this;

	// FUNCTION BODY[0x5bef40]: 18
	// <0x5bef4a>|0x00a|+0x008:'57'
	// <0>
	// <0x5bef52>|0x012|+0x010:'59'
	// <0x5bef62>|0x022|+0x010:'60'
	// <0x5bef72>|0x032|+0x010:'61'
	// <0x5bef82>|0x042|+0x006:'62'
	// <0x5bef88>|0x048|+0x030:'63'
	// <0x5befb8>|0x078|+0x036:'64'
	// <0x5befee>|0x0ae|+0x00c:'65'
	// <0x5beffa>|0x0ba|+0x00c:'66'
	// <0x5bf006>|0x0c6|+0x00c:'67'
	// <0x5bf012>|0x0d2|+0x00c:'68'
	// <0x5bf01e>|0x0de|+0x00c:'69'
	// <0x5bf02a>|0x0ea|+0x006:'70'
	// <0x5bf030>|0x0f0|+0x00d:'71'
	// <0>
	// <1>
	// <0x5bf03d>|0x0fd|+0x002:'74'
	// ******
}

// STATE[STUB]
// init-list sources off the owning game_world (its base_game_scene supplies the
// physics/renderer) - buildability shapes; a matcher confirms the real sources.
 human_npc::human_npc( game_world& game_world ) :
	game_object_( game_world ),								// base needs base_game_scene&
	m_ai_world( game_world.get_ai_world( ) ),
	m_sound_world( game_world.get_sound_world( ) ),
	m_physics_world( *game_world.get_physics_world( ) ),
	m_game_world( game_world ),
	m_renderer( game_world.renderer( ) ),
	m_visibility_parameters( 0.0f )							// buildability: matcher supplies real value
{
	// CALL SITE INFO
	// <0x5c0372> -> < unknown >
	// ******

	// FUNCTION BODY[0x5c0160]: 0
	// <0x5c0160>|0x000|+0x240:'101'	{
	// <0x5c03a0>|0x240|      :'102'	}
	// ******
}

// STATE[STUB]
 human_npc::~human_npc( )
{
	// CALL SITE INFO
	// <0x5bffde> -> < unknown >
	// ******

	// FUNCTION BODY[0x5bff20]: 1
	// <0x5bff25>|0x005|+0x063:'106'
	// ******
}

// STATE[STUB]
void human_npc::clear_resources( )
{
	// CALL SITE INFO
	// <0x5c0703> -> sound::world_user& < unknown >() const
	// <0x5c0750> -> void < unknown >( physics::bt_rigid_body_base* )
	// <0x5c0761> -> void < unknown >( ai::game_object const& ) const
	// <0x5c0783> -> void < unknown >( resources::unmanaged_resource_ptr )
	// ******

	// FUNCTION BODY[0x5c06d0]: 9
	// <0x5c06d3>|0x003|+0x01a:'111'
	// <0>
	// <0x5c06ed>|0x01d|+0x01f:'113'
	// <0>
	// <0x5c070c>|0x03c|+0x029:'115'
	// <0x5c0735>|0x065|+0x01d:'116'
	// <0>
	// <0x5c0752>|0x082|+0x011:'118'
	// <0x5c0763>|0x093|+0x022:'119'
	// ******
}

// STATE[STUB]
void human_npc::set_brain_unit( resources::unmanaged_resource_ptr const& brain_unit )
{
	// FUNCTION BODY[0x5bed10]: 2
	// <0>
	// <0x5bed10>|0x000|+0x00c:'125'
	// ******
}

// STATE[STUB]
void human_npc::set_animation_space_graph( animation_space_graph_ptr const& space_graph )
{
	// FUNCTION BODY[0x5bfed0]: 2
	// <0>
	// <0x5bfed0>|0x000|+0x043:'131'
	// ******
}

// STATE[STUB]
void human_npc::set_model( animated_model_instance_ptr const& model )
{
	// FUNCTION BODY[0x5bfe50]: 4
	// <0>
	// <0x5bfe50>|0x000|+0x041:'137'
	// <0x5bfe91>|0x041|+0x01d:'138'
	// <0x5bfeae>|0x05e|+0x01a:'139'
	// ******
}

// STATE[STUB]
void human_npc::set_default_animation( resources::managed_resource_ptr const& default_animation )
{
	// FUNCTION BODY[0x5bfe20]: 1
	// <0x5bfe21>|0x001|+0x02c:'144'
	// ******
}

// STATE[STUB]
void human_npc::enable( )
{
	// CALL SITE INFO
	// <0x5c0c5b> -> void < unknown >( resources::unmanaged_resource_ptr )
	// <0x5c0c73> -> sound::world_user& < unknown >() const
	// <0x5c0cc2> -> void < unknown >( physics::bt_rigid_body_base*, u16, u16 )
	// <0x5c0d8f> -> void < unknown >( resources::unmanaged_resource_ptr )
	// ******

	// FUNCTION BODY[0x5c0c30]: 23
	// <0>
	// <0x5c0c33>|0x003|+0x02a:'150'
	// <0>
	// <0x5c0c5d>|0x02d|+0x01f:'152'
	// <0x5c0c7c>|0x04c|+0x024:'153'
	// <0>
	// <1>
	// <2>
	// <0x5c0ca0>|0x070|+0x024:'157'
	// <0x5c0cc4>|0x094|+0x02f:'158'
	// <0x5c0cf3>|0x0c3|+0x019:'159'
	// <0>
	// <0x5c0d0c>|0x0dc|+0x00e:'161'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5c0d1a>|0x0ea|+0x055:'170'
	// <0x5c0d6f>|0x13f|+0x022:'171'
	// ******
}

// STATE[STUB]
void human_npc::on_sound_event( sound::sound_producer const& sound_source )
{
	// LOCALS
	// ai::sensed_sound_object 			perceived_sound
	// ******

	// CALL SITE INFO
	// <0x5be5fa> -> ai::game_object const* < unknown >() const
	// <0x5be626> -> float3 < unknown >( float3 const& ) const
	// <0x5be65b> -> void < unknown >( ai::npc&, ai::sensed_sound_object const& ) const
	// ******

	// FUNCTION BODY[0x5be5a0]: 10
	// <0>
	// <1>
	// <0x5be5a9>|0x009|+0x006:'178'
	// <0x5be5af>|0x00f|+0x046:'179'
	// <0x5be5f5>|0x055|+0x007:'180'
	// <0x5be5fc>|0x05c|+0x030:'181'
	// <0x5be62c>|0x08c|+0x00c:'182'
	// <0x5be638>|0x098|+0x007:'183'
	// <0>
	// <0x5be63f>|0x09f|+0x01e:'185'
	// ******
}

// STATE[STUB]
void human_npc::on_hit_event( hit_object const& hit_source )
{
	// LOCALS
	// ai::sensed_hit_object 			perceived_hit
	// ******

	// CALL SITE INFO
	// <0x5be53f> -> float3 < unknown >( float3 const& ) const
	// <0x5be58e> -> void < unknown >( ai::npc&, ai::sensed_hit_object const& ) const
	// ******

	// FUNCTION BODY[0x5be4c0]: 9
	// <0x5be4c3>|0x003|+0x012:'190'
	// <0x5be4d5>|0x015|+0x070:'191'
	// <0x5be545>|0x085|+0x002:'192'
	// <0x5be547>|0x087|+0x014:'193'
	// <0x5be55b>|0x09b|+0x008:'194'
	// <0>
	// <1>
	// <0x5be563>|0x0a3|+0x02e:'197'
	// <0x5be591>|0x0d1|+0x009:'198'
	// ******
}

// STATE[STUB]
math::aabb human_npc::get_aabb( ) const
{
	return vostok::math::create_zero_aabb();	// aabb default ctor is private; use the friend factory

	// FUNCTION BODY[0x5be6b0]: 1
	// <0x5be6b0>|0x000|+0x019:'203'
	// ******
}

// STATE[STUB]
float3 human_npc::get_random_surface_point( const u32 current_time ) const
{
	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0x5be9d0]: 1
	// <0x5be9d0>|0x000|+0x020:'208'
	// ******
}

// STATE[STUB]
float3 human_npc::get_position( float3 const& requester ) const
{
	// CALL SITE INFO
	// <0x5be49b> -> float4x4 < unknown >( float3 const& ) const
	// ******

	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0x5be480]: 1
	// <0x5be486>|0x006|+0x02e:'213'
	// ******
}

// STATE[STUB]
float3 human_npc::get_position( ) const
{
	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0x5be460]: 1
	// <0x5be460>|0x000|+0x015:'218'
	// ******
}

// STATE[STUB]
float4x4 human_npc::get_eyes_matrix( ) const
{
	// CALL SITE INFO
	// <0x5bece0> -> float3 < unknown >() const
	// <0x5becf0> -> float3 < unknown >() const
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY[0x5becb0]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5becb3>|0x003|+0x054:'227'
	// ******
}

// STATE[STUB]
float3 human_npc::get_eyes_direction( ) const
{
	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0x5bee00]: 1
	// <0x5bee06>|0x006|+0x122:'232'
	// ******
}

// STATE[STUB]
float3 human_npc::get_eyes_position( ) const
{
	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0x5bed20]: 1
	// <0x5bed27>|0x007|+0x0d3:'237'
	// ******
}

// STATE[STUB]
float4x4 human_npc::local_to_cell( float3 const& requester ) const
{
	return vostok::math::float4x4();

	// FUNCTION BODY[0x5be440]: 2
	// <0>
	// <0x5be440>|0x000|+0x017:'243'
	// ******
}

// STATE[STUB]
void human_npc::draw_damage_model( render::game::renderer& render, render::scene_ptr const& scene ) const
{
	// FUNCTION BODY[0x5be970]: 12
	// <0>
	// <1>
	// <0x5be970>|0x000|+0x024:'250'
	// <0x5be994>|0x024|+0x00c:'251'
	// <0>
	// <0x5be9a0>|0x030|+0x00c:'253'
	// <0x5be9ac>|0x03c|+0x01c:'254'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
void human_npc::draw( render::game::renderer& render, render::scene_ptr const& scene ) const
{
	// CALL SITE INFO
	// <0x5c0404> -> float3 < unknown >() const
	// <0x5c0413> -> float3 < unknown >() const
	// <0x5c04b5> -> float3 < unknown >() const
	// ******

	// FUNCTION BODY[0x5c03c0]: 27
	// <0x5c03cb>|0x00b|+0x007:'264'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5c03d2>|0x012|+0x025:'273'
	// <0>
	// <0x5c03f7>|0x037|+0x00f:'275'
	// <0x5c0406>|0x046|+0x025:'276'
	// <0x5c042b>|0x06b|+0x067:'277'
	// <0>
	// <0x5c0492>|0x0d2|+0x009:'279'
	// <0>
	// <0x5c049b>|0x0db|+0x037:'281'
	// <0x5c04d2>|0x112|+0x007:'282'
	// <0>
	// <0x5c04d9>|0x119|+0x009:'284'
	// <0>
	// <0x5c04e2>|0x122|+0x022:'286'
	// <0x5c0504>|0x144|+0x007:'287'
	// <0>
	// <1>
	// <0x5c050b>|0x14b|+0x024:'290'
	// ******
}

// STATE[STUB]
void human_npc::set_transform( float4x4 const& transform )
{
	// FUNCTION BODY[0x5c0650]: 5
	// <0x5c0658>|0x008|+0x008:'305'
	// <0x5c0660>|0x010|+0x010:'306'
	// <0>
	// <0x5c0670>|0x020|+0x03b:'308'
	// <0x5c06ab>|0x05b|+0x019:'309'
	// ******
}

// STATE[STUB]
void human_npc::tick( const u32 current_time_in_ms, const bool is_game_paused )
{
	// CALL SITE INFO
	// <0x5c0be2> -> void < unknown >( physics::bt_rigid_body_base*, float4x4 const& )
	// <0x5c0c15> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x5c0ba0]: 14
	// <0x5c0ba7>|0x007|+0x013:'314'
	// <0>
	// <0x5c0bba>|0x01a|+0x006:'316'
	// <0>
	// <0x5c0bc0>|0x020|+0x024:'318'
	// <0x5c0be4>|0x044|+0x00f:'319'
	// <0x5c0bf3>|0x053|+0x013:'320'
	// <0x5c0c06>|0x066|+0x008:'321'
	// <0>
	// <1>
	// <0x5c0c0e>|0x06e|+0x00d:'324'
	// <0x5c0c1b>|0x07b|+0x006:'325'
	// <0>
	// <0x5c0c21>|0x081|+0x006:'327'
	// ******
}

// STATE[STUB]
void human_npc::render_model( )
{
	// LOCALS
	// animation::animation_player* 	animation_player
	// ******

	// FUNCTION BODY[0x5c0540]: 17
	// <0x5c0546>|0x006|+0x00c:'332'
	// <0x5c0552>|0x012|+0x029:'333'
	// <0>
	// <0x5c057b>|0x03b|+0x027:'335'
	// <0x5c05a2>|0x062|+0x00a:'336'
	// <0x5c05ac>|0x06c|+0x019:'337'
	// <0>
	// <0x5c05c5>|0x085|+0x02f:'339'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c05f4>|0x0b4|+0x026:'344'
	// <0>
	// <1>
	// <2>
	// <0x5c061a>|0x0da|+0x013:'348'
	// ******
}

// STATE[STUB]
object_weapon* human_npc::pop_weapon( )
{
	return NULL;

	// FUNCTION BODY[0x5bef30]: 1
	// <0x5bef31>|0x001|+0x00d:'364'
	// ******
}

// STATE[STUB]
bool human_npc::is_safe( ) const
{
	// CALL SITE INFO
	// <0x5be963> -> bool < unknown >( resources::unmanaged_resource_ptr ) const
	// ******

	return false;

	// FUNCTION BODY[0x5be940]: 1
	// <0x5be943>|0x003|+0x022:'369'
	// ******
}

// STATE[STUB]
bool human_npc::is_target_in_melee_range( ai::npc const* const target ) const
{
	// CALL SITE INFO
	// <0x5be8ca> -> float3 < unknown >( float3 const& ) const
	// ******

	return false;

	// FUNCTION BODY[0x5be890]: 2
	// <0x5be890>|0x000|+0x003:'373'	{
	// <0>
	// <0x5be893>|0x003|+0x09a:'375'
	// <0x5be92d>|0x09d|-0x006:'375'
	// <0x5be927>|0x097|+0x008:'376'
	// <0x5be92f>|0x09f|      :'376'	}
	// ******
}

// STATE[STUB]
bool human_npc::is_at_node( ai::game_object const* const node ) const
{
	return false;

	// FUNCTION BODY[0x5be430]: 4
	// <0>
	// <1>
	// <2>
	// <0x5be430>|0x000|+0x002:'383'
	// ******
}

// STATE[STUB]
void human_npc::prepare_to_attack( ai::npc const* const target, ai::weapon const* const gun )
{
	// CALL SITE INFO
	// <0x5bfd64> -> ai::game_object const* < unknown >() const
	// <0x5bfd70> -> ai::game_object const* < unknown >() const
	// <0x5bfd7d> -> pcstr < unknown >() const
	// <0x5bfd86> -> pcstr < unknown >() const
	// <0x5bfd8f> -> pcstr < unknown >() const
	// ******

	// FUNCTION BODY[0x5bfce0]: 3
	// <0x5bfce9>|0x009|+0x10e:'388'
	// <0x5bfdf7>|0x117|+0x003:'389'
	// <0x5bfdfa>|0x11a|+0x003:'390'
	// ******
}

// STATE[STUB]
void human_npc::attack( ai::npc const* const target, ai::weapon const* const gun )
{
	// CALL SITE INFO
	// <0x5bfc41> -> ai::game_object const* < unknown >() const
	// <0x5bfc50> -> ai::game_object const* < unknown >() const
	// <0x5bfc5d> -> pcstr < unknown >() const
	// <0x5bfc67> -> pcstr < unknown >() const
	// <0x5bfc70> -> pcstr < unknown >() const
	// ******

	// FUNCTION BODY[0x5bfbc0]: 2
	// <0>
	// <0x5bfbc9>|0x009|+0x107:'396'
	// ******
}

// STATE[STUB]
void human_npc::attack_melee( ai::npc const* const target, ai::weapon const* const gun )
{
	// CALL SITE INFO
	// <0x5bfb21> -> ai::game_object const* < unknown >() const
	// <0x5bfb30> -> ai::game_object const* < unknown >() const
	// <0x5bfb3d> -> pcstr < unknown >() const
	// <0x5bfb47> -> pcstr < unknown >() const
	// <0x5bfb50> -> pcstr < unknown >() const
	// ******

	// FUNCTION BODY[0x5bfaa0]: 2
	// <0>
	// <0x5bfaa9>|0x009|+0x107:'402'
	// ******
}

// STATE[STUB]
void human_npc::attack_from_cover( ai::npc const* const target, ai::weapon const* const gun )
{
	// CALL SITE INFO
	// <0x5bfa01> -> ai::game_object const* < unknown >() const
	// <0x5bfa10> -> ai::game_object const* < unknown >() const
	// <0x5bfa1d> -> pcstr < unknown >() const
	// <0x5bfa27> -> pcstr < unknown >() const
	// <0x5bfa30> -> pcstr < unknown >() const
	// ******

	// FUNCTION BODY[0x5bf980]: 2
	// <0>
	// <0x5bf989>|0x009|+0x107:'408'
	// ******
}

// STATE[STUB]
void human_npc::stop_attack( ai::npc const* const target, ai::weapon const* const gun )
{
	// CALL SITE INFO
	// <0x5bf901> -> pcstr < unknown >() const
	// ******

	// FUNCTION BODY[0x5bf880]: 4
	// <0>
	// <0x5bf889>|0x009|+0x0d8:'414'
	// <0x5bf961>|0x0e1|+0x006:'415'
	// <0x5bf967>|0x0e7|+0x006:'416'
	// ******
}

// STATE[STUB]
void human_npc::survey_area( )
{
	// CALL SITE INFO
	// <0x5bf80e> -> pcstr < unknown >() const
	// ******

	// FUNCTION BODY[0x5bf790]: 2
	// <0x5bf799>|0x009|+0x0d5:'421'
	// <0>
	// ******
}

// STATE[STUB]
void human_npc::stop_patrolling( )
{
	// CALL SITE INFO
	// <0x5bf71e> -> pcstr < unknown >() const
	// ******

	// FUNCTION BODY[0x5bf6a0]: 2
	// <0x5bf6a9>|0x009|+0x0d5:'427'
	// <0>
	// ******
}

// STATE[STUB]
void human_npc::reload( ai::weapon const* const gun )
{
	// CALL SITE INFO
	// <0x5bf624> -> ai::game_object const* < unknown >() const
	// <0x5bf62f> -> pcstr < unknown >() const
	// <0x5bf638> -> pcstr < unknown >() const
	// ******

	// FUNCTION BODY[0x5bf5a0]: 1
	// <0x5bf5a9>|0x009|+0x0ef:'433'
	// ******
}

// STATE[STUB]
void human_npc::fill_stats( ai::npc_statistics& stats ) const
{
	// LOCALS
	// fixed_string< 64 > 				new_item_content
	// ******

	// TYPEDEFS
	// typedef
	// 	fixed_string< 64 >
	// 	content_type;

	// ******

	// CALL SITE INFO
	// <0x5bea73> -> pcstr < unknown >() const
	// <0x5bebb7> -> float3 < unknown >() const
	// <0x5bebc7> -> float3 < unknown >() const
	// <0x5bebe5> -> float3 < unknown >() const
	// <0x5bec9c> -> void < unknown >( ai::npc_statistics&, resources::unmanaged_resource_ptr ) const
	// ******

	// FUNCTION BODY[0x5bea00]: 17
	// <0x5bea0e>|0x00e|+0x021:'438'
	// <0>
	// <1>
	// <0x5bea2f>|0x02f|+0x03a:'441'
	// <0x5bea69>|0x069|+0x03b:'442'
	// <0x5beaa4>|0x0a4|+0x03b:'443'
	// <0>
	// <0x5beadf>|0x0df|+0x012:'445'
	// <0x5beaf1>|0x0f1|+0x06d:'446'
	// <0x5beb5e>|0x15e|+0x03b:'447'
	// <0>
	// <0x5beb99>|0x199|+0x012:'449'
	// <0x5bebab>|0x1ab|+0x067:'450'
	// <0x5bec12>|0x212|+0x042:'451'
	// <0>
	// <0x5bec54>|0x254|+0x025:'453'
	// <0x5bec79>|0x279|+0x025:'454'
	// ******
}

// STATE[STUB]
void human_npc::set_attributes( human_npc::npc_game_attributes& attributes )
{
	// FUNCTION BODY[0x5bf100]: 4
	// <0x5bf10a>|0x00a|+0x00c:'459'
	// <0>
	// <1>
	// <0x5bf116>|0x016|+0x0ad:'462'
	// ******
}

// STATE[STUB]
void human_npc::get_available_weapons( vectora< ai::weapon* >& list_to_be_filled ) const
{
	// FUNCTION BODY[0x5be670]: 2
	// <0x5be673>|0x003|+0x00d:'467'
	// <0x5be680>|0x010|+0x028:'468'
	// ******
}

// STATE[STUB]
void human_npc::set_translation( float4x4 const& new_translation )
{
	// LOCALS
	// float4x4 						new_transform
	// ******

	// FUNCTION BODY[0x5c0790]: 5
	// <0>
	// <1>
	// <0x5c079e>|0x00e|+0x142:'484'
	// <0>
	// <0x5c08e0>|0x150|+0x00a:'486'
	// ******
}

// STATE[STUB]
void human_npc::set_behaviour( resources::unmanaged_resource_ptr new_behaviour )
{
	// CALL SITE INFO
	// <0x5be863> -> void < unknown >( resources::unmanaged_resource_ptr, resources::resource_ptr< resources::unmanaged_resource, resources::unmanaged_intrusive_base > )
	// ******

	// FUNCTION BODY[0x5be830]: 1
	// <0x5be830>|0x000|+0x035:'491'
	// ******
}

// STATE[STUB]
bool human_npc::debug_draw_allowed( ) const
{
	return false;

	// FUNCTION BODY[0x5be420]: 1
	// <0x5be420>|0x000|+0x005:'496'
	// ******
}

// STATE[STUB]
void human_npc::move_to_position( ai::movement_target const* const target )
{
	// CALL SITE INFO
	// <0x5c0fcf> -> pcstr < unknown >() const
	// ******

	// FUNCTION BODY[0x5c0f10]: 10
	// <0x5c0f19>|0x009|+0x00d:'501'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5c0f26>|0x016|+0x109:'509'
	// <0x5c102f>|0x11f|+0x011:'510'
	// ******
}

// STATE[STUB]
void human_npc::on_animation_end( )
{
	// CALL SITE INFO
	// <0x5bf4db> -> pcstr < unknown >() const
	// <0x5bf568> -> void < unknown >( ai::animation_item const* const, resources::unmanaged_resource_ptr )
	// <0x5bf588> -> void < unknown >( resources::unmanaged_resource_ptr )
	// ******

	// FUNCTION BODY[0x5bf430]: 13
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5bf43f>|0x00f|+0x106:'520'
	// <0x5bf545>|0x115|+0x025:'521'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5bf56a>|0x13a|+0x020:'527'
	// ******
}

// STATE[STUB]
void human_npc::hit(
	hit_initiator const* const		initiator,
	const u32						bone_index,
	pcstr							damage_type,
	const float						amount,
	const float						armor_piercing,
	bullet* const					bullet
)
{
	// FUNCTION BODY[0x5bf0a0]: 2
	// <0x5bf0a0>|0x000|+0x006:'539'
	// <0x5bf0a6>|0x006|+0x04b:'540'
	// ******
}

// STATE[STUB]
void human_npc::hit(
	hit_initiator const* const		initiator,
	collision::bone_collision_data const&	bone_data,
	pcstr							damage_type,
	const float						amount,
	const float						armor_piercing,
	bullet* const					bullet
)
{
	// FUNCTION BODY[0x5bf050]: 1
	// <0x5bf050>|0x000|+0x045:'552'
	// ******
}

// STATE[STUB]
void human_npc::on_movement_end( )
{
	// CALL SITE INFO
	// <0x5bf419> -> void < unknown >( resources::unmanaged_resource_ptr )
	// ******

	// FUNCTION BODY[0x5bf2e0]: 15
	// <0x5bf2ed>|0x00d|+0x010:'557'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5bf2fd>|0x01d|+0x0f2:'565'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5bf3ef>|0x10f|+0x02c:'571'
	// ******
}

// STATE[STUB]
void human_npc::play_animation( ai::animation_item const* const target )
{
	// LOCALS
	// animation::animation_expression_emitter_ptr animation_emitter
	// ******

	// CALL SITE INFO
	// <0x5c0e76> -> pcstr < unknown >() const
	// ******

	// FUNCTION BODY[0x5c0da0]: 4
	// <0x5c0da9>|0x009|+0x00f:'576'
	// <0x5c0db8>|0x018|+0x027:'577'
	// <0x5c0ddf>|0x03f|+0x011:'578'
	// <0x5c0df0>|0x050|+0x0f3:'579'
	// ******
}

// STATE[STUB]
void human_npc::tick_animation_player( const u32 current_time_in_ms )
{
	// FUNCTION BODY[0x5c0b30]: 4
	// <0x5c0b38>|0x008|+0x014:'584'
	// <0x5c0b4c>|0x01c|+0x030:'585'
	// <0x5c0b7c>|0x04c|+0x007:'586'
	// <0x5c0b83>|0x053|+0x005:'587'
	// ******
}

// STATE[STUB]
void human_npc::up_to_terrain( )
{
	// LOCALS
	// physics::closest_ray_result 		result
	// ******

	// CALL SITE INFO
	// <0x5c0984> -> physics::closest_ray_result < unknown >( float3 const&, float3 const&, const float, u16, u16 )
	// ******

	// FUNCTION BODY[0x5c0900]: 13
	// <0>
	// <0x5c0906>|0x006|+0x008:'593'
	// <0>
	// <0x5c090e>|0x00e|+0x078:'595'
	// <0x5c0986>|0x086|+0x007:'596'
	// <0x5c098d>|0x08d|+0x03c:'597'
	// <0>
	// <0x5c09c9>|0x0c9|+0x043:'599'
	// <0>
	// <1>
	// <0x5c0a0c>|0x10c|+0x030:'602'
	// <0x5c0a3c>|0x13c|+0x091:'603'
	// <0x5c0acd>|0x1cd|+0x04f:'604'
	// ******
}

// STATE[STUB]
void human_npc::select_new_goal( )
{
	// CALL SITE INFO
	// <0x5be820> -> void < unknown >( resources::unmanaged_resource_ptr )
	// ******

	// FUNCTION BODY[0x5be800]: 1
	// <0x5be800>|0x000|+0x022:'609'
	// ******
}

// STATE[STUB]
void human_npc::on_affect_event(
	pcstr							body_part_name,
	const hit_affects_type_enum		arg_1 /* hit_affects_type_enum affect_type */,
	const affect_event_type_enum	arg_2 /* affect_event_type_enum event_type */
) const
{
	// CALL SITE INFO
	// <0x5bf26a> -> pcstr < unknown >() const
	// ******

	// FUNCTION BODY[0x5bf1d0]: 3
	// <0>
	// <0x5bf1e2>|0x012|+0x013:'615'
	// <0x5bf1f5>|0x025|+0x0d4:'616'
	// ******
}

} // namespace survarium
