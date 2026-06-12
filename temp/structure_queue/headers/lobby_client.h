////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef LOBBY_CLIENT_H_INCLUDED
#define LOBBY_CLIENT_H_INCLUDED

/* INCLUDES */
class boost::function<void __cdecl(void)>;
class boost::function<void __cdecl(vostok::network_core::packet_reader &)>;
class vostok::core::noncopyable;
class vostok::fixed_string<128>;
class vostok::fixed_string<32>;
class vostok::network::tcp_packet_client;
class vostok::vectora<survarium::inventory_item_instance>;
class survarium::game;
struct vostok::server_connection_info;
struct survarium::account_money;
struct survarium::faction_price;
struct survarium::items_compatibility;
struct survarium::player_leveling_info;
struct survarium::player_profile;
struct survarium::player_reputation;
struct survarium::player_skill;
struct survarium::profile_slot_restriction;
struct survarium::service_prices;
enum vostok::network_core::client_error_codes_enum;
class survarium::game_team_id;
class survarium::inventory_item_instance;
typedef vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>
	vostok::configs::binary_config_ptr;
class vostok::configs::binary_config;

/* FORWARD REFS */
class vostok::network_core::packet_reader;
class vostok::vectora<survarium::player_skill>;
class vostok::vectora<unsigned char>;
class survarium::vector<survarium::relocate_item_descr>;

namespace survarium {

enum survarium::game_team_id
{
	team_1				= 0x00,
	team_2				= 0x01,
	team_neutral		= 0x02,
	team_undefined		= 0x03,
	team_invalid		= 0xff,
};
enum lobby::client_state_enum
{
	surf_lobby_menu				= 0x0,
	in_match_making_order		= 0x1,
	in_match_making				= 0x2,
	in_match					= 0x3,
	unknown						= 0x4,
};

class lobby_client : public core::noncopyable {
public:
			explicit								lobby_client						( game& g );
													~lobby_client						( );

			void									connect								( server_connection_info const& lobby_connection_info );

			void									disconnect							( );

	inline	void									set_on_packet_received				( boost::function< void( network_core::packet_reader& ) > const& arg_0 ) { /* no source */ }
	inline	void									set_on_connected					( boost::function< void() > const& arg_0 ) { /* no source */ }
	inline	void									set_on_disconnected					( boost::function< void() > const& arg_0 ) { /* no source */ }

			u32										session_id							( ) const;

	inline	bool									net_connected						( ) const { /* no source */ }

			void									set_status_ready_for_match			(
														const u32		arg_0 /* u32 profile_id */
													);

			void									query_client_status					( lobby::query_info_types type );
			void									query_prices						(
														const u32		arg_0 /* u32 faction_id */
													);
			void									query_profile_contents				(
														const u32		arg_0 /* u32 profile_id */
													);

			void									discard_playing_order				( );
			void									discard_playing_order_on_connected	( );

	inline	lobby::client_state_enum				status								( ) const { /* no source */ }
			lobby::client_state_enum				status								( fixed_string< 128 >& dest ) const;
	inline	lobby::client_state_enum&				status								( ) { /* no source */ }

	inline	u32										match_id							( ) const { /* no source */ }
	inline	u32&									match_id							( ) { /* no source */ }
	inline	game_team_id							team_id								( ) const { /* no source */ }
	inline	game_team_id&							team_id								( ) { /* no source */ }
	inline	u32										match_order_id						( ) const { /* no source */ }

	inline	u8										profiles_count						( ) const { /* no source */ }

	inline	player_profile const&					profile								( u8 arg_0 ) const { /* no source */ }

	inline	u8										player_skills_count					( ) const { /* no source */ }
	inline	player_skill const&						player_skill						( u8 arg_0 ) const { /* no source */ }
	inline	u8										player_perks_count					( ) const { /* no source */ }
	inline	u8										player_perk							( u8 arg_0 ) const { /* no source */ }

	inline	u32										slot_restrictions_count				( ) const { /* no source */ }
	inline	profile_slot_restriction const&			slot_restriction					( u32 arg_0 ) const { /* no source */ }

	inline	u32										item_compatibilities_count			( ) const { /* no source */ }

	inline	items_compatibility const&				get_items_compatibility				( u32 arg_0 ) const { /* no source */ }
			bool									check_compatibility					( const u32 first_item_id, const u32 second_item_id );

			bool									can_move_item						( const u32 item_category_id, const u32 target_slot_id );
			void									move_item							( vector< relocate_item_descr >& items );

			faction_price const&					price								(
														const u8	arg_0 /* u8 faction_id */
													);

			void									buy_item							(
														const u16		item_dict_id,
														const u32		count,
														const u8		faction_id,
														const bool		use_premium_money
													);

			void									set_player_skills					( vectora< player_skill >& skills, vectora< u8 >& perks );
			void									reroll_player_skills				( );

	inline	vectora< inventory_item_instance >&		inventory_item_instances			( ) { /* no source */ }

	inline	void									add_inventory_item					( inventory_item_instance arg_0 ) { /* no source */ }

			bool									read_status_info					( network_core::packet_reader& reader );
			bool									read_enumerate_profiles_info		( network_core::packet_reader& reader );
			u8										read_profile_content_info			( network_core::packet_reader& reader );
			bool									read_enumerate_inventory_info		( network_core::packet_reader& reader );
			u8										read_price_items					( network_core::packet_reader& reader );
			bool									read_profile_slots_restrictions		( network_core::packet_reader& reader );
			bool									read_items_compatibility			( network_core::packet_reader& reader );
			bool									read_account_money					( network_core::packet_reader& reader );
			bool									read_player_skills					( network_core::packet_reader& reader );
			bool									read_player_reputations				( network_core::packet_reader& reader );
			bool									read_player_skills_tree				( network_core::packet_reader& reader );
			bool									read_service_prices					( network_core::packet_reader& reader );
			bool									read_ping_server_answer				( network_core::packet_reader& reader );

	inline	server_connection_info&					connection_info						( ) { /* no source */ }

	inline	account_money const&					get_account_money					( ) { /* no source */ }
	inline	player_leveling_info const&				get_player_leveling					( ) { /* no source */ }
	inline	service_prices const&					get_service_prices					( ) { /* no source */ }

	inline	configs::binary_config_ptr&				skills_tree_config					( ) { /* no source */ }

	inline	u32										get_player_faction_reputation		( u8 arg_0 ) { /* no source */ }
	inline	player_reputation const&				get_player_reputation				( u8 arg_0 ) { /* no source */ }
	inline	u8										get_player_reputations_count		( ) { /* no source */ }
	inline	fixed_string< 32 > const&				get_player_name						( ) const { /* no source */ }

			void									ping_server							( );

			void									on_connected						( );
			void									on_disconnected						( );
			void									on_error							(
														network_core::client_error_codes_enum	__formal,
														boost::system::error_code		__formal
													);

			void									sign_in_on_packet_received			( network_core::packet_reader& reader );

			void									clear_initial_info					( );
			void									clear_profile_info					( );

private:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	char									account_nickname_[32];
	/* 0x0020 */	game&									m_game;
	/* 0x0024 */	server_connection_info					m_connection_info;
	/* 0x00a8 */	network::tcp_packet_client				m_packet_client;
	/* 0x0130 */	boost::function< void( network_core::packet_reader& ) >	m_on_packet_received;
	/* 0x0150 */	boost::function< void() >				m_on_connected;
	/* 0x0170 */	boost::function< void() >				m_on_disconnected;
	/* 0x0190 */	lobby::client_state_enum				m_status;
	/* 0x0194 */	bool									m_net_client_connected;
	/* 0x0198 */	u32										m_match_id;
	/* 0x019c */	u32										m_match_order_id;
	/* 0x01a0 */	game_team_id							m_team_id;
	/* 0x01a4 */	fixed_string< 128 >						m_last_status_message;
	/* 0x0230 */	fixed_string< 32 >						m_player_name;
	/* 0x025c */	u8										m_profiles_count;
	/* 0x0260 */	player_profile							m_profiles[3];
	/* 0x0788 */	vectora< inventory_item_instance >		m_inventory_item_instances;
	/* 0x0798 */	player_skill*							m_player_skills;
	/* 0x079c */	u8										m_player_skills_count;
	/* 0x07a0 */	player_reputation*						m_player_reputations;
	/* 0x07a4 */	u8										m_player_reputations_count;
	/* 0x07a8 */	faction_price							m_prices[16];
	/* 0x0828 */	account_money							m_account_money;
	/* 0x0834 */	player_leveling_info					m_player_leveling_info;
	/* 0x0844 */	pbyte									m_player_perks;
	/* 0x0848 */	u8										m_player_perks_count;
	/* 0x084c */	profile_slot_restriction*				m_profile_slot_restrictions;
	/* 0x0850 */	u32										m_profile_slot_restrictions_count;
	/* 0x0854 */	items_compatibility*					m_items_compatibility;
	/* 0x0858 */	u32										m_items_compatibilities_count;
	/* 0x085c */	configs::binary_config_ptr				m_skills_tree_config;
	/* 0x0860 */	service_prices							m_service_prices;
	/* 0x086c */	bool									m_discard_playing_order_on_connected;
}; // class lobby_client

STATIC_SIZE_ASSERT(lobby_client, 0x870);

} // namespace survarium

#endif // #ifndef LOBBY_CLIENT_H_INCLUDED
