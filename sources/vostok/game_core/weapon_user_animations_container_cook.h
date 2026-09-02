// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_USER_ANIMATIONS_CONTAINER_COOK_H_INCLUDED
#define WEAPON_USER_ANIMATIONS_CONTAINER_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

class weapon_user_animations_container_cook : public resources::translate_query_cook , public core::noncopyable {
public:
	explicit		weapon_user_animations_container_cook( );

public:
	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource					( resources::resource_base* resource ) override;

private:
			void	on_config_loaded				( resources::queries_result& data );
			void	on_animations_loaded			( resources::queries_result& data );

	typedef resources::translate_query_cook super;
}; // class weapon_user_animations_container_cook

STATIC_SIZE_ASSERT(weapon_user_animations_container_cook, 0x20);

} // namespace survarium

#endif // #ifndef WEAPON_USER_ANIMATIONS_CONTAINER_COOK_H_INCLUDED
