////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef MEDKIT_H_INCLUDED
#define MEDKIT_H_INCLUDED

#include <vostok/game_core/inventory_item.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/damage_protector.h>
#include <vostok/game_core/hit_affects_type_enum.h>

namespace vostok {
namespace network_core {
	class udp_match_packet;
	class packet_reader;
}
}

namespace survarium {

struct base_player;
struct engine;
class items_cook;

class medkit : public inventory_item {
public:
	virtual	void								action						( bool key_down ) override;

private:
												medkit						( );
	virtual										~medkit						( );

			void								load						( configs::binary_config_value config );

protected:
			void								active_tick					( u32 frame_time_ms );
			void								set_active					( bool bactive );
	// body inferred from active_tick's inlined tail-if bytes (cmp m_activity_time_ms,0; sete; movzx)
	inline	bool								empty						( ) const { return !m_activity_time_ms; }
			void								remove_affects				( );
			float								reduce_damage				(
													pcstr		body_part_name,
													pcstr		damage_type,
													float		amount,
													float		armor_piercing
												);
private:
	// STATE[STUB]
	virtual	void								activate					( base_player& user, engine& engine ) override { /* VOSTOK_UNREFERENCED_PARAMETERS( user, engine ); */ }
	// STATE[STUB]
	virtual	void								deactivate					( ) override { }

	// STATE[STUB]
	virtual	float4x4							transform					( ) const override { VOSTOK_UNREACHABLE_CODE(); }

	virtual	void								tick						( ) override { /* no source */ }

	virtual	bool								is_ready_to_be_deactivated	( ) const override { /* no source sushi@TODO */ return false; }

	// STATE[STUB]
	virtual	animation::mixing::expression		selected_animations			( mutable_buffer& buffer, bool is_third_view ) const override { VOSTOK_UNREACHABLE_CODE(); }

	virtual	void								on_player_model_added		( ) override { /* no source */ }
	virtual	void								on_player_model_removed		( ) override { /* no source */ }

	// STATE[STUB]
	virtual	void								update_bones_matrices		(
													animation::skeleton_ptr const&		user_skeleton,
													float4x4* const						user_matrices,
													u32									user_matrices_count,
													u32									current_time_in_ms,
													float4x4&							character_head_transform,
													float4x4&							character_transform,
													animation::animation_player const&	animation_player
												) override {}

	// STATE[100.00%|DONE]: pure base forward; ICF-folds with oxygen_tank/weapon_ammunition onto
	// one body at rva 0xbcd90 (29 bytes) - this medkit rep carries the bytes, paired at 100 in report.json.
	virtual	void								serialize					( network_core::udp_match_packet& packet, u32 client_offset ) const override	{ inventory_item::serialize( packet, client_offset ); }
	// STATE[100.00%|DONE]: pure base forward; ICF-folds with oxygen_tank/weapon_ammunition onto
	// one body at rva 0xbcdb0 (25 bytes) - this medkit rep carries the bytes, paired at 100 in report.json.
	virtual	void								deserialize					( network_core::packet_reader& reader ) override								{ inventory_item::deserialize( reader ); }

	virtual	bool								is_sprinting				( ) const override { /* no source sushi@TODO */ return false; }

protected:
	struct affect {
		/* 0x0000 */	char					body_part_name[16];
		/* 0x0010 */	hit_affects_type_enum	type;
	}; // struct affect

	struct damage_protection {
		inline	damage_protection	( ) { /* no source */ }
		inline	~damage_protection	( ) { /* no source */ }

		/* 0x0000 */	damage_protector		protector;
		/* 0x0050 */	char					body_part_name[16];
		/* 0x0060 */	char					hit_type[16];
		/* 0x0070 */	float					hit_coeff;
		/* 0x0074 */	float					threshold;
	}; // struct damage_protection

	struct item_influence {
		/* 0x0000 */	char					body_part_name[16];
		/* 0x0010 */	float					health_amount;
	}; // struct item_influence

protected:
			medkit::damage_protection const*	find_damage_protection		( pcstr body_part_name, pcstr hit_type );

private:
	/* 0x0000 */	/* inventory_item */
	/* 0x0118 */	item_influence*			m_influences;
	/* 0x011c */	u8						m_influences_count;
	/* 0x0120 */	affect*					m_affects;
	/* 0x0124 */	u8						m_affects_count;
	/* 0x0128 */	damage_protection*		m_damage_protect;
	/* 0x012c */	u8						m_damage_protect_count;
	/* 0x0130 */	u32						m_config_activity_time_ms;
	/* 0x0134 */	u32						m_config_delay_ms;
	/* 0x0138 */	scheduler::identifier	m_scheduler_identifier;
	/* 0x013c */	u32						m_activity_time_ms;
	/* 0x0140 */	u32						m_delay_ms;
	/* 0x0144 */	bool					m_active;
	/* 0x0148 */	float					m_add_stamina_regen;

private:
	friend class items_cook;
}; // class medkit

STATIC_SIZE_ASSERT(medkit, 0x150);

} // namespace survarium

#endif // #ifndef MEDKIT_H_INCLUDED
