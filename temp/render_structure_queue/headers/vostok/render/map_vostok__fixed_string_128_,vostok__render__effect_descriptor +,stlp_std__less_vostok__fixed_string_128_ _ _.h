////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MAP_H_INCLUDED
#define RENDER_MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<vostok::fixed_string<128>,vostok::render::effect_descriptor *,stlp_std::less<vostok::fixed_string<128> >,vostok::render::std_allocator<stlp_std::pair<vostok::fixed_string<128>,vostok::render::effect_descriptor *> > >;

/* FORWARD REFS */
class stlp_std::less<vostok::fixed_string<128> >;
class vostok::render::map<vostok::fixed_string<128>,stlp_std::pair<vostok::fixed_string<128> const ,vostok::render::effect_descriptor *>,stlp_std::less<vostok::fixed_string<128> > >;

namespace vostok {
namespace render {

class map< fixed_string< 128 >, effect_descriptor*, std::less< fixed_string< 128 > > > : public std::map< fixed_string< 128 >, effect_descriptor*, std::less< fixed_string< 128 > >, std_allocator< std::pair< fixed_string< 128 >, effect_descriptor* > > > {
public:
	inline				map<vostok::fixed_string<128>,vostok::render::effect_descriptor *,stlp_std::less<vostok::fixed_string<128> > >( ) { /* no source */ }
	inline	explicit	map<vostok::fixed_string<128>,vostok::render::effect_descriptor *,stlp_std::less<vostok::fixed_string<128> > >( std::less< fixed_string< 128 > > const& arg_0 ) { /* no source */ }
	inline	explicit	map<vostok::fixed_string<128>,vostok::render::effect_descriptor *,stlp_std::less<vostok::fixed_string<128> > >(
							map< fixed_string< 128 >, std::pair< fixed_string< 128 > const , effect_descriptor* >, std::less< fixed_string< 128 > > > const&	arg_0
						) { /* no source */ }

	inline	map< fixed_string< 128 >, std::pair< fixed_string< 128 > const , effect_descriptor* >, std::less< fixed_string< 128 > > >&	operator=	(
							map< fixed_string< 128 >, std::pair< fixed_string< 128 > const , effect_descriptor* >, std::less< fixed_string< 128 > > > const&	arg_0
						) { /* no source */ }

	inline				~map<vostok::fixed_string<128>,vostok::render::effect_descriptor *,stlp_std::less<vostok::fixed_string<128> > >( ) { /* no source */ }
}; // class map< fixed_string< 128 >, effect_descriptor*, std::less< fixed_string< 128 > > >

STATIC_SIZE_ASSERT(map< fixed_string< 128 >, effect_descriptor*, std::less< fixed_string< 128 > > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MAP_H_INCLUDED
