////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\GAME\SOURCES\PLAYER_H_INCLUDED
#define VOSTOK\GAME\SOURCES\PLAYER_H_INCLUDED

namespace vostok {
namespace resources {

// STATE[STUB]
inline survarium::game_team_id survarium::player::team( ) const
{
	// FUNCTION BODY[0x8e730]
	// <0x8e730>|0x000|      :'105'	{
	// ******
}

// STATE[STUB]
inline survarium::damage_model_ptr const& survarium::player::damage_model( ) const
{
	// FUNCTION BODY[0x8e740]
	// <0x8e740>|0x000|      :'107'	{
	// ******
}

// STATE[STUB]
inline float4x4 const& survarium::player::get_transform( ) const
{
	// FUNCTION BODY[0x8e750]
	// <0x8e750>|0x000|      :'130'	{
	// ******
}

// STATE[STUB]
inline survarium::player_input const& survarium::player::input( ) const
{
	// FUNCTION BODY[0x8e760]
	// <0x8e760>|0x000|      :'160'	{
	// ******
}

// STATE[STUB]
inline float3 const& survarium::player::position( ) const
{
	// FUNCTION BODY[0x8e770]
	// <0x8e770>|0x000|      :'161'	{
	// ******
}

// STATE[STUB]
inline float survarium::player::get_look_pitch( ) const
{
	return 0.0f;

	// FUNCTION BODY[0x8e780]
	// <0x8e780>|0x000|      :'162'	{
	// ******
}

// STATE[STUB]
inline physics::bt_character_controller& survarium::player::physics_controller( )
{
	// FUNCTION BODY[0x8e790]
	// <0x8e790>|0x000|      :'180'	{
	// ******
}

// STATE[STUB]
inline survarium::player_stamina& survarium::player::stamina( )
{
	// FUNCTION BODY[0x8e7a0]
	// <0x8e7a0>|0x000|      :'181'	{
	// ******
}

// STATE[STUB]
inline bool survarium::player::is_replaying_history( ) const
{
	return false;

	// FUNCTION BODY[0x8e7b0]
	// <0x8e7b0>|0x000|      :'201'	{
	// ******
}

// STATE[STUB]
inline u32 survarium::player::local_time( const u32 time_in_ms ) const
{
	return 0;

	// FUNCTION BODY[0x8e7c0]
	// <0x8e7c0>|0x000|      :'202'	{
	// ******
}

// STATE[STUB]
inline  resource_ptr<survarium::player,vostok::resources::unmanaged_intrusive_base>::~resource_ptr<survarium::player,vostok::resources::unmanaged_intrusive_base>( )
{
	// FUNCTION BODY[0x8e7e0]
	// <0x8e7d0>|0x000|      :'262'	{
	// ******
}

	// TYPEDEFS
	// typedef
	// 	char[32]
	// 	account_name_type;

	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::inventory_item_instance*
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
	// 	vostok::fixed_vector< std::pair< enum survarium::game_action_id, enum survarium::player_input_handler::action_state_enum >, 32 >
	// 	actions_type;

	// ******

} // namespace resources
} // namespace vostok

#endif // #ifndef VOSTOK\GAME\SOURCES\PLAYER_H_INCLUDED
