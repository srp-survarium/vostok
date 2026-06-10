////////////////////////////////////////////////////////////////////////////
//	Created 	: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client_impl.h"

namespace vostok {
namespace network {

// STATE[STUB]
void login_client_impl::on_connected(
	const u32							retry_count,
	boost::function< void( enum connection_error_types_enum ) > const&	functor,
	boost::asio::ip::tcp::resolver::iterator	start_iterator,
	boost::system::error_code const&	error_code,
	boost::asio::ip::tcp::resolver::iterator	iterator
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &retry_count, &functor, &start_iterator, &error_code, &iterator );

	// FUNCTION BODY[0x799350]: 22
	// <0x79936a>|0x01a|+0x00c:'21'
	// <0>
	// <0x799376>|0x026|+0x015:'23'
	// <0x79938b>|0x03b|+0x010:'24'
	// <0>
	// <0x79939b>|0x04b|+0x00a:'26'
	// <0x7993a5>|0x055|+0x080:'27'
	// <0x799425>|0x0d5|+0x0de:'28'
	// <0x799503>|0x1b3|+0x080:'29'
	// <0x799583>|0x233|+0x04a:'30'
	// <0>
	// <0x7995cd>|0x27d|+0x002:'32'
	// <0x7995cf>|0x27f|+0x010:'33'
	// <0x7995df>|0x28f|+0x00a:'34'
	// <0>
	// <1>
	// <0x7995e9>|0x299|+0x047:'37'
	// <0>
	// <1>
	// <0x799630>|0x2e0|+0x08a:'40'
	// <0x7996ba>|0x36a|+0x010:'41'
	// <0x7996ca>|0x37a|+0x00a:'42'
	// ******
}

// STATE[STUB]
void login_client_impl::connect(
	resolve_error_types_enum	error,
	boost::asio::ip::tcp::resolver::iterator	iterator,
	const u32					retry_count,
	boost::function< void( enum connection_error_types_enum ) > const&	functor
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &error, &iterator, &retry_count, &functor );

	// FUNCTION BODY[0x799720]: 29
	// <0x79973b>|0x01b|+0x00c:'52'
	// <0>
	// <0x799747>|0x027|+0x00a:'54'
	// <0x799751>|0x031|+0x080:'55'
	// <0x7997d1>|0x0b1|+0x010:'56'
	// <0x7997e1>|0x0c1|+0x026:'57'
	// <0>
	// <1>
	// <0x799807>|0x0e7|+0x080:'60'
	// <0x799887>|0x167|+0x0ba:'61'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x799941>|0x221|+0x010:'67'
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
	// <11>
	// <0x799951>|0x231|+0x0dd:'80'
	// ******
}


} // namespace network
} // namespace vostok
