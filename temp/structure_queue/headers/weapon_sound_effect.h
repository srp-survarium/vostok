////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EFFECT_H_INCLUDED
#define WEAPON_SOUND_EFFECT_H_INCLUDED

/* INCLUDES */
class survarium::weapon;
struct survarium::weapon_sound_effect::sounds;
enum vostok::animation::callback_return_type_enum;
class vostok::sound::sound_emitter;
class vostok::sound::sound_instance_proxy;

/* FORWARD REFS */
class vostok::animation::animation_callback_params;

namespace survarium {

class weapon_sound_effect : public boost::noncopyable {
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
	/* 0x0000 */	weapon_sound_effect::sounds		m_first_view_sounds;
	/* 0x0010 */	weapon_sound_effect::sounds		m_third_view_sounds;
	/* 0x0020 */	weapon&							m_weapon;
	/* 0x0024 */	bool							m_stop_sounds_on_state_finalize;
	/* 0x0025 */	u8								m_sounds_counter;
	/* 0x0026 */	u8								m_simultaneous_sounds_queue_size;
}; // class weapon_sound_effect

STATIC_SIZE_ASSERT(weapon_sound_effect, 0x28);

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EFFECT_H_INCLUDED
