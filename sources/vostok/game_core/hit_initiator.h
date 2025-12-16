////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef HIT_INITIATOR_H_INCLUDED
#define HIT_INITIATOR_H_INCLUDED

namespace survarium {

struct hit_initiator : public boost::noncopyable {
public:
	inline	explicit	hit_initiator	( u8 id_, bool is_local_ ) : id( id_ ), is_local( is_local ) { }
	virtual				~hit_initiator	( ) { }

	virtual	void		on_fire			( ) { }

public:
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	u8		id;
	/* 0x0005 */	bool	is_local;
}; // struct hit_initiator

STATIC_SIZE_ASSERT(hit_initiator, 0x8);

} // namespace survarium

#endif // #ifndef HIT_INITIATOR_H_INCLUDED
