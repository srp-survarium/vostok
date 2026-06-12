////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class stlp_std::pair<enum vostok::input::enum_keyboard,survarium::dik_to_swf_bind>;

namespace survarium {

class std_allocator< std::pair< enum input::enum_keyboard, dik_to_swf_bind > > {
public:
	inline				std_allocator<stlp_std::pair<enum vostok::input::enum_keyboard,survarium::dik_to_swf_bind> >( ) { /* no source */ }

	inline	std::pair< enum input::enum_keyboard, dik_to_swf_bind >*	address		( std::pair< enum input::enum_keyboard, dik_to_swf_bind >& arg_0 ) const { /* no source */ }
	inline	std::pair< enum input::enum_keyboard, dik_to_swf_bind > const*	address		( std::pair< enum input::enum_keyboard, dik_to_swf_bind > const& arg_0 ) const { /* no source */ }

	inline	std::pair< enum input::enum_keyboard, dik_to_swf_bind >*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< enum input::enum_keyboard, dik_to_swf_bind >*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	std::pair< enum input::enum_keyboard, dik_to_swf_bind >*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< enum input::enum_keyboard, dik_to_swf_bind >*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( std::pair< enum input::enum_keyboard, dik_to_swf_bind >* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							std::pair< enum input::enum_keyboard, dik_to_swf_bind >*	arg_0,
							std::pair< enum input::enum_keyboard, dik_to_swf_bind > const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( std::pair< enum input::enum_keyboard, dik_to_swf_bind >* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< std::pair< enum input::enum_keyboard, dik_to_swf_bind > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< enum input::enum_keyboard, dik_to_swf_bind > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
