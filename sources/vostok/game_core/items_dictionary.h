// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ITEMS_DICTIONARY_H_INCLUDED
#define ITEMS_DICTIONARY_H_INCLUDED

#include <vostok/game_core/dictionary_item.h>

namespace survarium {

class items_dictionary_cook;

class items_dictionary : public resources::unmanaged_resource , private boost::noncopyable {
public:
	inline	dictionary_item const&				item_by_id					( u32 item_dictionary_id ) const
	{
		return m_items_dict.find( item_dictionary_id )->second;
	}

	inline	void								add_item_desc				( dictionary_item const& item ) { m_items_dict[item.item_id] = item; }
	inline	map< u32, dictionary_item > const&	get_dictionary				( ) const						{ return m_items_dict; }

		/* 0x0108 */	configs::binary_config_ptr				dict_config;
	private:
		/* 0x010c */	mutable map< u32, dictionary_item >		m_items_dict;
private:
	friend class items_dictionary_cook;
}; // class items_dictionary

STATIC_SIZE_ASSERT(items_dictionary, 0x128);

} // namespace survarium

#endif // #ifndef ITEMS_DICTIONARY_H_INCLUDED
