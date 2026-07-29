////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class stlp_std::priv::_Rb_tree_node<vostok::render::res_input_layout *>;

namespace vostok {
namespace render {

class std_allocator< std::priv::_Rb_tree_node< res_input_layout* > > {
public:
	inline				std_allocator<stlp_std::priv::_Rb_tree_node<vostok::render::res_input_layout *> >( ) { /* no source */ }

	inline	std::priv::_Rb_tree_node< res_input_layout* >*	address		( std::priv::_Rb_tree_node< res_input_layout* >& arg_0 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< res_input_layout* > const*	address		( std::priv::_Rb_tree_node< res_input_layout* > const& arg_0 ) const { /* no source */ }

	inline	std::priv::_Rb_tree_node< res_input_layout* >*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< res_input_layout* >*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	std::priv::_Rb_tree_node< res_input_layout* >*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< res_input_layout* >*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( std::priv::_Rb_tree_node< res_input_layout* >* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							std::priv::_Rb_tree_node< res_input_layout* >*	arg_0,
							std::priv::_Rb_tree_node< res_input_layout* > const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( std::priv::_Rb_tree_node< res_input_layout* >* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< std::priv::_Rb_tree_node< res_input_layout* > >

STATIC_SIZE_ASSERT(std_allocator< std::priv::_Rb_tree_node< res_input_layout* > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
