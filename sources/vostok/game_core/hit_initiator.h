////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef HIT_INITIATOR_H_INCLUDED
#define HIT_INITIATOR_H_INCLUDED

namespace survarium {

struct hit_initiator : private boost::noncopyable {
	inline	explicit	hit_initiator	( const u8 id, const bool is_local ) : id( id ), is_local( is_local ) { }
	virtual				~hit_initiator	( ) { }

	virtual	void		on_fire			( ) { }

	/* 0x0004 */	const u8	id;
	/* 0x0005 */	const bool	is_local;
}; // struct hit_initiator

STATIC_SIZE_ASSERT(hit_initiator, 0x8);

} // namespace survarium

#endif // #ifndef HIT_INITIATOR_H_INCLUDED
