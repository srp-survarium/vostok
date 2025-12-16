////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_MODEL_COOK_H_INCLUDED
#define DAMAGE_MODEL_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

class damage_model_cook : public resources::translate_query_cook {
public:
					damage_model_cook			( );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource				( resources::resource_base* resource ) override;

private:
			void	on_hit_params_received		( resources::queries_result& data );


	static	pcstr	get_current_hit_type		( ) { /* no source */ }


}; // class damage_model_cook

STATIC_SIZE_ASSERT(damage_model_cook, 0x20);

} // namespace survarium

#endif // #ifndef DAMAGE_MODEL_COOK_H_INCLUDED
