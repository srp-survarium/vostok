////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<unsigned short,survarium::game_material const *,stlp_std::less<unsigned short>,survarium::std_allocator<stlp_std::pair<unsigned short,survarium::game_material const *> > >;

/* FORWARD REFS */
class survarium::map<unsigned short,stlp_std::pair<unsigned short const ,survarium::game_material const *>,stlp_std::less<unsigned short> >;

namespace survarium {

class map< u16, game_material const*, std::less< u16 > > : public std::map< u16, game_material const*, std::less< u16 >, std_allocator< std::pair< u16, game_material const* > > > {
public:
	inline				map<unsigned short,survarium::game_material const *,stlp_std::less<unsigned short> >( ) { /* no source */ }
	inline	explicit	map<unsigned short,survarium::game_material const *,stlp_std::less<unsigned short> >( std::less< u16 > const& arg_0 ) { /* no source */ }
	inline	explicit	map<unsigned short,survarium::game_material const *,stlp_std::less<unsigned short> >(
							map< u16, std::pair< u16 const , game_material const* >, std::less< u16 > > const&	arg_0
						) { /* no source */ }

	inline	map< u16, std::pair< u16 const , game_material const* >, std::less< u16 > >&	operator=	(
							map< u16, std::pair< u16 const , game_material const* >, std::less< u16 > > const&	arg_0
						) { /* no source */ }

	inline				~map<unsigned short,survarium::game_material const *,stlp_std::less<unsigned short> >( ) { /* no source */ }
}; // class map< u16, game_material const*, std::less< u16 > >

STATIC_SIZE_ASSERT(map< u16, game_material const*, std::less< u16 > >, 0x18);

} // namespace survarium

#endif // #ifndef MAP_H_INCLUDED
