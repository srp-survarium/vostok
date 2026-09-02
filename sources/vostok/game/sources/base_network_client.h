// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef BASE_NETWORK_CLIENT_H_INCLUDED
#define BASE_NETWORK_CLIENT_H_INCLUDED

#include <boost/function.hpp>
#include <vostok/resources_unmanaged_resource.h>
#include <vostok/game_core/game_team_id.h>

namespace vostok {
namespace network {
	class login_client;
} // namespace network
} // namespace vostok

namespace survarium {

class camera_director;
class game;
struct hit_info;
class lobby_client;
class match_client;
struct match_options;
class messaging_client;
class player;
class player_input;
class player_input_handler;
class stats_graph;

// canonical UDT alias - player itself lives in player.h (batch 6), only the
// resource_ptr is held here
typedef resources::resource_ptr<
	player,
	resources::unmanaged_intrusive_base
> player_ptr;

class base_network_client : private boost::noncopyable {
public:
			explicit					base_network_client					( game& game );
	virtual								~base_network_client				( );

	virtual	void						load								( pcstr arg_0, camera_director const& arg_1 ) = 0;

	virtual	void						connect_to_login					(
											pcstr			arg_0,
											const u16		arg_1,
											pcstr			arg_2,
											pcstr			arg_3
										) = 0;

	virtual	void						disconnect							( ) = 0;

	virtual	void						tick								( u32 arg_0, const bool arg_1 ) = 0;

	virtual	bool						has_bandwidth						( ) const = 0;

	virtual	float3						get_current_player_position			( ) const;

	virtual	void						send_local_player_input				(
											player_input const&		arg_0,
											const u32				arg_1,
											float4x4 const&			arg_2,
											const float				arg_3
										) = 0;

	virtual	void						unload								( ) = 0;

	virtual	void						fill_current_player_stats			( boost::function< void( u32, float, float, pcstr ) > callback );

	virtual	void						initiate_kill_current_player		( ) = 0;
	virtual	void						initiate_respawn_current_player		( ) = 0;

	virtual	void						on_player_hit_received				( hit_info const& arg_0 ) = 0;

	virtual	match_options&				match_options						( ) = 0;

	virtual	network::login_client&		login_client						( ) = 0;
	virtual	lobby_client&				lobby_client						( ) = 0;
	virtual	match_client&				match_client						( ) = 0;
	virtual	messaging_client&			messaging_client					( ) = 0;

	virtual	player_ptr					get_player							( const u8 arg_0 ) const = 0;

	virtual	bool						is_player_local						( const u8 arg_0 ) const = 0;
			bool						is_player_current					( const u8 id ) const;

			game_team_id				current_player_team					( ) const;

			player_ptr					get_current_player					( );

			void						use_physics_controller_for_current	( pcstr const arguments );

	virtual	void						close_current_match					( bool arg_0 ) = 0;

			void						attach_to_player_cc					( pcstr const arguments );
			void						attach_to_player					( player_ptr player );
			void						detach_from_player					( );

private:
	virtual	void						apply_use_physics_controller_for_current( ) = 0;

protected:
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	bool						m_use_physics_controller_for_current;
	/* 0x0008 */	player_ptr					m_current_player;
	/* 0x000c */	player_input_handler*		m_input_handler;
	/* 0x0010 */	stats_graph*				m_linear_speed_graph;
	/* 0x0014 */	stats_graph*				m_angular_speed_graph;
	/* 0x0018 */	game&						m_game;
}; // class base_network_client

STATIC_SIZE_ASSERT(base_network_client, 0x1C);

} // namespace survarium

#endif // #ifndef BASE_NETWORK_CLIENT_H_INCLUDED
