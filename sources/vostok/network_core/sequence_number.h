////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_SEQUENCE_NUMBER_H_INCLUDED
#define NETWORK_CORE_SEQUENCE_NUMBER_H_INCLUDED

namespace vostok {
namespace network_core {

/* vostok::network_core::sequence_number<unsigned char> */

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class sequence_number<u8> {
public:
	inline							sequence_number<unsigned char>	( u8 arg_0 ) { /* no source */ }

	inline	void					serialize						( pbyte& arg_0 ) { /* no source */ }

	inline	void					serialize						( udp_match_packet& arg_0 ) { /* no source */ }

	inline	bool					operator==						( sequence_number<u8> const& arg_0 ) const { /* no source */ }

	inline	bool					operator!=						( sequence_number<u8> const& arg_0 ) const { /* no source */ }

	inline	bool					operator<						( sequence_number<u8> const& arg_0 ) const { /* no source */ }

	inline	bool					operator<=						( sequence_number<u8> const& arg_0 ) const { /* no source */ }

	inline	bool					operator>						( sequence_number<u8> const& arg_0 ) const { /* no source */ }

	inline	bool					operator>=						( sequence_number<u8> const& arg_0 ) const { /* no source */ }

	inline	sequence_number<u8>		operator++						( s32 arg_0 ) { /* no source */ }

	inline	sequence_number<u8>&	operator++						( ) { /* no source */ }

	inline	sequence_number<u8>		operator--						( s32 arg_0 ) { /* no source */ }

	inline	sequence_number<u8>&	operator--						( ) { /* no source */ }


	static	inline	sequence_number<u8>		deserialize						( packet_reader& arg_0 ) { /* no source */ }


private:
	/* 0x0000 */	u8		m_number;
}; // class sequence_number<u8>

STATIC_SIZE_ASSERT(sequence_number<u8>, 0x1);

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_SEQUENCE_NUMBER_H_INCLUDED
