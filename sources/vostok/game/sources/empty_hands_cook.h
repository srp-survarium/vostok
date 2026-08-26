////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef EMPTY_HANDS_COOK_H_INCLUDED
#define EMPTY_HANDS_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

class empty_hands_cook : public resources::translate_query_cook , private boost::noncopyable {
	typedef	resources::translate_query_cook	super;

public:
					empty_hands_cook				( );

private:
	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;

private:

	virtual	void	delete_resource					( resources::resource_base* resource ) override;

			void	on_empty_hands_config_loaded	( resources::queries_result& data );
			void	on_empty_hands_animations_loaded( resources::queries_result& data );

}; // class empty_hands_cook

STATIC_SIZE_ASSERT(empty_hands_cook, 0x20);

} // namespace survarium

#endif // #ifndef EMPTY_HANDS_COOK_H_INCLUDED
