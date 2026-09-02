// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BOOBY_TRAP_SET_CORE_COOK_H_INCLUDED
#define BOOBY_TRAP_SET_CORE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include <vostok/game_core/booby_trap_set_cook_data.h>

namespace survarium {

class booby_trap_set_core;

class booby_trap_set_core_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
	typedef resources::translate_query_cook super;

	virtual	void					translate_query				( resources::query_result_for_cook& parent ) override;

private:
	virtual	booby_trap_set_core*	new_derived_resource		( ) = 0;
	virtual	u32						get_derived_resource_size	( ) = 0;

	virtual	void					query_for_derived_resources	(
										resources::query_result_for_cook*	parent,
										booby_trap_set_core*				resource,
										booby_trap_set_cook_data const&		cook_data,
										configs::binary_config_ptr			config
									) { VOSTOK_UNREFERENCED_PARAMETERS( cook_data, config ); finish_query( parent, resource ); }

protected:
									booby_trap_set_core_cook	( );

			void					finish_query				( resources::query_result_for_cook* parent, booby_trap_set_core* resource );

private:
			void					on_config_ready				( resources::queries_result& data, booby_trap_set_cook_data cook_data );
			void					on_subresources_loaded		(
										resources::queries_result&		data,
										booby_trap_set_core*			resource,
										booby_trap_set_cook_data		cook_data,
										configs::binary_config_ptr		config
									);
}; // class booby_trap_set_core_cook

STATIC_SIZE_ASSERT(booby_trap_set_core_cook, 0x20);

} // namespace survarium

#endif // #ifndef BOOBY_TRAP_SET_CORE_COOK_H_INCLUDED
