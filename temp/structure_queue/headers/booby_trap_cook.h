////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef BOOBY_TRAP_COOK_H_INCLUDED
#define BOOBY_TRAP_COOK_H_INCLUDED

/* INCLUDES */
class survarium::booby_trap_core_cook;
class survarium::game_world;
class vostok::configs::binary_config;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class survarium::booby_trap;
class survarium::booby_trap_core;

namespace survarium {

class booby_trap_cook : public booby_trap_core_cook {
public:
			explicit			booby_trap_cook				( game_world& gw );

	virtual	booby_trap_core*	new_derived_resource		( ) override { /* no source */ }

	virtual	u32					get_derived_resource_size	( ) override { /* no source */ }

	virtual	void				query_for_derived_resources	(
									resources::query_result_for_cook*		parent,
									booby_trap_core*						resource,
									configs::binary_config_ptr				config
								) override;

			void				on_models_ready				( resources::queries_result& data, booby_trap* game_resource );

	virtual						~booby_trap_cook			( ) { /* no source */ }

private:
	/* 0x0000 */	/* booby_trap_core_cook */
	/* 0x0020 */	game_world&		m_game_world;
}; // class booby_trap_cook

STATIC_SIZE_ASSERT(booby_trap_cook, 0x24);

} // namespace survarium

#endif // #ifndef BOOBY_TRAP_COOK_H_INCLUDED
