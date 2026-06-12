////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_COOK_H_INCLUDED
#define PLAYER_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;
class survarium::inventory_cooker_data;
class survarium::player_creation_params;
class survarium::player_parameters_cooker_data;

namespace survarium {

class player_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
					player_cook				( );

	virtual	void	translate_query			( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource			( resources::resource_base* resource ) override;

			void	on_config_loaded		( resources::queries_result& data );
			void	on_hit_params_loaded	( resources::queries_result& data, player_creation_params* params );
			void	on_subresources_loaded	(
						resources::queries_result&			data,
						player_creation_params*				params,
						inventory_cooker_data*				inventory_cook_data,
						player_parameters_cooker_data*		player_parameters_cook_data
					);

	virtual			~player_cook			( ) { /* no source */ }
}; // class player_cook

STATIC_SIZE_ASSERT(player_cook, 0x20);

} // namespace survarium

#endif // #ifndef PLAYER_COOK_H_INCLUDED
