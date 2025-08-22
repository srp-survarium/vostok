////////////////////////////////////////////////////////////////////////////
//	Created		: 01.03.2010
//	Author		: Alexander Maniluk
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef XSP_LAN_ADDRESSES_SYNC_DEFINES_HPP_INCLUDED
#define XSP_LAN_ADDRESSES_SYNC_DEFINES_HPP_INCLUDED

#include <vostok/network/sources/writers.h>
#include <vostok/network/sources/readers.h>

namespace boost {
namespace asio {
namespace ip {
namespace xsp {

static char const lan_search_request_string[]		= "anybody_there?";
static char const lan_search_response_string[]		= "i'm_here!";
static char const lan_my_key_string[]				= "my_key_is";

static unsigned int const lan_search_wait_timeout	= 3000; //3 seconds

struct random_id
{
	static const unsigned int random_length = 16;
	char m_data[random_length];
	
	template <typename Writer>
	void				write(Writer & writer) const
	{
		writer.write	(m_data, random_length);
	}
	template <typename Reader>
	vostok::signalling_bool		read(Reader & reader)
	{
		vostok::mutable_buffer	tmp_dest(m_data, random_length);
		return reader.read		(tmp_dest, random_length) ? true : false;
	}
	bool equal(random_id const & right) const
	{
		return (vostok::memory::compare(
			vostok::const_buffer(m_data, random_length),
			vostok::const_buffer(right.m_data, random_length)) == 0);
	}
	bool operator != (random_id const & right) const
	{
		return !equal(right);
	}
}; //struct random_id

} // namespace xsp
} // namespace ip
} // namespace asio
} // namespace boost

#endif // #ifndef XSP_LAN_ADDRESSES_SYNC_DEFINES_HPP_INCLUDED