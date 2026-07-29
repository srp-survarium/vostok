////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class stlp_std::priv::_Rb_tree_node<vostok::render::shader_constant_table *>;

namespace vostok {
namespace render {

class std_allocator< std::priv::_Rb_tree_node< shader_constant_table* > > {
public:
	inline				std_allocator<stlp_std::priv::_Rb_tree_node<vostok::render::shader_constant_table *> >( ) { /* no source */ }

	inline	std::priv::_Rb_tree_node< shader_constant_table* >*	address		( std::priv::_Rb_tree_node< shader_constant_table* >& arg_0 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< shader_constant_table* > const*	address		( std::priv::_Rb_tree_node< shader_constant_table* > const& arg_0 ) const { /* no source */ }

	inline	std::priv::_Rb_tree_node< shader_constant_table* >*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< shader_constant_table* >*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	std::priv::_Rb_tree_node< shader_constant_table* >*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< shader_constant_table* >*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( std::priv::_Rb_tree_node< shader_constant_table* >* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							std::priv::_Rb_tree_node< shader_constant_table* >*	arg_0,
							std::priv::_Rb_tree_node< shader_constant_table* > const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( std::priv::_Rb_tree_node< shader_constant_table* >* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< std::priv::_Rb_tree_node< shader_constant_table* > >

STATIC_SIZE_ASSERT(std_allocator< std::priv::_Rb_tree_node< shader_constant_table* > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
