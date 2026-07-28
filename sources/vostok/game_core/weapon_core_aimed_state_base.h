////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_AIMED_STATE_BASE_H_INCLUDED
#define WEAPON_CORE_AIMED_STATE_BASE_H_INCLUDED

#include <vostok/game_core/weapon_core_base_state.h>

namespace vostok { void use_game_core_weapon_core_aimed_state_base( ); }

namespace survarium {

class weapon_core_aimed_state_base : public weapon_core_base_state {
protected:
			explicit	weapon_core_aimed_state_base	( weapon_core& weapon );

protected:
	virtual	void		initialize						( ) override;
	virtual	void		finalize						( ) override;

	// temp_include_all.cpp anchor; calls the protected virtuals non-virtually to
	// keep their out-of-line symbols. Friend decls emit no bytes.
	friend void ::vostok::use_game_core_weapon_core_aimed_state_base( );
}; // class weapon_core_aimed_state_base

STATIC_SIZE_ASSERT(weapon_core_aimed_state_base, 0x138);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_AIMED_STATE_BASE_H_INCLUDED
