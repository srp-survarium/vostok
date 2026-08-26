////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef HAND_TO_WEAPON_IK_PROCESSOR_H_INCLUDED
#define HAND_TO_WEAPON_IK_PROCESSOR_H_INCLUDED

#include <vostok/animation/api.h>
#include <vostok/animation/linear_interpolator.h>

namespace vostok {
namespace network_core {
	class packet_reader;
	class udp_match_packet;
}
namespace animation {
	class skeleton;
}
}

namespace survarium {

class vostok::core::noncopyable;

class hand_to_weapon_ik_processor : public core::noncopyable {
public:
			enum hands_enum {
				left		= 0x0,
				right		= 0x1,
				hands_count = 0x2,
			};

			explicit	hand_to_weapon_ik_processor	( );

			void		activate					( animation::skeleton const& user_skeleton, animation::skeleton const& weapon_skeleton );
			void		process						( const u32 current_time_in_ms, float4x4 const* weapon_matrices, float4x4* user_matrices ) const;

			void		activate_hand				( const hand_to_weapon_ik_processor::hands_enum hand, const bool active, const u32 current_time_in_ms );

public:
			void		serialize					( network_core::udp_match_packet& packet, u32 client_offset ) const;
			void		deserialize					( network_core::packet_reader& reader );

public:
			struct hand {
				explicit	hand( )
					: start_transition_time_in_ms( 0 ), hand_bone_index( u32( -1 ) ), hand_matrix_index( u32( -1 ) ), locator_matrix_index( u32( -1 ) ), is_active( true )
				{
				}

				/* 0x0000 */	u32		start_transition_time_in_ms;
				/* 0x0004 */	u32		hand_bone_index;
				/* 0x0008 */	u32		hand_matrix_index;
				/* 0x000c */	u32		locator_matrix_index;
				/* 0x0010 */	bool	is_active;
			}; // struct hand

private:
			enum hands_bits {
				left_bit			= 1 << left,
				right_bit			= 1 << right,
				valid_hands_bits	= left_bit | right_bit,
				invalid_hands_bits	= ~valid_hands_bits,
			};

			void		process_hand				( hand_to_weapon_ik_processor::hand const& h, float4x4 const& target_hand_obj_space_transform, float4x4* matrices ) const;

private:
	static	bool		hand_need_correction				( hand_to_weapon_ik_processor::hand const& h, const u32 current_time_in_ms );
	static	bool		hand_need_interpolation				( hand_to_weapon_ik_processor::hand const& h, const u32 current_time_in_ms );
	static	u32			get_hand_new_start_transition_time	( hand_to_weapon_ik_processor::hand const& h, const u32 current_time_in_ms );

			float		get_hand_coefficient		( hand_to_weapon_ik_processor::hand const& h, const u32 current_time_in_ms ) const;

private:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	hand_to_weapon_ik_processor::hand	m_hands[2];
	/* 0x0028 */	animation::linear_interpolator		m_interpolator;
	/* 0x0030 */	u32									m_weapon_bone_index;
	/* 0x0034 */	float								m_current_transition_time;
	/* 0x0038 */	animation::skeleton const*			m_skeleton;
	/* 0x003c */	bool								m_active;
}; // class hand_to_weapon_ik_processor

STATIC_SIZE_ASSERT(hand_to_weapon_ik_processor, 0x40);

} // namespace survarium

#endif // #ifndef HAND_TO_WEAPON_IK_PROCESSOR_H_INCLUDED
