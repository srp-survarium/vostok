// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BOOBY_TRAP_CORE_COOK_H_INCLUDED
#define BOOBY_TRAP_CORE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include <vostok/game_core/booby_trap_core.h>

namespace survarium {

class booby_trap_core_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
	typedef resources::translate_query_cook super;

			explicit			booby_trap_core_cook		( );

	virtual	void				translate_query				( resources::query_result_for_cook& parent ) override;
	virtual	void				delete_resource				( resources::resource_base* resource ) override {
		VOSTOK_DELETE_IMPL( g_allocator, resource );
	}

			void				on_subresources_loaded		( resources::queries_result& data, configs::binary_config_ptr config );

private:
	virtual	booby_trap_core*	new_derived_resource		( ) { return VOSTOK_NEW_IMPL( g_allocator, booby_trap_core ); }
	virtual	u32					get_derived_resource_size	( ) { return sizeof( booby_trap_core ); }
	virtual	void				query_for_derived_resources	( resources::query_result_for_cook* parent, booby_trap_core* resource, configs::binary_config_ptr config ) {
		VOSTOK_UNREFERENCED_PARAMETER( config );
		finish_query( parent, resource );
	}

protected:
			void				finish_query				( resources::query_result_for_cook* parent, booby_trap_core* resource );
}; // class booby_trap_core_cook

STATIC_SIZE_ASSERT(booby_trap_core_cook, 0x20);

} // namespace survarium

#endif // #ifndef BOOBY_TRAP_CORE_COOK_H_INCLUDED
