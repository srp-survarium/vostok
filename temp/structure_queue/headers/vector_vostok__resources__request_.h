////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::resources::request,survarium::std_allocator<vostok::resources::request> >
	stlp_std::vector<vostok::resources::request >;
class vostok::resources::request;

namespace survarium {

class vector< resources::request > : public std::vector< resources::request > {
public:
	inline									vector<vostok::resources::request>	( ) { /* no source */ }
	inline									vector<vostok::resources::request>	( u32 arg_0, resources::request const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::resources::request>	( u32 arg_0 ) { /* no source */ }

	inline	resources::request&				operator[]							( u32 index ) { /* no source */ }
	inline	resources::request const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32								size								( ) const { /* no source */ }

	inline									~vector<vostok::resources::request>	( ) { /* no source */ }
}; // class vector< resources::request >

STATIC_SIZE_ASSERT(vector< resources::request >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
