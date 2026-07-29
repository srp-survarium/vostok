////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::culling::portal_sector_system::quad,vostok::render::std_allocator<vostok::render::culling::portal_sector_system::quad> >
	stlp_std::vector<vostok::render::culling::portal_sector_system::quad >;
class vostok::render::culling::portal_sector_system::quad;

namespace vostok {
namespace render {

class vector< culling::portal_sector_system::quad > : public std::vector< culling::portal_sector_system::quad > {
public:
	inline				vector<vostok::render::culling::portal_sector_system::quad>( ) { /* no source */ }
	inline				vector<vostok::render::culling::portal_sector_system::quad>( u32 arg_0, culling::portal_sector_system::quad const& arg_1 ) { /* no source */ }
	inline	explicit	vector<vostok::render::culling::portal_sector_system::quad>( u32 arg_0 ) { /* no source */ }

	inline	culling::portal_sector_system::quad&	operator[]	( u32 arg_0 ) { /* no source */ }
	inline	culling::portal_sector_system::quad const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32			size		( ) const { /* no source */ }

	inline				~vector<vostok::render::culling::portal_sector_system::quad>( ) { /* no source */ }
}; // class vector< culling::portal_sector_system::quad >

STATIC_SIZE_ASSERT(vector< culling::portal_sector_system::quad >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
