// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPTIONS_MONITOR_INDEX_SELECTOR_H_INCLUDED
#define OPTIONS_MONITOR_INDEX_SELECTOR_H_INCLUDED

#include "options_item_int.h"
#include "game_memory.h"

namespace survarium {

class options_monitor_index_selector : public options_item_int {
	typedef options_item_int super;

public:
			explicit	options_monitor_index_selector	( options_tab& parent_tab );
	// sushi@TODO: Owned pointer-array cleanup model; recover direct typed destruction and original guard.
	inline				~options_monitor_index_selector	( )
	{
		if ( m_values )
			VOSTOK_DELETE_ARRAY_IMPL( ::survarium::g_allocator, m_values );
	}

	virtual	void		call							( flash_function_handler_params& params ) override;

	virtual	void		revert							( ) override;

			void		refill_resolutions_data			( );

private:
	/* 0x0000 */	/* options_item_int */
	/* 0x0020 */	fixed_string< 32 >		m_cached_monitors_names[6];
}; // class options_monitor_index_selector

STATIC_SIZE_ASSERT(options_monitor_index_selector, 0x128);

} // namespace survarium

#endif // #ifndef OPTIONS_MONITOR_INDEX_SELECTOR_H_INCLUDED
