////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "text_translator.h"
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/resources_query_result.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

 text_translator::text_translator( )
{
}

 text_translator::~text_translator( )
{
}

// claude@NOTE: line 29 STR_JOIN'd config path - the recorded tuples ctor is
// <char const*, char*, char const*>, i.e. STR_JOINA( path, "resources/localization/", <lang>,
// ... ); the middle char* is a runtime language string whose source is unknown from the
// available corpus, so the exact literals/byte image are not recoverable here. Structure
// (2 stmts: STR_JOIN+request build, then query_resources) reproduced.
void text_translator::load_text_localization( )
{
	resources::request requests[1] = { { NULL, resources::binary_config_class } };
	STR_JOINA							( requests[0].path, "resources/localization/", "", "" );	// claude@TODO: middle char* is an unknown runtime language string

	resources::query_resources(
		requests,
		boost::bind( &text_translator::on_texts_ready, this, _1 ),
		g_allocator
	);
}











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
