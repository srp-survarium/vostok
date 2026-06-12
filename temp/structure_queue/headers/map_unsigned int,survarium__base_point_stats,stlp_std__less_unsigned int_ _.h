////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<unsigned int,survarium::base_point_stats,stlp_std::less<unsigned int>,survarium::std_allocator<stlp_std::pair<unsigned int,survarium::base_point_stats> > >;

/* FORWARD REFS */
class survarium::map<unsigned int,stlp_std::pair<unsigned int const ,survarium::base_point_stats>,stlp_std::less<unsigned int> >;

namespace survarium {

class map< u32, base_point_stats, std::less< u32 > > : public std::map< u32, base_point_stats, std::less< u32 >, std_allocator< std::pair< u32, base_point_stats > > > {
public:
	inline				map<unsigned int,survarium::base_point_stats,stlp_std::less<unsigned int> >( ) { /* no source */ }
	inline	explicit	map<unsigned int,survarium::base_point_stats,stlp_std::less<unsigned int> >( std::less< u32 > const& arg_0 ) { /* no source */ }
	inline	explicit	map<unsigned int,survarium::base_point_stats,stlp_std::less<unsigned int> >(
							map< u32, std::pair< u32 const , base_point_stats >, std::less< u32 > > const&	arg_0
						) { /* no source */ }

	inline	map< u32, std::pair< u32 const , base_point_stats >, std::less< u32 > >&	operator=	(
							map< u32, std::pair< u32 const , base_point_stats >, std::less< u32 > > const&	arg_0
						) { /* no source */ }

	inline				~map<unsigned int,survarium::base_point_stats,stlp_std::less<unsigned int> >( ) { /* no source */ }
}; // class map< u32, base_point_stats, std::less< u32 > >

STATIC_SIZE_ASSERT(map< u32, base_point_stats, std::less< u32 > >, 0x18);

} // namespace survarium

#endif // #ifndef MAP_H_INCLUDED
