////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef BOOBY_TRAP_H_INCLUDED
#define BOOBY_TRAP_H_INCLUDED

#include <vostok/game_core/booby_trap_core.h>
#include <vostok/render/facade/model.h>
#include <vostok/resources_unmanaged_resource.h>


namespace survarium {

class game_world;

class booby_trap : public booby_trap_core {
	typedef booby_trap_core super;

private:
			explicit	booby_trap				( game_world& gw );
	virtual				~booby_trap				( );

public:
			void		on_trap_fired_message	( );
			void		on_trap_disarmed_message( );

private:
	virtual	void		defuse_completed		( ) override;

	virtual	void		switch_to_state			( booby_trap_state new_state ) override;

	virtual	void		register_tick			( scheduler& scheduler ) override;
	virtual	void		unregister_tick			( scheduler& scheduler ) override;

			void		on_new_state			( booby_trap_state old_state );

			void		play_fired_effects		( ) const;

	/* 0x0000 */	/* booby_trap_core */
	// PDB spells the models vostok::render::static_model_instance_ptr - the
	// same resource_ptr type our render tree typedefs as static_model_ptr
	/* 0x01b8 */	render::static_model_ptr				m_models[4];
	/* 0x01c8 */	resources::unmanaged_resource_ptr		m_particle_fired;
	/* 0x01cc */	resources::unmanaged_resource_ptr		m_sound_fired;
	/* 0x01d0 */	game_world&								m_game_world;
private:
	friend class booby_trap_cook;
}; // class booby_trap

STATIC_SIZE_ASSERT(booby_trap, 0x1D8);

} // namespace survarium

#endif // #ifndef BOOBY_TRAP_H_INCLUDED
