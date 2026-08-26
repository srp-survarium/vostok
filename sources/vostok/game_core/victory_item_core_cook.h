////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VICTORY_ITEM_CORE_COOK_H_INCLUDED
#define VICTORY_ITEM_CORE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

class victory_item_core;

class victory_item_core_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
	typedef resources::translate_query_cook super;

									victory_item_core_cook		( );

	virtual	victory_item_core*		create_resource				( );

	virtual	void					translate_query				( resources::query_result_for_cook& parent ) override;
	virtual	void					delete_resource				( resources::resource_base* resource ) override;

	virtual	void					on_config_loaded			( resources::queries_result& data );

}; // class victory_item_core_cook

STATIC_SIZE_ASSERT(victory_item_core_cook, 0x20);

} // namespace survarium

#endif // #ifndef VICTORY_ITEM_CORE_COOK_H_INCLUDED
