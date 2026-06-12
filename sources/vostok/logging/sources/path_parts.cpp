////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "path_parts.h"

using vostok::logging::path_parts;
using vostok::logging::format_string_type;

path_parts::path_parts	(pcstr initiator) : m_index(0), m_current_element(0)
{
	ASSERT									( initiator );
	if ( * initiator != initiator_separator )
		add_part							( initiator );
	add_part								(0);
}

void path_parts::add_part		( pcstr const part )
{
	ASSERT					( !part || *part );
	if ( m_parts.empty() )
		m_current_element	= part;

	m_parts.push_back		( part );
}

void path_parts::to_next_element()
{
	m_current_element		= strchr( m_current_element, initiator_separator );
	if ( m_current_element && m_current_element[1] ) {
		++m_current_element; // next element in this part
		return;
	}

	m_current_element		= m_parts[++m_index]; // next part of path
}

void path_parts::concat2buffer( format_string_type& buffer )
{
	u32 string_length = 0;
	buffer[0] = '\0';
	for ( u32 i = 0 ; m_parts[i] ; ++i ) {
		u32 part_length = strings::length( m_parts[i] );
		memory::copy( &buffer[string_length], 512 - string_length, m_parts[i], part_length + 1 );
		string_length += part_length;
	}
	if ( string_length && buffer[string_length - 1] == ':' )
		buffer[string_length - 1] = '\0';
}