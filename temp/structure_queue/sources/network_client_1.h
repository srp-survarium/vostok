////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\GAME\SOURCES\NETWORK_CLIENT_H_INCLUDED
#define VOSTOK\GAME\SOURCES\NETWORK_CLIENT_H_INCLUDED

namespace survarium {

// STATE[STUB]
inline  player_desc::player_desc( )
{
	// FUNCTION BODY[0x92180]: 0
	// <0x92180>|0x000|+0x00c:'39'	{
	// <0x9218c>|0x00c|      :'40'	}
	// ******
}

// STATE[STUB]
inline void network_client::load( pcstr map_name, camera_director const& director )
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x9da12 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x9da12 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// ******

	// FUNCTION BODY[0xaea00]
	// <0xaea00>|0x000|      :'53'	{
	// ******
}

// STATE[STUB]
inline bool network_client::has_bandwidth( ) const
{
	return false;

	// FUNCTION BODY[0x22700]
	// <0x22700>|0x000|      :'57'	{
	// ******
}

// STATE[STUB]
inline void network_client::initiate_respawn_current_player( )
{
	// FUNCTION BODY[0x4f210]
	// <0x4f210>|0x000|      :'65'	{
	// ******
}

// STATE[STUB]
inline network::login_client& network_client::login_client( )
{
	// FUNCTION BODY[0x92190]
	// <0x92190>|0x000|      :'69'	{
	// ******
}

// STATE[STUB]
inline lobby_client& network_client::lobby_client( )
{
	// FUNCTION BODY[0x921a0]
	// <0x921a0>|0x000|      :'70'	{
	// ******
}

// STATE[STUB]
inline match_client& network_client::match_client( )
{
	// FUNCTION BODY[0x921b0]
	// <0x921b0>|0x000|      :'71'	{
	// ******
}

// STATE[STUB]
inline messaging_client& network_client::messaging_client( )
{
	// FUNCTION BODY[0x921c0]
	// <0x921c0>|0x000|      :'72'	{
	// ******
}

// STATE[STUB]
inline void* network_client::`scalar deleting destructor'( u32 arg_0 )
{
	return NULL;

	// FUNCTION BODY[0x921e0]
	// <0x921d0>|0x000|      :'74'	{
	// ******
}

// STATE[STUB]
inline void network_client::on_player_hit_received( hit_info const& info )
{
	// FUNCTION BODY[0x22c50]
	// <0x22c50>|0x000|      :'76'	{
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
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// ******

} // namespace survarium

#endif // #ifndef VOSTOK\GAME\SOURCES\NETWORK_CLIENT_H_INCLUDED
