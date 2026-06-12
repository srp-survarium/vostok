////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EFFECT_SOUNDS_H_INCLUDED
#define WEAPON_SOUND_EFFECT_SOUNDS_H_INCLUDED

/* INCLUDES */
typedef vostok::buffer_vector<vostok::intrusive_ptr<vostok::sound::sound_instance_proxy,vostok::sound::sound_instance_proxy,vostok::threading::single_threading_policy> >
	vostok::buffer_vector<vostok::sound::sound_instance_proxy_ptr >;
typedef vostok::buffer_vector<vostok::resources::resource_ptr<vostok::sound::sound_emitter,vostok::resources::unmanaged_intrusive_base> >
	vostok::buffer_vector<vostok::sound::sound_emitter_ptr >;
class vostok::sound::sound_emitter;
class vostok::sound::sound_instance_proxy;

namespace survarium {

struct weapon_sound_effect::sounds : public boost::noncopyable {
	inline			sounds			(
						sound::sound_emitter_ptr* const		arg_0,
						const u8							arg_1,
						sound::sound_instance_proxy_ptr* const	arg_2,
						const u8							arg_3
					) { /* no source */ }

	inline	void	clear_instances	( ) { /* no source */ }

	inline			~sounds			( ) { /* no source */ }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	const buffer_vector< sound::sound_emitter_ptr >	sounds_emitters;
	/* 0x0008 */	buffer_vector< sound::sound_instance_proxy_ptr >	sounds_instances;
}; // struct weapon_sound_effect::sounds

STATIC_SIZE_ASSERT(weapon_sound_effect::sounds, 0x10);

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EFFECT_SOUNDS_H_INCLUDED
