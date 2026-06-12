////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::relocate_item_descr,survarium::std_allocator<survarium::relocate_item_descr> >
	stlp_std::vector<survarium::relocate_item_descr >;
class survarium::relocate_item_descr;

namespace survarium {

class vector< relocate_item_descr > : public std::vector< relocate_item_descr > {
public:
	inline									vector<survarium::relocate_item_descr>( ) { /* no source */ }
	inline									vector<survarium::relocate_item_descr>( u32 arg_0, relocate_item_descr const& arg_1 ) { /* no source */ }
	inline	explicit						vector<survarium::relocate_item_descr>( u32 arg_0 ) { /* no source */ }

	inline	relocate_item_descr&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	relocate_item_descr const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<survarium::relocate_item_descr>( ) { /* no source */ }
}; // class vector< relocate_item_descr >

STATIC_SIZE_ASSERT(vector< relocate_item_descr >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
