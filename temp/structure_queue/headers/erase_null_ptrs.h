////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ERASE_NULL_PTRS_H_INCLUDED
#define ERASE_NULL_PTRS_H_INCLUDED

/* FORWARD REFS */
class survarium::hit_receiver_info;

namespace survarium {

struct erase_null_ptrs : public boost::noncopyable {
	inline	bool	operator()		( hit_receiver_info const& arg_0 ) const { /* no source */ }

	inline			erase_null_ptrs	( ) { /* no source */ }
	inline			~erase_null_ptrs( ) { /* no source */ }
}; // struct erase_null_ptrs

STATIC_SIZE_ASSERT(erase_null_ptrs, 0x1);

} // namespace survarium

#endif // #ifndef ERASE_NULL_PTRS_H_INCLUDED
