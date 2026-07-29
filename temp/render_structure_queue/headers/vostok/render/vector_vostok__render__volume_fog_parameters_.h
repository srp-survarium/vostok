////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::volume_fog_parameters,vostok::render::std_allocator<vostok::render::volume_fog_parameters> >
	stlp_std::vector<vostok::render::volume_fog_parameters >;
class vostok::render::volume_fog_parameters;

namespace vostok {
namespace render {

class vector< volume_fog_parameters > : public std::vector< volume_fog_parameters > {
public:
	inline									vector<vostok::render::volume_fog_parameters>( ) { /* no source */ }
	inline									vector<vostok::render::volume_fog_parameters>( u32 arg_0, volume_fog_parameters const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::render::volume_fog_parameters>( u32 arg_0 ) { /* no source */ }

	inline	volume_fog_parameters&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	volume_fog_parameters const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<vostok::render::volume_fog_parameters>( ) { /* no source */ }
}; // class vector< volume_fog_parameters >

STATIC_SIZE_ASSERT(vector< volume_fog_parameters >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
