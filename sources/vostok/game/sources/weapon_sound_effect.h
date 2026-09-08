// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_SOUND_EFFECT_H_INCLUDED
#define WEAPON_SOUND_EFFECT_H_INCLUDED

#include <vostok/animation/animation_callback.h>
#include <vostok/sound/sound_emitter.h>
#include <vostok/sound/sound_instance_proxy.h>

namespace survarium {

class weapon;

class weapon_sound_effect : private boost::noncopyable {
	typedef sound::sound_emitter_ptr sound_emitter_type;
	typedef sound::sound_instance_proxy_ptr sound_instance_type;

public:
					weapon_sound_effect	(
						weapon&								weapon,
						sound::sound_emitter_ptr* const		first_view_sounds_emitters,
						const u8							first_view_sounds_emitters_count,
						sound::sound_instance_proxy_ptr* const	first_view_sounds_instances,
						const u8							max_first_view_sounds_instances_count,
						sound::sound_emitter_ptr* const		third_view_sounds_emitters,
						const u8							third_view_sounds_emitters_count,
						sound::sound_instance_proxy_ptr* const	third_view_sounds_instances,
						const u8							max_third_view_sounds_instances_count,
						bool								stop_sounds_on_state_finalize,
						u8									simultaneous_sounds_queue_size
					);

			void	initialize			( );
			void	finalize			( );

			animation::callback_return_type_enum	on_sound_event		( animation::animation_callback_params& params );

	typedef buffer_vector< sound_instance_type > sounds_instances_type;
	typedef buffer_vector< sound_emitter_type > sounds_emitters_type;

public:
	struct sounds : private boost::noncopyable {
		inline			sounds			(
							sound_emitter_type* const			arg_0,
							const u8							arg_1,
							sound_instance_type* const			arg_2,
							const u8							arg_3
						)
			:	sounds_emitters		( arg_0, arg_1, arg_1 ),
				sounds_instances	( arg_2, arg_3 )
		{ }

		inline	void	clear_instances	( ) { sounds_instances.clear( ); }

public:
		/* 0x0000 */	/* boost::noncopyable */
		/* 0x0000 */	const buffer_vector< sound_emitter_type >	sounds_emitters;
		/* 0x0008 */	buffer_vector< sound_instance_type >		sounds_instances;
	}; // struct sounds

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	sounds		m_first_view_sounds;
	/* 0x0010 */	sounds		m_third_view_sounds;
	/* 0x0020 */	weapon&		m_weapon;
	/* 0x0024 */	bool		m_stop_sounds_on_state_finalize;
	/* 0x0025 */	u8			m_sounds_counter;
	/* 0x0026 */	u8			m_simultaneous_sounds_queue_size;
}; // class weapon_sound_effect

STATIC_SIZE_ASSERT(weapon_sound_effect, 0x28);
STATIC_SIZE_ASSERT(weapon_sound_effect::sounds, 0x10);

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EFFECT_H_INCLUDED
