////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client_impl.h"

namespace vostok {
namespace network {

// STATE[STUB]
void login_client_impl::on_resolved(
	boost::asio::ip::tcp::resolver* const	resolver,
	const u32							retry_count,
	boost::function< void( enum resolve_error_types_enum, boost::asio::ip::tcp::resolver::iterator ) > const&	functor,
	boost::system::error_code const&	error_code,
	boost::asio::ip::tcp::resolver::iterator	iterator
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &resolver, &retry_count, &functor, &error_code, &iterator );

	// FUNCTION BODY[0x7988f0]: 49
	// <0x79890b>|0x01b|+0x00c:'21'
	// <0>
	// <0x798917>|0x027|+0x015:'23'
	// <0x79892c>|0x03c|+0x080:'24'
	// <0x7989ac>|0x0bc|+0x0da:'25'
	// <0x798a86>|0x196|+0x010:'26'
	// <0x798a96>|0x1a6|+0x00a:'27'
	// <0x798aa0>|0x1b0|+0x081:'28'
	// <0x798b21>|0x231|+0x03d:'29'
	// <0x798b5e>|0x26e|+0x016:'30'
	// <0x798b74>|0x284|+0x026:'31'
	// <0>
	// <0x798b9a>|0x2aa|+0x005:'33'
	// <0x798b9f>|0x2af|+0x07e:'34'
	// <0x798c1d>|0x32d|+0x008:'35'
	// <0x798c25>|0x335|+0x07e:'36'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x798ca3>|0x3b3|+0x0ba:'48'
	// <0x798d5d>|0x46d|+0x026:'49'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x798d83>|0x493|+0x03d:'54'
	// <0x798dc0>|0x4d0|+0x03c:'55'
	// <0x798dfc>|0x50c|+0x0f0:'56'
	// <0x798eec>|0x5fc|+0x089:'57'
	// <0x798f75>|0x685|+0x026:'58'
	// <0>
	// <1>
	// <2>
	// <0x798f9b>|0x6ab|+0x03d:'62'
	// <0>
	// <0x798fd8>|0x6e8|+0x092:'64'
	// <0>
	// <1>
	// <2>
	// <0x79906a>|0x77a|+0x010:'68'
	// <0x79907a>|0x78a|+0x03c:'69'
	// ******
}

// STATE[STUB]
void login_client_impl::resolve(
	boost::function< void( enum resolve_error_types_enum, boost::asio::ip::tcp::resolver::iterator ) > const&	functor,
	const u32		retry_count
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &functor, &retry_count );

	// LOCALS
	// boost::asio::ip::tcp::resolver::query query
	// boost::asio::ip::tcp::resolver* const resolver
	// char[6] 							port
	// ******

	// FUNCTION BODY[0x7990e0]: 23
	// <0x7990fa>|0x01a|+0x080:'74'
	// <0>
	// <0x79917a>|0x09a|+0x012:'76'
	// <0x79918c>|0x0ac|+0x010:'77'
	// <0>
	// <0x79919c>|0x0bc|+0x05e:'79'
	// <0>
	// <1>
	// <0x7991fa>|0x11a|+0x02a:'82'
	// <0>
	// <0x799224>|0x144|+0x097:'84'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x7992bb>|0x1db|+0x07d:'96'
	// ******
}


} // namespace network
} // namespace vostok
