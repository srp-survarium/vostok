////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<vostok::fixed_string<260>,survarium::base_game_object *,stlp_std::less<vostok::fixed_string<260> >,survarium::std_allocator<stlp_std::pair<vostok::fixed_string<260>,survarium::base_game_object *> > >;

/* FORWARD REFS */
class stlp_std::less<vostok::fixed_string<260> >;
class survarium::map<vostok::fixed_string<260>,stlp_std::pair<vostok::fixed_string<260> const ,survarium::base_game_object *>,stlp_std::less<vostok::fixed_string<260> > >;

namespace survarium {

class map< fixed_string< 260 >, base_game_object*, std::less< fixed_string< 260 > > > : public std::map< fixed_string< 260 >, base_game_object*, std::less< fixed_string< 260 > >, std_allocator< std::pair< fixed_string< 260 >, base_game_object* > > > {
public:
	inline				map<vostok::fixed_string<260>,survarium::base_game_object *,stlp_std::less<vostok::fixed_string<260> > >( ) { /* no source */ }
	inline	explicit	map<vostok::fixed_string<260>,survarium::base_game_object *,stlp_std::less<vostok::fixed_string<260> > >( std::less< fixed_string< 260 > > const& arg_0 ) { /* no source */ }
	inline	explicit	map<vostok::fixed_string<260>,survarium::base_game_object *,stlp_std::less<vostok::fixed_string<260> > >(
							map< fixed_string< 260 >, std::pair< fixed_string< 260 > const , base_game_object* >, std::less< fixed_string< 260 > > > const&	arg_0
						) { /* no source */ }

	inline	map< fixed_string< 260 >, std::pair< fixed_string< 260 > const , base_game_object* >, std::less< fixed_string< 260 > > >&	operator=	(
							map< fixed_string< 260 >, std::pair< fixed_string< 260 > const , base_game_object* >, std::less< fixed_string< 260 > > > const&	arg_0
						) { /* no source */ }

	inline				~map<vostok::fixed_string<260>,survarium::base_game_object *,stlp_std::less<vostok::fixed_string<260> > >( ) { /* no source */ }
}; // class map< fixed_string< 260 >, base_game_object*, std::less< fixed_string< 260 > > >

STATIC_SIZE_ASSERT(map< fixed_string< 260 >, base_game_object*, std::less< fixed_string< 260 > > >, 0x18);

} // namespace survarium

#endif // #ifndef MAP_H_INCLUDED
