////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MAP_H_INCLUDED
#define RENDER_MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<vostok::render::render_surface_instance *,vostok::math::float4x4,stlp_std::less<vostok::render::render_surface_instance *>,vostok::render::std_allocator<stlp_std::pair<vostok::render::render_surface_instance *,vostok::math::float4x4> > >;

/* FORWARD REFS */
class stlp_std::less<vostok::render::render_surface_instance *>;
class vostok::render::map<vostok::render::render_surface_instance *,stlp_std::pair<vostok::render::render_surface_instance * const,vostok::math::float4x4>,stlp_std::less<vostok::render::render_surface_instance *> >;

namespace vostok {
namespace render {

class map< render_surface_instance*, float4x4, std::less< render_surface_instance* > > : public std::map< render_surface_instance*, float4x4, std::less< render_surface_instance* >, std_allocator< std::pair< render_surface_instance*, float4x4 > > > {
public:
	inline				map<vostok::render::render_surface_instance *,vostok::math::float4x4,stlp_std::less<vostok::render::render_surface_instance *> >( ) { /* no source */ }
	inline	explicit	map<vostok::render::render_surface_instance *,vostok::math::float4x4,stlp_std::less<vostok::render::render_surface_instance *> >( std::less< render_surface_instance* > const& arg_0 ) { /* no source */ }
	inline	explicit	map<vostok::render::render_surface_instance *,vostok::math::float4x4,stlp_std::less<vostok::render::render_surface_instance *> >(
							map< render_surface_instance*, std::pair< render_surface_instance* const, float4x4 >, std::less< render_surface_instance* > > const&	arg_0
						) { /* no source */ }

	inline	map< render_surface_instance*, std::pair< render_surface_instance* const, float4x4 >, std::less< render_surface_instance* > >&	operator=	(
							map< render_surface_instance*, std::pair< render_surface_instance* const, float4x4 >, std::less< render_surface_instance* > > const&	arg_0
						) { /* no source */ }

	inline				~map<vostok::render::render_surface_instance *,vostok::math::float4x4,stlp_std::less<vostok::render::render_surface_instance *> >( ) { /* no source */ }
}; // class map< render_surface_instance*, float4x4, std::less< render_surface_instance* > >

STATIC_SIZE_ASSERT(map< render_surface_instance*, float4x4, std::less< render_surface_instance* > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MAP_H_INCLUDED
