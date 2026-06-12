////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<vostok::fixed_string<16>,survarium::hit_type_parameters_modifyer,stlp_std::less<vostok::fixed_string<16> >,survarium::std_allocator<stlp_std::pair<vostok::fixed_string<16>,survarium::hit_type_parameters_modifyer> > >;

/* FORWARD REFS */
class stlp_std::less<vostok::fixed_string<16> >;
class survarium::map<vostok::fixed_string<16>,stlp_std::pair<vostok::fixed_string<16> const ,survarium::hit_type_parameters_modifyer>,stlp_std::less<vostok::fixed_string<16> > >;

namespace survarium {

class map< fixed_string< 16 >, hit_type_parameters_modifyer, std::less< fixed_string< 16 > > > : public std::map< fixed_string< 16 >, hit_type_parameters_modifyer, std::less< fixed_string< 16 > >, std_allocator< std::pair< fixed_string< 16 >, hit_type_parameters_modifyer > > > {
public:
	inline				map<vostok::fixed_string<16>,survarium::hit_type_parameters_modifyer,stlp_std::less<vostok::fixed_string<16> > >( ) { /* no source */ }
	inline	explicit	map<vostok::fixed_string<16>,survarium::hit_type_parameters_modifyer,stlp_std::less<vostok::fixed_string<16> > >( std::less< fixed_string< 16 > > const& arg_0 ) { /* no source */ }
	inline	explicit	map<vostok::fixed_string<16>,survarium::hit_type_parameters_modifyer,stlp_std::less<vostok::fixed_string<16> > >(
							map< fixed_string< 16 >, std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer >, std::less< fixed_string< 16 > > > const&	arg_0
						) { /* no source */ }

	inline	map< fixed_string< 16 >, std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer >, std::less< fixed_string< 16 > > >&	operator=	(
							map< fixed_string< 16 >, std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer >, std::less< fixed_string< 16 > > > const&	arg_0
						) { /* no source */ }

	inline				~map<vostok::fixed_string<16>,survarium::hit_type_parameters_modifyer,stlp_std::less<vostok::fixed_string<16> > >( ) { /* no source */ }
}; // class map< fixed_string< 16 >, hit_type_parameters_modifyer, std::less< fixed_string< 16 > > >

STATIC_SIZE_ASSERT(map< fixed_string< 16 >, hit_type_parameters_modifyer, std::less< fixed_string< 16 > > >, 0x18);

} // namespace survarium

#endif // #ifndef MAP_H_INCLUDED
