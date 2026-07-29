////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_CONSTANT_STORAGE_H_INCLUDED
#define RENDER_EFFECT_CONSTANT_STORAGE_H_INCLUDED

/* INCLUDES */
class vostok::quasi_singleton<vostok::render::effect_constant_storage>;
class vostok::render::vector<vostok::render::data_indexer>;
struct vostok::render::fixed_constants_data_buffer;

namespace vostok {
namespace render {

struct effect_constant_storage : public quasi_singleton< effect_constant_storage > {
			effect_constant_storage	( );
			~effect_constant_storage( );

	void	clear					( );

private:
	bool	is_equal				( u32 const* a_ptr, u32 const* b_ptr, const u32 num_comparision );

	/* 0x0000 */	/* quasi_singleton< effect_constant_storage > */
	/* 0x0000 */	vector< data_indexer >			m_indexers;
	/* 0x000c */	fixed_constants_data_buffer*	m_constant_buffer;
}; // struct effect_constant_storage

STATIC_SIZE_ASSERT(effect_constant_storage, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_CONSTANT_STORAGE_H_INCLUDED
