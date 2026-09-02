// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef WEAPON_AMMUNITION_COOK_H_INCLUDED
#define WEAPON_AMMUNITION_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

class weapon_ammunition_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
	typedef resources::translate_query_cook super;

	explicit		weapon_ammunition_cook		( );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource				( resources::resource_base* resource ) override;

private:
			void	on_config_ready				( resources::queries_result& data, resources::query_result_for_cook* parent );
}; // class weapon_ammunition_cook

STATIC_SIZE_ASSERT(weapon_ammunition_cook, 0x20);

} // namespace survarium

#endif // #ifndef WEAPON_AMMUNITION_COOK_H_INCLUDED
