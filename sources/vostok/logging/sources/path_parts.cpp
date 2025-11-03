////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "path_parts.h"

using vostok::logging::path_parts;
using vostok::logging::Buffer512;

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

// sushi@TODO: To delete
void path_parts::concat2buffer(vostok::buffer_string& buffer)
{
	int cur_index			= 0;

	while ( m_parts[cur_index] ) {
		buffer				+= m_parts[cur_index];
		++cur_index;
	}

	if ( buffer.length() && *buffer.rbegin() == initiator_separator )
		buffer.set_length	( buffer.length() - 1 );
}

// STATE[STUB]
// void vostok::logging::path_parts::concat2buffer(char[512]&)
void path_parts::concat2buffer( Buffer512& buffer )
{
	// LOCALS
	// u32 							string_length
	// u32 							i<1>
	// u32 							part_length<2>
	// ******

	// FUNCTION BODY
	// <0x70dab9>|0x000|0x000:'55'
	// <0x70dac0>|0x007|0x007:'56'
	// <0x70dac6>|0x00d|0x006|[1]:'57'
	// <0x70daf2>|0x039|0x02c|[2]:'58'
	// <0x70db19>|0x060|0x027:'59'
	// <0x70db50>|0x097|0x037:'60'
	// <0x70db59>|0x0a0|0x009:'61'
	// <1>
	// <0x70db5e>|0x0a5|0x005:'63'
	// <0x70db73>|0x0ba|0x015:'64'
	// ******
}