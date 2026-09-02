// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "text_translator.h"
#include <vostok/console_command.h>
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>
static char s_localization_str[16] = "russian";
static console_commands::cc_string s_localization( "localization", s_localization_str, 16, true, console_commands::command_type_user_specific );

namespace survarium {

 text_translator::text_translator( )
{
}

 text_translator::~text_translator( )
{
}

// The console-command buffer is also the localization path component.
void text_translator::load_text_localization( )
{
	pstr path;
	STR_JOINA							( path, "resources/localization/", s_localization_str, "" );
	resources::request requests[1]	= { { path, resources::binary_config_class } };

	resources::query_resources(
		requests,
		boost::bind( &text_translator::on_texts_ready, this, _1 ),
		g_allocator

	);
}
#line 50
void text_translator::translate_text( pcstr text_id, wchar_t* translated_text )
{
	size_t converted_chars_count;
	pcstr translated;
	if ( !m_text_data->get_root( )["strings"].value_exists( text_id ) )
	{
		LOG_INFO							( "There is no available localization for [%s] !!!", text_id );
		translated						= text_id;
	}
	else
		translated						= m_text_data->get_root( )["strings"][text_id];

	converted_chars_count				= 0;
	mbstowcs_s							( &converted_chars_count, translated_text, 0x200, translated, _TRUNCATE );
}

void text_translator::on_texts_ready( resources::queries_result& data )
{
	m_text_data							= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
}

} // namespace survarium
