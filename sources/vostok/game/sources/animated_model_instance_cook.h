////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATED_MODEL_INSTANCE_COOK_H_INCLUDED
#define ANIMATED_MODEL_INSTANCE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

struct animated_model_instance;

class animated_model_instance_cook : public resources::translate_query_cook {
	typedef	resources::translate_query_cook	super;

public:
					animated_model_instance_cook	( );

	virtual	void	translate_request_path			( pcstr request, fs_new::virtual_path_string& new_request ) const override;
	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource					( resources::resource_base* resource ) override;

private:
			void	on_config_loaded				( resources::queries_result& data );
			void	on_subresources_loaded			( resources::queries_result& data );
			void	on_hit_params_loaded			( resources::queries_result& data, animated_model_instance* new_model );

}; // class animated_model_instance_cook

STATIC_SIZE_ASSERT(animated_model_instance_cook, 0x20);

} // namespace survarium

#endif // #ifndef ANIMATED_MODEL_INSTANCE_COOK_H_INCLUDED
