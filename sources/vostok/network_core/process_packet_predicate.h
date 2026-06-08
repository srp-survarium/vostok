////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PROCESS_PACKET_PREDICATE_H_INCLUDED
#define NETWORK_CORE_PROCESS_PACKET_PREDICATE_H_INCLUDED

#include <boost/noncopyable.hpp>

namespace vostok {
namespace network_core {

class udp_match_client;
class packet_reader;

class process_packet_predicate : public boost::noncopyable {
public:
	inline	explicit	process_packet_predicate	( udp_match_client& client ) :
		m_client	( client )
	{
		/* no source */
	}

	inline	void		operator()					( u8 message_type, packet_reader& reader ) const { /* no source */ }

	inline				~process_packet_predicate	( ) { /* no source */ }

private:
	/* 0x0000 */	udp_match_client&		m_client;
}; // class process_packet_predicate

STATIC_SIZE_ASSERT(process_packet_predicate, 0x4);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_PROCESS_PACKET_PREDICATE_H_INCLUDED
