// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LADDER_COOK_H_INCLUDED
#define LADDER_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

class vostok::resources::translate_query_cook;

class ladder_cook : public resources::translate_query_cook {
public:
					ladder_cook					( );
	virtual			~ladder_cook				( );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource				( resources::resource_base* resource ) override;

private:
			void	on_animations_loaded		( resources::queries_result& data, configs::binary_config_value const& config );
}; // class ladder_cook

STATIC_SIZE_ASSERT(ladder_cook, 0x20);

} // namespace survarium

#endif // #ifndef LADDER_COOK_H_INCLUDED
