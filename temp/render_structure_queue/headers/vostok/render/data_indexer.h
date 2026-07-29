////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DATA_INDEXER_H_INCLUDED
#define RENDER_DATA_INDEXER_H_INCLUDED

namespace vostok {
namespace render {

struct data_indexer {
	inline		data_indexer( u32* arg_0, u32 arg_1 ) { /* no source */ }

	/* 0x0000 */	u32*	data_ptr;
	/* 0x0004 */	u32		class_id;
}; // struct data_indexer

STATIC_SIZE_ASSERT(data_indexer, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DATA_INDEXER_H_INCLUDED
