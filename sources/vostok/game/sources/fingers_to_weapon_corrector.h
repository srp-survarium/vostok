// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef FINGERS_TO_WEAPON_CORRECTOR_H_INCLUDED
#define FINGERS_TO_WEAPON_CORRECTOR_H_INCLUDED

#include <vostok/detail_noncopyable.h>
#include <vostok/animation/api.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/render/facade/model.h>

namespace vostok {
namespace animation {
	class skeleton;
} // namespace animation
} // namespace vostok

namespace survarium {

class fingers_to_weapon_corrector : private core::noncopyable {
public:
	enum hands_enum {
		left			= 0x0,
		right			= 0x1,
		hands_count		= 0x2,
	};

	enum { phalanges_count = 15 };

public:
						fingers_to_weapon_corrector	( );

			void		activate					(
							animation::skeleton const&				character_skeleton,
							render::render_model_instance_ptr		weapon_model,
							const bool								first_person_view
						);

			void		process						( const u32 current_time_in_ms, float4x4* matrices ) const;

			void		activate_hand				(
							const hands_enum	hand,
							const bool			is_active,
							const u32			current_time_in_ms
						);

private:
			float		get_hand_coefficient		( const float hand_transition_time, const bool hand_active ) const;

			void		initialize_bones_indices	( animation::skeleton const& character_skeleton );
			void		initialize_locators			( render::render_model_instance const& weapon_model, const bool first_person_view );

	struct hand {
		inline		hand( ) :
			start_transition_time_in_ms	( 0 ),
			is_active					( true )
		{
		}

	public:
		/* 0x0000 */	float4x4	phalanges_matrices[ phalanges_count ];
		/* 0x03c0 */	u32			phalanges_bones_indices[ phalanges_count ];
		/* 0x03fc */	u32			start_transition_time_in_ms;
		/* 0x0400 */	bool		is_active;
	}; // struct hand

public:
	inline	void		set_first_person_view		( const bool arg_0 ) { /* no source */ }

private:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	hand								m_hands[2];
	/* 0x0808 */	animation::linear_interpolator		m_interpolator;
	/* 0x0810 */	render::render_model_instance_ptr	m_weapon_model;
	/* 0x0814 */	bool								m_first_person_view;
}; // class fingers_to_weapon_corrector

STATIC_SIZE_ASSERT(fingers_to_weapon_corrector, 0x818);
STATIC_SIZE_ASSERT(fingers_to_weapon_corrector::hand, 0x404);

} // namespace survarium

#endif // #ifndef FINGERS_TO_WEAPON_CORRECTOR_H_INCLUDED
