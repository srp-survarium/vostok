////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef BOOBY_TRAP_SET_H_INCLUDED
#define BOOBY_TRAP_SET_H_INCLUDED

#include <vostok/game_core/booby_trap_set_core.h>
#include <vostok/game_core/player_death_subscriber.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/render/facade/model.h>


namespace survarium {

class game_world;

class booby_trap_set : public booby_trap_set_core {
	friend class booby_trap_set_cook;
public:
			void									on_trap_placed_message		( u8 index, float3 const& position, float3 const& angles );
			void									on_trap_removed_message		( u8 index );
			void									on_trap_fired_message		( u8 index );
			void									on_trap_disarmed_message	( u8 index );

private:
	virtual	game_material_manager const&			get_game_material_manager	( ) override;

	virtual	void									action						( bool key_down ) override;

	virtual	void									remove						( ) override;

	virtual	void									holder_assigned				( ) override;
	virtual	void									holder_removed				( ) override;

			explicit								booby_trap_set				( game_world& game_world );
	virtual											~booby_trap_set				( ) { /* no source */ }

			void									tick						( const u32 time_delta_ms, const u32 current_time_ms );

			void									on_player_death				( );

	// target keeps the tick()/action() call sites out-of-line (push 0; call), so the
	// base must not inline the small body here
	__declspec( noinline )
			void									toggle_ghost_model			( bool enable );
	// PDB spells the models vostok::render::static_model_instance_ptr - the
	// same resource_ptr type our render tree typedefs as static_model_ptr
			render::static_model_ptr				pick_ghost_model			( bool is_placing_allowed );
			bool									pick_current_ghost_model	( float4x4 const& transform, bool is_placing_allowed );
			void									remove_current_ghost_model	( );

	inline	booby_trap_set*							return_this					( ) { /* no source */ return this; }

private:
	typedef booby_trap_set_core super;

	/* 0x0000 */	/* booby_trap_set_core */
	/* 0x0148 */	player_death_subscriber					m_player_death_subscriber;
	/* 0x0170 */	scheduler::identifier					m_scheduler_identifier;
	/* 0x0174 */	render::static_model_ptr				m_model_ghost_allowed;
	/* 0x0178 */	render::static_model_ptr				m_model_ghost_denied;
	/* 0x017c */	render::static_model_ptr				m_current_rendering_model;
	/* 0x0180 */	game_world&								m_game_world;
}; // class booby_trap_set

STATIC_SIZE_ASSERT(booby_trap_set, 0x188);

} // namespace survarium

#endif // #ifndef BOOBY_TRAP_SET_H_INCLUDED
