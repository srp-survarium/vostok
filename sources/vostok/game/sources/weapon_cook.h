// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef WEAPON_COOK_H_INCLUDED
#define WEAPON_COOK_H_INCLUDED

#include <vostok/game_core/weapon_core_cook.h>


namespace survarium {

class game;
class weapon_core;

class weapon_cook : public weapon_core_cook {
public:
	static	void		register_cooks_for_logic_states( );

			explicit	weapon_cook					( game& g );

	// these methods mangle private (AAE / EAE / EBE) in the shipped PDB; declaring
	// them private keeps each out-of-line symbol pairable with the target
private:
			void		on_weapon_subresources_ready(
							resources::queries_result&		data,
							configs::binary_config_ptr		config_ptr,
							weapon_core*					object_to_cook
						);
	virtual	void		on_weapon_config_loaded		( resources::queries_result& data ) override;

	virtual	void		delete_resource				( resources::resource_base* resource ) override;

	virtual	u32			cooked_object_size			( weapon_core& object_to_cook ) const override;

public:
	typedef	weapon_core_cook	super;

private:
	/* 0x0000 */	/* weapon_core_cook */
	/* 0x0020 */	game&		m_game;
}; // class weapon_cook

STATIC_SIZE_ASSERT(weapon_cook, 0x24);

} // namespace survarium

#endif // #ifndef WEAPON_COOK_H_INCLUDED
