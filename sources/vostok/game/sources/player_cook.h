////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_COOK_H_INCLUDED
#define PLAYER_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

struct inventory_cooker_data;
struct player_creation_params;
struct player_parameters_cooker_data;

class player_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
					player_cook				( );

	virtual	void	translate_query			( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource			( resources::resource_base* resource ) override;

private:
			void	on_config_loaded		( resources::queries_result& data );
			void	on_hit_params_loaded	( resources::queries_result& data, player_creation_params* params );
			void	on_subresources_loaded	(
						resources::queries_result&			data,
						player_creation_params*				params,
						inventory_cooker_data*				inventory_cook_data,
						player_parameters_cooker_data*		player_parameters_cook_data
					);

public:
	virtual			~player_cook			( ) { /* no source */ }

	// Lets the /OPT:REF reachability anchor address-take the private callbacks.
	friend	void	use_game_player_cook	( );
}; // class player_cook

STATIC_SIZE_ASSERT(player_cook, 0x20);

} // namespace survarium

#endif // #ifndef PLAYER_COOK_H_INCLUDED
