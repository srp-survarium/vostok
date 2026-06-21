////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EFFECT_H_INCLUDED
#define WEAPON_SOUND_EFFECT_H_INCLUDED

#include <vostok/animation/animation_callback.h>
#include <vostok/sound/sound_emitter.h>
#include <vostok/sound/sound_instance_proxy.h>

namespace survarium {

class weapon;

class weapon_sound_effect : public boost::noncopyable {
public:
	struct sounds : public boost::noncopyable {
		inline			sounds			(
							sound::sound_emitter_ptr* const		arg_0,
							const u8							arg_1,
							sound::sound_instance_proxy_ptr* const	arg_2,
							const u8							arg_3
						)
			// buildability init (const member): the caller-provided buffers back
			// both vectors - a matcher confirms the real live counts.
			:	sounds_emitters		( arg_0, arg_1, arg_1 ),
				sounds_instances	( arg_2, arg_3 )
		{ /* no source */ }

		inline	void	clear_instances	( ) { sounds_instances.clear( ); }

		inline			~sounds			( ) { /* no source */ }

	public:
		/* 0x0000 */	/* boost::noncopyable */
		/* 0x0000 */	const buffer_vector< sound::sound_emitter_ptr >		sounds_emitters;
		/* 0x0008 */	buffer_vector< sound::sound_instance_proxy_ptr >	sounds_instances;
	}; // struct sounds

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

	inline			~weapon_sound_effect( ) { /* no source */ }

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
