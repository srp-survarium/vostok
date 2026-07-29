////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MAP_H_INCLUDED
#define RENDER_MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<vostok::render::render_model_instance *,vostok::render::material_effects,stlp_std::less<vostok::render::render_model_instance *>,vostok::render::std_allocator<stlp_std::pair<vostok::render::render_model_instance *,vostok::render::material_effects> > >;

/* FORWARD REFS */
class stlp_std::less<vostok::render::render_model_instance *>;
class vostok::render::map<vostok::render::render_model_instance *,stlp_std::pair<vostok::render::render_model_instance * const,vostok::render::material_effects>,stlp_std::less<vostok::render::render_model_instance *> >;

namespace vostok {
namespace render {

class map< render_model_instance*, material_effects, std::less< render_model_instance* > > : public std::map< render_model_instance*, material_effects, std::less< render_model_instance* >, std_allocator< std::pair< render_model_instance*, material_effects > > > {
public:
	inline				map<vostok::render::render_model_instance *,vostok::render::material_effects,stlp_std::less<vostok::render::render_model_instance *> >( ) { /* no source */ }
	inline	explicit	map<vostok::render::render_model_instance *,vostok::render::material_effects,stlp_std::less<vostok::render::render_model_instance *> >( std::less< render_model_instance* > const& arg_0 ) { /* no source */ }
	inline	explicit	map<vostok::render::render_model_instance *,vostok::render::material_effects,stlp_std::less<vostok::render::render_model_instance *> >(
							map< render_model_instance*, std::pair< render_model_instance* const, material_effects >, std::less< render_model_instance* > > const&	arg_0
						) { /* no source */ }

	inline	map< render_model_instance*, std::pair< render_model_instance* const, material_effects >, std::less< render_model_instance* > >&	operator=	(
							map< render_model_instance*, std::pair< render_model_instance* const, material_effects >, std::less< render_model_instance* > > const&	arg_0
						) { /* no source */ }

	inline				~map<vostok::render::render_model_instance *,vostok::render::material_effects,stlp_std::less<vostok::render::render_model_instance *> >( ) { /* no source */ }
}; // class map< render_model_instance*, material_effects, std::less< render_model_instance* > >

STATIC_SIZE_ASSERT(map< render_model_instance*, material_effects, std::less< render_model_instance* > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MAP_H_INCLUDED
