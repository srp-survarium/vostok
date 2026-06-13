////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "messaging_client.h"

namespace survarium {

// STATE[STUB]
 messaging_client::messaging_client( game& g ) :
	// ref members + no-default-ctor packet client force the init-list; the
	// chat_handler/world placeholders are buildability only - a matcher
	// supplies the real sources (game's chat handler) when this TU is enabled
	m_game( g ),
	m_chat_handler( *( chat_handler* )NULL ),
	m_network_client( *( network::world* )NULL )
{
	// FUNCTION BODY[0x7666a0]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x766727>|0x087|+0x00c:'33'
	// <0x766733>|0x093|+0x02a:'34'
	// <0x76675d>|0x0bd|+0x042:'35'
	// <0x76679f>|0x0ff|+0x042:'36'
	// <0x7667e1>|0x141|+0x042:'37'
	// ******
}

// STATE[STUB]
 messaging_client::~messaging_client( )
{
	// FUNCTION BODY[0x7661c0]: 0
	// <0x7661c0>|0x000|+0x000:'41'	{
	// <0x7661c0>|0x000|      :'42'	}
	// ******
}

// STATE[STUB]
void messaging_client::assign_match_channel_order(
	const u32				match_id,
	const game_team_id		arg_1 /* game_team_id team_id */
)
{
	// FUNCTION BODY[0x766370]: 6
	// <0x766374>|0x004|+0x00d:'46'
	// <0>
	// <0x766381>|0x011|+0x006:'48'
	// <0x766387>|0x017|+0x00a:'49'
	// <0x766391>|0x021|+0x005:'50'
	// <0>
	// ******
}

// STATE[STUB]
void messaging_client::connect( server_connection_info const& connection_info )
{
	// FUNCTION BODY[0x7663a0]: 12
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7663a6>|0x006|+0x02c:'60'
	// <0x7663d2>|0x032|+0x00e:'61'
	// <0>
	// <0x7663e0>|0x040|+0x009:'63'
	// <0x7663e9>|0x049|+0x007:'64'
	// <0>
	// <0x7663f0>|0x050|+0x042:'66'
	// <0x766432>|0x092|+0x00d:'67'
	// ******
}

// STATE[STUB]
void messaging_client::disconnect( )
{
	// FUNCTION BODY[0x766150]: 6
	// <0x766153>|0x003|+0x00a:'72'
	// <0>
	// <1>
	// <0x76615d>|0x00d|+0x017:'75'
	// <0x766174>|0x024|+0x038:'76'
	// <0>
	// ******
}

// STATE[STUB]
void messaging_client::on_connected( )
{
	// LOCALS
	// network_core::tcp_packet 		packet
	// ******

	// CALL SITE INFO
	// <0x766646> -> network::login_client& < unknown >()
	// ******

	// FUNCTION BODY[0x7665b0]: 8
	// <0>
	// <0x7665be>|0x00e|+0x058:'84'
	// <0>
	// <0x766616>|0x066|+0x00e:'86'
	// <0x766624>|0x074|+0x015:'87'
	// <0x766639>|0x089|+0x026:'88'
	// <0x76665f>|0x0af|+0x011:'89'
	// <0x766670>|0x0c0|+0x00a:'90'
	// ******
}

// STATE[STUB]
void messaging_client::on_disconnected( )
{
	// FUNCTION BODY[0x765ed0]: 2
	// <0>
	// <0x765ed9>|0x009|+0x044:'96'
	// ******
}

// STATE[STUB]
void messaging_client::on_error(
	network_core::client_error_codes_enum	client_error_code,
	boost::system::error_code		system_error_code
)
{
	// FUNCTION BODY[0x766450]: 7
	// <0>
	// <0x76645c>|0x00c|+0x01d:'102'
	// <0>
	// <0x766479>|0x029|+0x10d:'104'
	// <0x766586>|0x136|+0x007:'105'
	// <0>
	// <0x76658d>|0x13d|+0x00d:'107'
	// ******
}

// STATE[STUB]
bool messaging_client::read_friend_status( network_core::packet_reader& reader )
{
	// LOCALS
	// const u32 						account_id
	// ******

	return false;

	// FUNCTION BODY[0x766220]: 17
	// <0x766229>|0x009|+0x019:'112'
	// <0>
	// <0x766242>|0x022|+0x109:'114'
	// <0x76634b>|0x12b|-0x0fb:'114'
	// <0>
	// <0x766250>|0x030|+0x00b:'116'
	// <0x76625b>|0x03b|+0x006:'117'
	// <0>
	// <0x766261>|0x041|+0x01d:'119'
	// <0x76627e>|0x05e|+0x004:'120'
	// <0x766282>|0x062|+0x003:'121'
	// <0x766285>|0x065|+0x005:'122'
	// <0>
	// <0x76628a>|0x06a|+0x0cc:'124'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
bool messaging_client::read_friend_list( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x766090]: 11
	// <0x766093>|0x003|+0x05d:'133'
	// <0x7660f0>|0x060|-0x04f:'133'
	// <0x7660a1>|0x011|+0x039:'134'
	// <0>
	// <0x7660da>|0x04a|+0x01a:'136'
	// <0>
	// <1>
	// <0x7660f4>|0x064|+0x013:'139'
	// <0x766107>|0x077|+0x023:'140'
	// <0x76612a>|0x09a|+0x018:'141'
	// <0>
	// <0x766142>|0x0b2|+0x003:'143'
	// ******
}

// STATE[STUB]
bool messaging_client::read_ignore_list( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x765fe0]: 9
	// <0x765fe3>|0x003|+0x054:'148'
	// <0x766037>|0x057|-0x046:'148'
	// <0x765ff1>|0x011|+0x039:'149'
	// <0x76602a>|0x04a|+0x011:'150'
	// <0>
	// <1>
	// <0x76603b>|0x05b|+0x011:'153'
	// <0x76604c>|0x06c|+0x02d:'154'
	// <0>
	// <0x766079>|0x099|+0x003:'156'
	// ******
}

// STATE[STUB]
bool messaging_client::read_found_players( network_core::packet_reader& reader )
{
	return false;

	// FUNCTION BODY[0x765f30]: 9
	// <0x765f33>|0x003|+0x054:'161'
	// <0x765f87>|0x057|-0x046:'161'
	// <0x765f41>|0x011|+0x039:'162'
	// <0x765f7a>|0x04a|+0x011:'163'
	// <0>
	// <1>
	// <0x765f8b>|0x05b|+0x011:'166'
	// <0x765f9c>|0x06c|+0x02d:'167'
	// <0>
	// <0x765fc9>|0x099|+0x003:'169'
	// ******
}

} // namespace survarium
