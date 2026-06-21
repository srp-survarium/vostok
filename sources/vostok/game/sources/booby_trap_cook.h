////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef BOOBY_TRAP_COOK_H_INCLUDED
#define BOOBY_TRAP_COOK_H_INCLUDED

#include <vostok/game_core/booby_trap_core_cook.h>

namespace survarium {

class booby_trap;
class game_world;

// temp_include_all.cpp's use_booby_trap_cook anchor defines FUNCTION-LOCAL
// stand-in classes with this name - different scope, no clash
class booby_trap_cook : public booby_trap_core_cook {
public:
			explicit			booby_trap_cook				( game_world& gw );

	virtual						~booby_trap_cook			( ) { /* no source */ }

private:
	// STATE[STUB]
	virtual	booby_trap_core*	new_derived_resource		( ) override
	{
		return NULL;

		// FUNCTION BODY[0x97e40]
		// <0x97e40>|0x000|      :'25'	{
		// ******
	}

	// STATE[STUB]
	virtual	u32					get_derived_resource_size	( ) override
	{
		return 0;

		// FUNCTION BODY[0x97e70]
		// <0x97e70>|0x000|      :'26'	{
		// ******
	}

	virtual	void				query_for_derived_resources	(
									resources::query_result_for_cook*		parent,
									booby_trap_core*						resource,
									configs::binary_config_ptr				config
								) override;

			void				on_models_ready				( resources::queries_result& data, booby_trap* game_resource );

private:
	/* 0x0000 */	/* booby_trap_core_cook */
	/* 0x0020 */	game_world&		m_game_world;
}; // class booby_trap_cook

STATIC_SIZE_ASSERT(booby_trap_cook, 0x24);

} // namespace survarium

#endif // #ifndef BOOBY_TRAP_COOK_H_INCLUDED
