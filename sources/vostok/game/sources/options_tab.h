// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPTIONS_TAB_H_INCLUDED
#define OPTIONS_TAB_H_INCLUDED

#include <vostok/scaleform/sources/flash_movie_resource.h>
#include "options_enum.h"

namespace survarium {

class game;
class options_item_base;

class options_tab : private boost::noncopyable {
public:
											options_tab		( game& g, flash_movie_resource_ptr& movie, options_enum type );
											~options_tab	( );

			void							apply			( flash_movie_resource_ptr& movie );

			void							revert			( flash_movie_resource_ptr& movie );

			void							initialize_data	( flash_movie_resource_ptr& movie );

	inline	options_item_base*				option_by_id	( u8 arg_0 ) { return m_options[ arg_0 ]; }

	inline	options_enum					type			( ) { /* no source */ return m_type; }

	inline	game&							get_game		( ) { /* no source */ return m_game; }
	inline	flash_movie_resource_ptr&		get_movie		( ) { /* no source */ return m_movie; }

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	options_item_base**				m_options;
	/* 0x0004 */	u8								m_options_count;
	/* 0x0008 */	options_enum					m_type;
	/* 0x000c */	game&							m_game;
	/* 0x0010 */	flash_movie_resource_ptr&		m_movie;
}; // class options_tab

STATIC_SIZE_ASSERT(options_tab, 0x14);

} // namespace survarium

#endif // #ifndef OPTIONS_TAB_H_INCLUDED
