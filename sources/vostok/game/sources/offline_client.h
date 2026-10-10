// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OFFLINE_CLIENT_H_INCLUDED
#define OFFLINE_CLIENT_H_INCLUDED

#include "network_client.h"
#include <vostok/game_core/player_profile.h>

namespace vostok { namespace vfs { class vfs_locked_iterator; } }

namespace survarium {

// Runs the normal world and player resource cooks without a login or match server.
class offline_client : public network_client {
public:
		explicit offline_client( game& game );
	virtual ~offline_client( );

	virtual void load( pcstr map_name, camera_director const& director ) override;
	virtual void disconnect( ) override;
	virtual void tick( u32 current_time_in_ms, bool is_game_paused ) override;
	virtual bool has_bandwidth( ) const override { return false; }
	virtual void send_local_player_input( player_input const&, u32, float4x4 const&, float ) override { }
	virtual void unload( ) override;
	virtual void initiate_kill_current_player( ) override;
	virtual void initiate_respawn_current_player( ) override;
	virtual player_ptr get_player( u8 id ) const override;
	virtual bool is_player_local( u8 id ) const override;
	virtual void close_current_match( bool user_initiate ) override;

	static pcstr default_map( );
	void initialize_lobby( );
	void choose_map_from_chat( wchar_t const* text );
	void show_maps( );
	void open_map_chat( );
	void toggle_free_camera( );
	void tick_map_chat( );
	enum { map_chat_settle_frames = 60, map_chat_unfold_frames = 10 };
	void start_match( );
	void load_command( pcstr map_name );

private:
	void on_player_loaded( resources::queries_result& data );
	void on_maps_ready( vfs::vfs_locked_iterator const& maps );
	bool map_available( pcstr map_name ) const;
	void show_map_error( pcstr map_name );
	void print_map_list( );
	bool configure_weapon( );

	player_profile m_profile;
	player_ptr m_player;
	fixed_string< 64 > m_map_name;
	vector< fixed_string< 64 > > m_available_maps;
	u32 m_map_chat_delay;
};

} // namespace survarium

#endif // OFFLINE_CLIENT_H_INCLUDED
