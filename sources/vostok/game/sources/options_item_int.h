////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OPTIONS_ITEM_INT_H_INCLUDED
#define OPTIONS_ITEM_INT_H_INCLUDED

#include "options_item_base.h"

namespace survarium {

class options_item_int : public options_item_base {
	typedef options_item_base super;

public:
					options_item_int	(
						options_tab&	parent_tab,
						pcstr			console_command,
						u8				option_item_id,
						pcstr*			values,
						u8				values_count
					);

	virtual	void	initialize			( ) override;

	virtual	void	fill_data			( flash_value& val ) override;
	virtual	void	fill_value			( flash_value& val ) override;

	virtual	void	apply				( ) override;

	virtual	void	revert				( ) override;

	virtual	void	call				( flash_function_handler_params& params ) override;


protected:
	/* 0x0000 */	/* options_item_base */
	/* 0x0018 */	pcstr*		m_values;
	/* 0x001c */	u8			m_values_count;
	/* 0x001d */	u8			m_current_value;
	/* 0x001e */	u8			m_source_value;
}; // class options_item_int

STATIC_SIZE_ASSERT(options_item_int, 0x20);

} // namespace survarium

#endif // #ifndef OPTIONS_ITEM_INT_H_INCLUDED
