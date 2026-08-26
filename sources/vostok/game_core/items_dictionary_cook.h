////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ITEMS_DICTIONARY_COOK_H_INCLUDED
#define ITEMS_DICTIONARY_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

class items_dictionary;

class items_dictionary_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
	typedef resources::translate_query_cook super;

					items_dictionary_cook				( );

	virtual	void	translate_query						( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource						( resources::resource_base* resource ) override;

private:
			void	on_items_dictionary_config_loaded	( resources::queries_result& data );
			void	on_subresources_loaded				( resources::queries_result& data, items_dictionary* cooked_resource, u32* item_dict_ids );
}; // class items_dictionary_cook

STATIC_SIZE_ASSERT(items_dictionary_cook, 0x20);

} // namespace survarium

#endif // #ifndef ITEMS_DICTIONARY_COOK_H_INCLUDED
