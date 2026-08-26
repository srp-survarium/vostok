////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OPTIONS_ITEM_FLOAT_H_INCLUDED
#define OPTIONS_ITEM_FLOAT_H_INCLUDED

#include "options_item_base.h"

namespace survarium {

class options_item_float : public options_item_base {
	typedef options_item_base super;

public:
					options_item_float	(
						options_tab&	parent_tab,
						pcstr			console_command,
						u8				option_item_id,
						float			step
					);

	virtual	void	initialize			( ) override;

	virtual	void	fill_data			( flash_value& val ) override;
	virtual	void	fill_value			( flash_value& val ) override;

	virtual	void	apply				( ) override;

	virtual	void	revert				( ) override;

	virtual	void	call				( flash_function_handler_params& params ) override;


protected:
	/* 0x0000 */	/* options_item_base */
	/* 0x0018 */	float		m_step;
	/* 0x001c */	float		m_current_value;
	/* 0x0020 */	float		m_source_value;
}; // class options_item_float

STATIC_SIZE_ASSERT(options_item_float, 0x24);

} // namespace survarium

#endif // #ifndef OPTIONS_ITEM_FLOAT_H_INCLUDED
