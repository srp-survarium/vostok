////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/async_connector.h>

namespace vostok {
namespace network_core {

// STATE[STUB]
async_connector::async_connector( )
{
	// FUNCTION BODY[0x554ff0]: 0
	// <0x554ff0>|0x000|+0x079:'16'	{
	// <0x555069>|0x079|      :'17'	}
	// ******
}

// STATE[STUB]
void async_connector::on_connected(
	boost::system::error_code const&	error_code,
	boost::asio::ip::tcp::resolver::iterator	iterator
)
{
	// FUNCTION BODY[0x555070]: 14
	// <0x555087>|0x017|+0x00c:'24'
	// <0x555093>|0x023|+0x011:'26'
	// <0x5550a4>|0x034|+0x010:'27'
	// <0x5550b4>|0x044|+0x01f:'28'
	// <0x5550d3>|0x063|+0x01a:'29'
	// <0x5550ed>|0x07d|+0x026:'30'
	// <0x555113>|0x0a3|+0x078:'33'
	// <0x55518b>|0x11b|+0x010:'34'
	// <0x55519b>|0x12b|+0x01f:'36'
	// <0x5551ba>|0x14a|+0x00e:'37'
	// ******
}

// STATE[STUB]
void async_connector::connect( boost::asio::ip::tcp::resolver::iterator const& iterator )
{
	// FUNCTION BODY[0x5551f0]: 11
	// <0x5551ff>|0x00f|+0x010:'42'
	// <0x55520f>|0x01f|+0x08d:'52'
	// ******
}

// STATE[STUB]
void async_connector::on_resolved(
	boost::asio::ip::tcp::resolver*		resolver,
	boost::system::error_code const&	error_code,
	boost::asio::ip::tcp::resolver::iterator	iterator
)
{
	// FUNCTION BODY[0x5552b0]: 34
	// <0x5552cb>|0x01b|+0x00c:'61'
	// <0x5552d7>|0x027|+0x015:'63'
	// <0x5552ec>|0x03c|+0x080:'64'
	// <0x55536c>|0x0bc|+0x0da:'65'
	// <0x555446>|0x196|+0x008:'66'
	// <0x55544e>|0x19e|+0x069:'67'
	// <0x5554b7>|0x207|+0x0c2:'77'
	// <0x555579>|0x2c9|+0x026:'78'
	// <0x55559f>|0x2ef|+0x017:'81'
	// <0x5555b6>|0x306|+0x010:'82'
	// <0x5555c6>|0x316|+0x0ef:'83'
	// <0x5556b5>|0x405|+0x089:'84'
	// <0x55573e>|0x48e|+0x01e:'85'
	// <0x55575c>|0x4ac|+0x01a:'86'
	// <0x555776>|0x4c6|+0x026:'87'
	// <0x55579c>|0x4ec|+0x017:'90'
	// <0x5557b3>|0x503|+0x08a:'91'
	// <0x55583d>|0x58d|+0x010:'92'
	// <0x55584d>|0x59d|+0x01b:'93'
	// <0x555868>|0x5b8|+0x00f:'94'
	// ******
}

// STATE[STUB]
void async_connector::connect(
	boost::asio::ip::tcp::socket&		socket,
	pcstr								host,
	u16									host_port,
	boost::function< void() > const&	on_connected,
	boost::function< void( client_error_codes_enum, boost::system::error_code ) > const&	on_error
)
{
	// LOCALS
	// boost::asio::ip::tcp::resolver::query query
	// boost::asio::ip::tcp::resolver* const resolver
	// char[6] 							port
	// ******

	// FUNCTION BODY[0x5558a0]: 26
	// <0x5558bb>|0x01b|+0x00f:'105'
	// <0x5558ca>|0x02a|+0x010:'106'
	// <0x5558da>|0x03a|+0x012:'107'
	// <0x5558ec>|0x04c|+0x011:'108'
	// <0x5558fd>|0x05d|+0x080:'110'
	// <0x55597d>|0x0dd|+0x012:'112'
	// <0x55598f>|0x0ef|+0x010:'113'
	// <0x55599f>|0x0ff|+0x099:'115'
	// <0x555a38>|0x198|+0x015:'118'
	// <0x555a4d>|0x1ad|+0x08e:'120'
	// <0x555adb>|0x23b|+0x077:'130'
	// ******
}

// STATE[STUB]
void async_connector::reset( )
{
	// FUNCTION BODY[0x554fd0]: 1
	// <0x554fd7>|0x007|+0x00d:'135'
	// ******
}

} // namespace network_core
} // namespace vostok
