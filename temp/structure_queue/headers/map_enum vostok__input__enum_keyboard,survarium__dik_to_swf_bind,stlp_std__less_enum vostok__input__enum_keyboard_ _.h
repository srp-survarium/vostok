////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<enum vostok::input::enum_keyboard,survarium::dik_to_swf_bind,stlp_std::less<enum vostok::input::enum_keyboard>,survarium::std_allocator<stlp_std::pair<enum vostok::input::enum_keyboard,survarium::dik_to_swf_bind> > >;

/* FORWARD REFS */
class stlp_std::less<enum vostok::input::enum_keyboard>;
class survarium::map<enum vostok::input::enum_keyboard,stlp_std::pair<enum vostok::input::enum_keyboard const ,survarium::dik_to_swf_bind>,stlp_std::less<enum vostok::input::enum_keyboard> >;

namespace survarium {

class map< enum input::enum_keyboard, dik_to_swf_bind, std::less< enum input::enum_keyboard > > : public std::map< enum input::enum_keyboard, dik_to_swf_bind, std::less< enum input::enum_keyboard >, std_allocator< std::pair< enum input::enum_keyboard, dik_to_swf_bind > > > {
public:
	inline				map<enum vostok::input::enum_keyboard,survarium::dik_to_swf_bind,stlp_std::less<enum vostok::input::enum_keyboard> >( ) { /* no source */ }
	inline	explicit	map<enum vostok::input::enum_keyboard,survarium::dik_to_swf_bind,stlp_std::less<enum vostok::input::enum_keyboard> >( std::less< enum input::enum_keyboard > const& arg_0 ) { /* no source */ }
	inline	explicit	map<enum vostok::input::enum_keyboard,survarium::dik_to_swf_bind,stlp_std::less<enum vostok::input::enum_keyboard> >(
							map< enum input::enum_keyboard, std::pair< enum input::enum_keyboard const , dik_to_swf_bind >, std::less< enum input::enum_keyboard > > const&	arg_0
						) { /* no source */ }

	inline	map< enum input::enum_keyboard, std::pair< enum input::enum_keyboard const , dik_to_swf_bind >, std::less< enum input::enum_keyboard > >&	operator=	(
							map< enum input::enum_keyboard, std::pair< enum input::enum_keyboard const , dik_to_swf_bind >, std::less< enum input::enum_keyboard > > const&	arg_0
						) { /* no source */ }

	inline				~map<enum vostok::input::enum_keyboard,survarium::dik_to_swf_bind,stlp_std::less<enum vostok::input::enum_keyboard> >( ) { /* no source */ }
}; // class map< enum input::enum_keyboard, dik_to_swf_bind, std::less< enum input::enum_keyboard > >

STATIC_SIZE_ASSERT(map< enum input::enum_keyboard, dik_to_swf_bind, std::less< enum input::enum_keyboard > >, 0x18);

} // namespace survarium

#endif // #ifndef MAP_H_INCLUDED
