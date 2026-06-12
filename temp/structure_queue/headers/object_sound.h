////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_SOUND_H_INCLUDED
#define OBJECT_SOUND_H_INCLUDED

/* INCLUDES */
class survarium::game_object_static;
typedef vostok::intrusive_ptr<vostok::sound::sound_instance_proxy,vostok::sound::sound_instance_proxy,vostok::threading::single_threading_policy>
	vostok::sound::sound_instance_proxy_ptr;
typedef vostok::resources::resource_ptr<vostok::sound::sound_emitter,vostok::resources::unmanaged_intrusive_base>
	vostok::sound::sound_emitter_ptr;
class vostok::sound::sound_emitter;
class vostok::sound::sound_instance_proxy;

/* FORWARD REFS */
class boost::function<void __cdecl(survarium::game_object_ &)>;
class vostok::configs::binary_config_value;
class vostok::resources::queries_result;
class survarium::base_game_scene;

namespace survarium {

class object_sound : public game_object_static {
public:
			explicit	object_sound			( base_game_scene& w );
	virtual				~object_sound			( );

	virtual	void		load					(
							configs::binary_config_value const&		t,
							pcstr									__formal,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert					( ) override;
	virtual	void		remove					( ) override;

			void		on_sound_resources_ready(
							resources::queries_result&		data,
							boost::function< void( game_object_& ) >&	callback
						);

private:
	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	pcstr								m_sound_name;
	/* 0x0154 */	s32									m_sound_emitter_type;
	/* 0x0158 */	sound::sound_emitter_ptr			m_sound_emitter;
	/* 0x015c */	sound::sound_instance_proxy_ptr		m_sound_instance;
}; // class object_sound

STATIC_SIZE_ASSERT(object_sound, 0x160);

} // namespace survarium

#endif // #ifndef OBJECT_SOUND_H_INCLUDED
