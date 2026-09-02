// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef INVENTORY_COOK_H_INCLUDED
#define INVENTORY_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

struct inventory_cooker_data;

class inventory_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
	typedef resources::translate_query_cook super;

					inventory_cook				( );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource				( resources::resource_base* resource ) override;

private:
			void	on_subresources_loaded		( resources::queries_result& data, inventory_cooker_data* cooker_data );

}; // class inventory_cook

STATIC_SIZE_ASSERT(inventory_cook, 0x20);

} // namespace survarium

#endif // #ifndef INVENTORY_COOK_H_INCLUDED
