////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_ANALYSIS_RESULT_COOK_H_INCLUDED
#define ANIMATION_ANALYSIS_RESULT_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

class animation_analysis_result_cook : public resources::translate_query_cook {
public:
	typedef resources::translate_query_cook super;

					animation_analysis_result_cook	( );

	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource					( resources::resource_base* resource ) override;
}; // class animation_analysis_result_cook

STATIC_SIZE_ASSERT(animation_analysis_result_cook, 0x20);

} // namespace survarium

#endif // #ifndef ANIMATION_ANALYSIS_RESULT_COOK_H_INCLUDED
