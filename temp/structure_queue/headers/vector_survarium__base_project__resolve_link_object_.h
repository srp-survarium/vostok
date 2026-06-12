////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::base_project::resolve_link_object,survarium::std_allocator<survarium::base_project::resolve_link_object> >
	stlp_std::vector<survarium::base_project::resolve_link_object >;
class survarium::base_project::resolve_link_object;

namespace survarium {

class vector< base_project::resolve_link_object > : public std::vector< base_project::resolve_link_object > {
public:
	inline											vector<survarium::base_project::resolve_link_object>( ) { /* no source */ }
	inline											vector<survarium::base_project::resolve_link_object>( u32 arg_0, base_project::resolve_link_object const& arg_1 ) { /* no source */ }
	inline	explicit								vector<survarium::base_project::resolve_link_object>( u32 arg_0 ) { /* no source */ }

	inline	base_project::resolve_link_object&		operator[]	( u32 arg_0 ) { /* no source */ }
	inline	base_project::resolve_link_object const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32										size		( ) const { /* no source */ }

	inline											~vector<survarium::base_project::resolve_link_object>( ) { /* no source */ }
}; // class vector< base_project::resolve_link_object >

STATIC_SIZE_ASSERT(vector< base_project::resolve_link_object >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
