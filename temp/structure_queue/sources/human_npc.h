////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\GAME\SOURCES\HUMAN_NPC_H_INCLUDED
#define VOSTOK\GAME\SOURCES\HUMAN_NPC_H_INCLUDED

namespace survarium {

// STATE[STUB]
inline math::color human_npc::get_color( ) const
{
	return vostok::math::color();

	// FUNCTION BODY[0xaba60]
	// <0xaba60>|0x000|      :'123'	{
	// ******
}

// STATE[STUB]
inline void human_npc::set_eyes_direction( float3 const& direction )
{
	// FUNCTION BODY[0x22c50]
	// <0x22c50>|0x000|      :'125'	{
	// ******
}

// STATE[STUB]
inline pcstr human_npc::get_name( ) const
{
	return NULL;

	// FUNCTION BODY[0xaba70]
	// <0xaba70>|0x000|      :'131'	{
	// ******
}

// STATE[STUB]
inline u32 human_npc::get_id( ) const
{
	return 0;

	// FUNCTION BODY[0xaba80]
	// <0xaba80>|0x000|      :'132'	{
	// ******
}

// STATE[STUB]
inline u32 human_npc::get_group_id( ) const
{
	return 0;

	// FUNCTION BODY[0xaba90]
	// <0xaba90>|0x000|      :'134'	{
	// ******
}

// STATE[STUB]
inline u32 human_npc::get_class_id( ) const
{
	return 0;

	// FUNCTION BODY[0xabaa0]
	// <0xabaa0>|0x000|      :'135'	{
	// ******
}

// STATE[STUB]
inline u32 human_npc::get_outfit_id( ) const
{
	return 0;

	// FUNCTION BODY[0xabab0]
	// <0xabab0>|0x000|      :'136'	{
	// ******
}

// STATE[STUB]
inline bool human_npc::is_patrolling( ) const
{
	return false;

	// FUNCTION BODY[0xabac0]
	// <0xabac0>|0x000|      :'138'	{
	// ******
}

// STATE[STUB]
inline bool human_npc::is_at_cover( ) const
{
	return false;

	// FUNCTION BODY[0xb2740]
	// <0xb2740>|0x000|      :'139'	{
	// ******
}

// STATE[STUB]
inline pcstr human_npc::get_description( ) const
{
	return NULL;

	// FUNCTION BODY[0xabad0]
	// <0xabad0>|0x000|      :'157'	{
	// ******
}

// STATE[STUB]
inline float3 human_npc::get_source_position( float3 const& requester ) const
{
	// CALL SITE INFO
	// <0xabaf4> -> float3 < unknown >( float3 const& ) const
	// ******

	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0xabae0]
	// <0xabae0>|0x000|      :'158'	{
	// ******
}

// STATE[STUB]
inline ai::npc* human_npc::cast_npc( )
{
	return NULL;

	// FUNCTION BODY[0xabb00]
	// <0xabb00>|0x000|      :'161'	{
	// ******
}

// STATE[STUB]
inline float human_npc::get_velocity( ) const
{
	return 0.0f;

	// FUNCTION BODY[0xabb10]
	// <0xabb10>|0x000|      :'165'	{
	// ******
}

// STATE[STUB]
inline float human_npc::get_luminosity( ) const
{
	return 0.0f;

	// FUNCTION BODY[0xabb20]
	// <0xabb20>|0x000|      :'166'	{
	// ******
}

// STATE[STUB]
inline ai::game_object const* human_npc::cast_game_object( ) const
{
	return NULL;

	// FUNCTION BODY[0xabeb0]
	// <0xabeb0>|0x000|      :'169'	{
	// ******
}

// STATE[STUB]
inline void human_npc::load(
	configs::binary_config_value const&		__formal,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	__formal
)
{
	// FUNCTION BODY[0xabb30]
	// <0xabb30>|0x000|      :'170'	{
	// ******
}

// STATE[STUB]
inline void* human_npc::`scalar deleting destructor'( u32 arg_0 )
{
	return NULL;

	// FUNCTION BODY[0xabb50]
	// <0xabb40>|0x000|      :'235'	{
	// ******
}

	// TYPEDEFS
	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::scheduler::record*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::ray_object_result*
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 64 >
	// 	content_type;

	// typedef
	// 	vostok::one_way_threads_channel< vostok::intrusive_spsc_queue< vostok::sound::sound_order, vostok::sound::sound_order, 4 >, vostok::intrusive_spsc_queue< vostok::sound::sound_order, vostok::sound::sound_order, 4 > >
	// 	orders_channel_type;

	// typedef
	// 	vostok::one_way_threads_channel< vostok::intrusive_spsc_queue< vostok::sound::sound_response, vostok::sound::sound_response, 4 >, vostok::intrusive_spsc_queue< vostok::sound::sound_response, vostok::sound::sound_response, 4 > >
	// 	responses_channel_type;

	// typedef
	// 	vostok::vectora< float3 >
	// 	path_type;

	// ******

} // namespace survarium

#endif // #ifndef VOSTOK\GAME\SOURCES\HUMAN_NPC_H_INCLUDED
