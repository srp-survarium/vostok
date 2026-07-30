#include "pch.h"
#include <vostok/math_int2.h>
#include <vostok/render/core/options.h>

namespace vostok {
namespace render {

render_cc::render_cc(
	pcstr define_name,
	enum_options_changes_result changed_result
)
	: render_next( 0 ),
	  m_define_name( define_name ),
	  m_changes_result( changed_result )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55cdc0]
}

render_cc_bool::render_cc_bool(
	pcstr name,
	enum_options_changes_result changed_result,
	pcstr define_name,
	bool& value,
	bool& prev_value,
	bool serializable,
	console_commands::command_type command_type
)
	: render_cc( define_name, changed_result ),
	  console_commands::cc_bool(
		name,
		value,
		serializable,
		command_type
	  ),
	  m_prev_value( prev_value )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b550]
}

void render_cc_bool::execute( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b5b0]
}

bool render_cc_bool::is_changed( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b5c0]
	return false;
}

bool render_cc_bool::fill_macro( shader_macro& ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b5d0]
	return false;
}

render_cc_float::render_cc_float(
	pcstr name,
	enum_options_changes_result changed_result,
	pcstr define_name,
	float& value,
	float& prev_value,
	float min,
	float max,
	bool serializable,
	console_commands::command_type command_type
)
	: render_cc( define_name, changed_result ),
	  console_commands::cc_float(
		name,
		value,
		min,
		max,
		serializable,
		command_type
	  ),
	  m_prev_value( prev_value )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b630]
}

void render_cc_float::execute( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b690]
}

bool render_cc_float::is_changed( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b6a0]
	return false;
}

bool render_cc_float::fill_macro( shader_macro& ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b6c0]
	return false;
}

render_cc_u32::render_cc_u32(
	pcstr name,
	enum_options_changes_result changed_result,
	pcstr define_name,
	u32& value,
	u32& prev_value,
	u32 min,
	u32 max,
	bool serializable,
	console_commands::command_type command_type
)
	: render_cc( define_name, changed_result ),
	  console_commands::cc_u32(
		name,
		value,
		min,
		max,
		serializable,
		command_type
	  ),
	  m_prev_value( prev_value )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b720]
}

void render_cc_u32::execute( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b780]
}

bool render_cc_u32::is_changed( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b790]
	return false;
}

bool render_cc_u32::fill_macro( shader_macro& ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b7a0]
	return false;
}

options::options( )
	: first_render_command( 0 ),
	  first_command( 0 ),
	  last_command( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f0a0]
}

void options::set_default_values( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55c9d0]
}

math::uint2 parse_resolution( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55ce00]
	return math::uint2( 1, 1 );
}

void options::register_console_commands( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55cfa0]
}

void options::begin_render_options_changing( long volatile* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55cd50]
}

enum_options_changes_result options::end_render_options_changing(
	vector<fs_new::virtual_path_string>&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f180]
	return ocr_need_nothing;
}

void options::on_config_loaded( resources::queries_result& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f350]
}

bool is_line_term( char )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55c9b0]
	return false;
}

u32 advance_term_string( memory::reader& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55cd60]
	return 0;
}

void r_string( memory::reader&, char (&)[4096] )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55cde0]
}

void options::load_impl( memory::reader& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f300]
}

void options::fill_global_macros( shader_defines_list& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f0d0]
}

void options::load_from_config( configs::binary_config_value const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55ce80]
}

} // namespace render
} // namespace vostok
