////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::light_props,survarium::std_allocator<vostok::render::light_props> >
	stlp_std::vector<vostok::render::light_props >;
class vostok::render::light_props;

namespace survarium {

class vector< render::light_props > : public std::vector< render::light_props > {
public:
	inline									vector<vostok::render::light_props>	( ) { /* no source */ }
	inline									vector<vostok::render::light_props>	( u32 arg_0, render::light_props const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::render::light_props>	( u32 arg_0 ) { /* no source */ }

	inline	render::light_props&			operator[]							( u32 arg_0 ) { /* no source */ }
	inline	render::light_props const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32								size								( ) const { /* no source */ }

	inline									~vector<vostok::render::light_props>( ) { /* no source */ }
}; // class vector< render::light_props >

STATIC_SIZE_ASSERT(vector< render::light_props >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
