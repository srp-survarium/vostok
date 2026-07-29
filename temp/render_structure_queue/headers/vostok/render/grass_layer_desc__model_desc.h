////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_LAYER_DESC_MODEL_DESC_H_INCLUDED
#define RENDER_GRASS_LAYER_DESC_MODEL_DESC_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<260>;

namespace vostok {
namespace render {

struct grass_layer_desc::model_desc {
	inline		model_desc	( ) { /* no source */ }

	/* 0x0000 */	fixed_string< 260 >		name;
	/* 0x0110 */	float					probability_;
	/* 0x0114 */	float					scale;
}; // struct grass_layer_desc::model_desc

STATIC_SIZE_ASSERT(grass_layer_desc::model_desc, 0x118);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_LAYER_DESC_MODEL_DESC_H_INCLUDED
