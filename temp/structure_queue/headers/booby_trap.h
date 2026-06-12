////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef BOOBY_TRAP_H_INCLUDED
#define BOOBY_TRAP_H_INCLUDED

/* INCLUDES */
class survarium::booby_trap_core;
class survarium::game_world;
class survarium::booby_trap_state;
typedef vostok::resources::resource_ptr<vostok::render::static_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::static_model_instance_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::render::static_model_instance;
class vostok::resources::unmanaged_resource;

/* FORWARD REFS */
class survarium::scheduler;

namespace survarium {

class booby_trap : public booby_trap_core {
public:
			explicit	booby_trap				( game_world& gw );
	virtual				~booby_trap				( );

			void		on_trap_fired_message	( );
			void		on_trap_disarmed_message( );

	virtual	void		defuse_completed		( ) override;

	virtual	void		switch_to_state			( booby_trap_state new_state ) override;

	virtual	void		register_tick			( scheduler& scheduler ) override;
	virtual	void		unregister_tick			( scheduler& scheduler ) override;

			void		on_new_state			( booby_trap_state old_state );

			void		play_fired_effects		( ) const;

private:
	/* 0x0000 */	/* booby_trap_core */
	/* 0x01b8 */	render::static_model_instance_ptr		m_models[4];
	/* 0x01c8 */	resources::unmanaged_resource_ptr		m_particle_fired;
	/* 0x01cc */	resources::unmanaged_resource_ptr		m_sound_fired;
	/* 0x01d0 */	game_world&								m_game_world;
}; // class booby_trap

STATIC_SIZE_ASSERT(booby_trap, 0x1D8);

} // namespace survarium

#endif // #ifndef BOOBY_TRAP_H_INCLUDED
