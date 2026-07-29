////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class stlp_std::pair<vostok::render::render_model_instance *,vostok::render::material_effects>;

namespace vostok {
namespace render {

class std_allocator< std::pair< render_model_instance*, material_effects > > {
public:
	inline				std_allocator<stlp_std::pair<vostok::render::render_model_instance *,vostok::render::material_effects> >( ) { /* no source */ }

	inline	std::pair< render_model_instance*, material_effects >*	address		( std::pair< render_model_instance*, material_effects >& arg_0 ) const { /* no source */ }
	inline	std::pair< render_model_instance*, material_effects > const*	address		( std::pair< render_model_instance*, material_effects > const& arg_0 ) const { /* no source */ }

	inline	std::pair< render_model_instance*, material_effects >*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< render_model_instance*, material_effects >*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	std::pair< render_model_instance*, material_effects >*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< render_model_instance*, material_effects >*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( std::pair< render_model_instance*, material_effects >* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							std::pair< render_model_instance*, material_effects >*	arg_0,
							std::pair< render_model_instance*, material_effects > const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( std::pair< render_model_instance*, material_effects >* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< std::pair< render_model_instance*, material_effects > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< render_model_instance*, material_effects > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
