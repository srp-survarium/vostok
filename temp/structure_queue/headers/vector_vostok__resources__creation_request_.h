////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::resources::creation_request,survarium::std_allocator<vostok::resources::creation_request> >
	stlp_std::vector<vostok::resources::creation_request >;
class vostok::resources::creation_request;

namespace survarium {

class vector< resources::creation_request > : public std::vector< resources::creation_request > {
public:
	inline											vector<vostok::resources::creation_request>( ) { /* no source */ }
	inline											vector<vostok::resources::creation_request>( u32 arg_0, resources::creation_request const& arg_1 ) { /* no source */ }
	inline	explicit								vector<vostok::resources::creation_request>( u32 arg_0 ) { /* no source */ }

	inline	resources::creation_request&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	resources::creation_request const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32										size		( ) const { /* no source */ }

	inline											~vector<vostok::resources::creation_request>( ) { /* no source */ }
}; // class vector< resources::creation_request >

STATIC_SIZE_ASSERT(vector< resources::creation_request >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
