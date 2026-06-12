////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class stlp_std::priv::_Rb_tree_node<stlp_std::pair<unsigned short const ,survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> > > >;

namespace survarium {

class std_allocator< std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > > > {
public:
	inline				std_allocator<stlp_std::priv::_Rb_tree_node<stlp_std::pair<unsigned short const ,survarium::map<unsigned short,survarium::material_pair const *,stlp_std::less<unsigned short> > > > >( ) { /* no source */ }

	inline	std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > >*	address		(
							std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > >&	arg_0
						) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > > const*	address		(
							std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > > const&	arg_0
						) const { /* no source */ }

	inline	std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > >*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > >*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > >*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > >*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	(
							std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > >*	arg_0,
							u32		arg_1
						) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > >*	arg_0,
							std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > > const&	arg_1
						) { /* no source */ }

	inline	void		destroy		(
							std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > >*	arg_0
						) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > > >

STATIC_SIZE_ASSERT(std_allocator< std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
