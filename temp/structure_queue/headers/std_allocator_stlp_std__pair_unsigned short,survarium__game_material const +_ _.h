////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class stlp_std::pair<unsigned short,survarium::game_material const *>;

namespace survarium {

class std_allocator< std::pair< u16, game_material const* > > {
public:
	inline				std_allocator<stlp_std::pair<unsigned short,survarium::game_material const *> >( ) { /* no source */ }

	inline	std::pair< u16, game_material const* >*	address		( std::pair< u16, game_material const* >& arg_0 ) const { /* no source */ }
	inline	std::pair< u16, game_material const* > const*	address		( std::pair< u16, game_material const* > const& arg_0 ) const { /* no source */ }

	inline	std::pair< u16, game_material const* >*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< u16, game_material const* >*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	std::pair< u16, game_material const* >*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< u16, game_material const* >*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( std::pair< u16, game_material const* >* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							std::pair< u16, game_material const* >*	arg_0,
							std::pair< u16, game_material const* > const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( std::pair< u16, game_material const* >* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< std::pair< u16, game_material const* > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< u16, game_material const* > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
