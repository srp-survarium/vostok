////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_PARAMETERS_MODIFYER_COOK_H_INCLUDED
#define PLAYER_PARAMETERS_MODIFYER_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

// sushi@TODO: This was `player_parameters_cook.h`

namespace survarium {

class player_parameters_modifyer_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
	typedef resources::translate_query_cook super;

					player_parameters_modifyer_cook	( );

	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource					( resources::resource_base* resource ) override;
}; // class player_parameters_modifyer_cook

STATIC_SIZE_ASSERT(player_parameters_modifyer_cook, 0x20);

} // namespace survarium

#endif // #ifndef PLAYER_PARAMETERS_MODIFYER_COOK_H_INCLUDED
