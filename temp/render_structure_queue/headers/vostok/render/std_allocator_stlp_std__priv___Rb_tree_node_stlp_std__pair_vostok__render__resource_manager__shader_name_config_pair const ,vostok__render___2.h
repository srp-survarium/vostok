////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class stlp_std::priv::_Rb_tree_node<stlp_std::pair<vostok::render::resource_manager::shader_name_config_pair const ,vostok::render::res_xs_hw<vostok::render::ps_data> *> >;

namespace vostok {
namespace render {

class std_allocator< std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > > > {
public:
	inline				std_allocator<stlp_std::priv::_Rb_tree_node<stlp_std::pair<vostok::render::resource_manager::shader_name_config_pair const ,vostok::render::res_xs_hw<vostok::render::ps_data> *> > >( ) { /* no source */ }

	inline	std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > >*	address		(
							std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > >&	arg_0
						) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > > const*	address		(
							std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > > const&	arg_0
						) const { /* no source */ }

	inline	std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > >*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > >*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > >*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > >*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	(
							std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > >*	arg_0,
							u32		arg_1
						) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > >*	arg_0,
							std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > > const&	arg_1
						) { /* no source */ }

	inline	void		destroy		(
							std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > >*	arg_0
						) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > > >

STATIC_SIZE_ASSERT(std_allocator< std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* > > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
