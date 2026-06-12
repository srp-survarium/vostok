////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::resources::resource_ptr<survarium::ladder,vostok::resources::unmanaged_intrusive_base>,survarium::std_allocator<vostok::resources::resource_ptr<survarium::ladder,vostok::resources::unmanaged_intrusive_base> > >
	stlp_std::vector<survarium::ladder_ptr >;
class survarium::ladder;

namespace survarium {

class vector< ladder_ptr > : public std::vector< ladder_ptr > {
public:
	inline							vector<vostok::resources::resource_ptr<survarium::ladder,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }
	inline							vector<vostok::resources::resource_ptr<survarium::ladder,vostok::resources::unmanaged_intrusive_base> >( u32 arg_0, ladder_ptr const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::resources::resource_ptr<survarium::ladder,vostok::resources::unmanaged_intrusive_base> >( u32 arg_0 ) { /* no source */ }

	inline	ladder_ptr&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	ladder_ptr const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32						size		( ) const { /* no source */ }

	inline							~vector<vostok::resources::resource_ptr<survarium::ladder,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }
}; // class vector< ladder_ptr >

STATIC_SIZE_ASSERT(vector< ladder_ptr >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
