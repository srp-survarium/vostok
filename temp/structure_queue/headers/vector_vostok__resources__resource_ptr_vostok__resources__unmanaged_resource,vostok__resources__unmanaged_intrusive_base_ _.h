////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>,survarium::std_allocator<vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> > >
	stlp_std::vector<vostok::resources::unmanaged_resource_ptr >;
class vostok::resources::unmanaged_resource;

namespace survarium {

class vector< resources::unmanaged_resource_ptr > : public std::vector< resources::unmanaged_resource_ptr > {
public:
	inline											vector<vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }
	inline											vector<vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> >( u32 arg_0, resources::unmanaged_resource_ptr const& arg_1 ) { /* no source */ }
	inline	explicit								vector<vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> >( u32 arg_0 ) { /* no source */ }

	inline	resources::unmanaged_resource_ptr&		operator[]	( u32 arg_0 ) { /* no source */ }
	inline	resources::unmanaged_resource_ptr const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32										size		( ) const { /* no source */ }

	inline											~vector<vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }
}; // class vector< resources::unmanaged_resource_ptr >

STATIC_SIZE_ASSERT(vector< resources::unmanaged_resource_ptr >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
