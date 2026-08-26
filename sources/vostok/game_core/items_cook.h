////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ITEMS_COOK_H_INCLUDED
#define ITEMS_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include <vostok/game_core/item_types_enum.h>

namespace survarium {

class items_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
	typedef resources::translate_query_cook super;

					items_cook					( );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource				( resources::resource_base* resource ) override;

private:
			void	create_item_and_finish_query( item_types_enum item_type, configs::binary_config_ptr config, resources::query_result_for_cook* parent );
			void	on_config_ready				( resources::queries_result& data, resources::query_result_for_cook* parent );

}; // class items_cook

STATIC_SIZE_ASSERT(items_cook, 0x20);

} // namespace survarium

#endif // #ifndef ITEMS_COOK_H_INCLUDED
