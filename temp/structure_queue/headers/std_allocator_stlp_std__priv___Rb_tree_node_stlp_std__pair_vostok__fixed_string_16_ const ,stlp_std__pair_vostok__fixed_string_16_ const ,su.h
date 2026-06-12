////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class stlp_std::priv::_Rb_tree_node<stlp_std::pair<vostok::fixed_string<16> const ,stlp_std::pair<vostok::fixed_string<16> const ,survarium::hit_type_parameters_modifyer> > >;

namespace survarium {

class std_allocator< std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > > > {
public:
	inline				std_allocator<stlp_std::priv::_Rb_tree_node<stlp_std::pair<vostok::fixed_string<16> const ,stlp_std::pair<vostok::fixed_string<16> const ,survarium::hit_type_parameters_modifyer> > > >( ) { /* no source */ }

	inline	std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > >*	address		(
							std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > >&	arg_0
						) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > > const*	address		(
							std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > > const&	arg_0
						) const { /* no source */ }

	inline	std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > >*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > >*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > >*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > >*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	(
							std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > >*	arg_0,
							u32		arg_1
						) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > >*	arg_0,
							std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > > const&	arg_1
						) { /* no source */ }

	inline	void		destroy		(
							std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > >*	arg_0
						) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > > >

STATIC_SIZE_ASSERT(std_allocator< std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
