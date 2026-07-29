////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_SPEEDTREE_SELECTION_H_INCLUDED
#define RENDER_EFFECT_SPEEDTREE_SELECTION_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;

namespace vostok {
namespace render {

class effect_speedtree_selection : public effect_descriptor {
public:
	virtual	void	compile						( effect_compiler& compiler, custom_config_value const& custom_config ) override;

	inline			effect_speedtree_selection	( ) { /* no source */ }
	virtual			~effect_speedtree_selection	( ) { /* no source */ }
}; // class effect_speedtree_selection

STATIC_SIZE_ASSERT(effect_speedtree_selection, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_SPEEDTREE_SELECTION_H_INCLUDED
