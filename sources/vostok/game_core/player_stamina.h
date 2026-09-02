// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef PLAYER_STAMINA_H_INCLUDED
#define PLAYER_STAMINA_H_INCLUDED

#include <vostok/game_core/player_stamina_subscriber.h>

namespace vostok {
namespace network_core {
	class packet_reader;
	class udp_match_packet;
}
}

namespace survarium {

class player_stamina : private boost::noncopyable {
public:
								player_stamina					( );
			explicit			player_stamina					( player_stamina const& other );
			player_stamina&		operator=						( player_stamina const& other );

			void				deserialize						( network_core::packet_reader& packet );
	inline	void				serialize						( network_core::udp_match_packet& arg_0, s32 arg_1 ) const { /* no source */ }

			void				load							( configs::binary_config_value const& config );
			void				reset							( );

	inline	void				set_max_value					( const float max_value )				{ m_max_value				= max_value;					}
	inline	void				set_spending_speed				( const float spending_speed )			{ m_spending_speed			= spending_speed;				}

	inline	float				get_regeneration_speed			( ) { return m_regeneration_speed; }
			void				set_regeneration_speed			( const float new_regeneration_speed );

	inline	void				set_regeneration_threshold		( const float new_regeneration_threshold ) { m_regeneration_threshold = new_regeneration_threshold; }
	inline	void				set_max_value_factor			( const float new_max_value_factor )		{ m_max_value_factor		= new_max_value_factor;			}
	inline	void				set_spending_speed_factor		( const float new_spending_speed_factor )	{ m_spending_speed_factor	= new_spending_speed_factor;	}
			void				set_regeneration_speed_factor	( const float new_regeneration_speed_factor );

			void				tick							( const u32 current_time_in_ms, const bool is_sprinting );

			void				spend							( const float amount );
			bool				can_be_spent					( ) const;

	inline	float				current_value					( ) const { return m_value; }
	inline	float				max_value						( ) const { return m_max_value; }
	inline	float				amount_to_jump					( ) const { return m_max_value * m_max_value_factor / 5.0f; }

	inline	float				get_max_carried_weight			( ) const { return m_max_carried_weight; }
	inline	void				set_max_carried_weight			( float max_carried_weight ) { m_max_carried_weight = max_carried_weight; }

			void				subscribe_on_depletion			( player_stamina_subscriber* const subscriber );
			void				unsubscribe_from_depletion		( player_stamina_subscriber* const subscriber );

private:
			void				increase_value					( const float amount );
			void				decrease_value					( const float amount );

			void				regenerate						( const u32 current_time_in_ms );
			void				sprint							( const u32 current_time_in_ms );

private:
	typedef intrusive_list<
		player_stamina_subscriber,
		player_stamina_subscriber*,
		&player_stamina_subscriber::next,
		threading::mutex,
		size_policy,
		no_debug_policy
	> stamina_subscribers_type;

	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	player_stamina_subscriber_list		m_subscribers;
	/* 0x0030 */	float								m_max_value;
	/* 0x0034 */	float								m_value;
	/* 0x0038 */	float								m_regeneration_threshold;
	/* 0x003c */	float								m_spending_threshold;
	/* 0x0040 */	float								m_spending_speed;
	/* 0x0044 */	float								m_regeneration_speed;
	/* 0x0048 */	float								m_max_value_factor;
	/* 0x004c */	float								m_spending_speed_factor;
	/* 0x0050 */	float								m_regeneration_speed_factor;
	/* 0x0054 */	u32									m_last_spending_time_in_ms;
	/* 0x0058 */	u32									m_last_tick_time_in_ms;
	/* 0x005c */	bool								m_lower_threshold_was_reached;
	/* 0x0060 */	float								m_max_carried_weight;
}; // class player_stamina

STATIC_SIZE_ASSERT(player_stamina, 0x68);

} // namespace survarium

#endif // #ifndef PLAYER_STAMINA_H_INCLUDED
