////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ITEMS_DICTIONARY_H_INCLUDED
#define ITEMS_DICTIONARY_H_INCLUDED

#include <vostok/game_core/dictionary_item.h>

namespace survarium {

class items_dictionary : public resources::unmanaged_resource , public boost::noncopyable {
public:
	inline	dictionary_item const&				item_by_id					( u32 id ) const { /* no source */ }
	inline	void								add_item_desc				( dictionary_item const& item ) { /* no source */ }
	inline	map< u32, dictionary_item > const&	get_dictionary				( ) const { /* no source */ }

												items_dictionary			( );
private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* boost::noncopyable */
	/* 0x0108 */	configs::binary_config_ptr		dict_config;
	/* 0x010c */	map< u32, dictionary_item >		m_items_dict;
}; // class items_dictionary

STATIC_SIZE_ASSERT(items_dictionary, 0x128);

} // namespace survarium

#endif // #ifndef ITEMS_DICTIONARY_H_INCLUDED
