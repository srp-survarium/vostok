////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_HIDE_STATE_BASE_H_INCLUDED
#define WEAPON_CORE_HIDE_STATE_BASE_H_INCLUDED

#include <vostok/game_core/weapon_core_animation_end_aware_state.h>

namespace vostok { void use_game_core_weapon_core_hide_state_base( ); }

namespace survarium {

class weapon_core_hide_state_base : public weapon_core_animation_end_aware_state {
protected:
	// ctor mangles ??0...@@IAE@... -> protected (objdiff pairs by mangled name)
	explicit		weapon_core_hide_state_base	( weapon_core& weapon, bool& is_shown );

	// initialize/finalize mangle ...@@MAEXXZ -> protected virtual
	virtual	void	initialize					( ) override;
	virtual	void	finalize					( ) override;

public:
	virtual	bool	is_ready_for_transition		( ) const override { return has_animation_ended( ); }

private:
	// on_animation_end_impl mangles ...@@EAEXAA_N@Z -> private virtual
	virtual	void	on_animation_end_impl		( bool& animation_player_tick_result ) override;

	// temp_include_all.cpp anchor; needs to call the protected/private virtuals
	// non-virtually to keep their out-of-line symbols. Friend decls emit no bytes.
	friend void ::vostok::use_game_core_weapon_core_hide_state_base( );

private:
	/* 0x0000 */	/* weapon_core_animation_end_aware_state */
	/* 0x0140 */	bool&		m_is_shown;
}; // class weapon_core_hide_state_base

STATIC_SIZE_ASSERT(weapon_core_hide_state_base, 0x148);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_HIDE_STATE_BASE_H_INCLUDED
