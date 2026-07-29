////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MAP_H_INCLUDED
#define RENDER_MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<unsigned int,vostok::render::texture_pool *,stlp_std::less<unsigned int>,vostok::render::std_allocator<stlp_std::pair<unsigned int,vostok::render::texture_pool *> > >;

/* FORWARD REFS */
class vostok::render::map<unsigned int,stlp_std::pair<unsigned int const ,vostok::render::texture_pool *>,stlp_std::less<unsigned int> >;

namespace vostok {
namespace render {

class map< u32, texture_pool*, std::less< u32 > > : public std::map< u32, texture_pool*, std::less< u32 >, std_allocator< std::pair< u32, texture_pool* > > > {
public:
	inline				map<unsigned int,vostok::render::texture_pool *,stlp_std::less<unsigned int> >( ) { /* no source */ }
	inline	explicit	map<unsigned int,vostok::render::texture_pool *,stlp_std::less<unsigned int> >( std::less< u32 > const& arg_0 ) { /* no source */ }
	inline	explicit	map<unsigned int,vostok::render::texture_pool *,stlp_std::less<unsigned int> >(
							map< u32, std::pair< u32 const , texture_pool* >, std::less< u32 > > const&	arg_0
						) { /* no source */ }

	inline	map< u32, std::pair< u32 const , texture_pool* >, std::less< u32 > >&	operator=	(
							map< u32, std::pair< u32 const , texture_pool* >, std::less< u32 > > const&	arg_0
						) { /* no source */ }

	inline				~map<unsigned int,vostok::render::texture_pool *,stlp_std::less<unsigned int> >( ) { /* no source */ }
}; // class map< u32, texture_pool*, std::less< u32 > >

STATIC_SIZE_ASSERT(map< u32, texture_pool*, std::less< u32 > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MAP_H_INCLUDED
