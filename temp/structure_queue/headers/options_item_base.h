////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OPTIONS_ITEM_BASE_H_INCLUDED
#define OPTIONS_ITEM_BASE_H_INCLUDED

/* INCLUDES */
class vostok::console_commands::console_command;
class survarium::options_tab;
struct survarium::flash_function_handler;
enum survarium::option_item_type_enum;

/* FORWARD REFS */
class survarium::flash_function_handler_params;
class survarium::flash_value;

namespace survarium {

enum survarium::option_item_type_enum
{
	bool_selector		= 0x0,
	string_selector		= 0x1,
	slider_selector		= 0x2,
};

class options_item_base : public flash_function_handler , public boost::noncopyable {
public:
										options_item_base	(
											options_tab&				parent_tab,
											pcstr						console_command,
											u8							option_item_id,
											option_item_type_enum		type
										);

	virtual	void						initialize			( ) = 0;

	virtual	void						fill_data			( flash_value& arg_0 ) = 0;
	virtual	void						fill_value			( flash_value& arg_0 ) = 0;

	virtual	void						apply				( ) = 0;

	virtual	void						revert				( );

	virtual	void						call				( flash_function_handler_params& arg_0 ) override = 0;

	inline	option_item_type_enum		option_type			( ) { /* no source */ }

	inline								~options_item_base	( ) { /* no source */ }

private:
	/* 0x0000 */	/* flash_function_handler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	console_commands::console_command*		m_console_command;
	/* 0x000c */	option_item_type_enum					m_type;
	/* 0x0010 */	options_tab&							m_parent_tab;
	/* 0x0014 */	u8										m_option_item_id;
}; // class options_item_base

STATIC_SIZE_ASSERT(options_item_base, 0x18);

} // namespace survarium

#endif // #ifndef OPTIONS_ITEM_BASE_H_INCLUDED
