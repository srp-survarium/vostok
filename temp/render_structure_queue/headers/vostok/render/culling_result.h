////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_RESULT_H_INCLUDED
#define RENDER_CULLING_RESULT_H_INCLUDED

namespace vostok {
namespace render {

struct culling_result {

	/* 0x0000 */	void*		user_data;
}; // struct culling_result

STATIC_SIZE_ASSERT(culling_result, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_RESULT_H_INCLUDED
