////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_OPTIONS_DESCRIPTOR_CONST_ITERATOR_H_INCLUDED
#define RENDER_EFFECT_OPTIONS_DESCRIPTOR_CONST_ITERATOR_H_INCLUDED

/* INCLUDES */
struct vostok::render::effect_options_descriptor;
class vostok::render::effect_options_descriptor const* const;

namespace vostok {
namespace render {

struct effect_options_descriptor::const_iterator {
	inline										const_iterator	( ) { /* no source */ }
	inline	explicit							const_iterator	( effect_options_descriptor const* const arg_0 ) { /* no source */ }

	inline	bool								operator bool	( ) { /* no source */ }

	inline	void								operator++		( ) { /* no source */ }

	inline	effect_options_descriptor const&	operator*		( ) { /* no source */ }

	inline	effect_options_descriptor const*	operator->		( ) { /* no source */ }

	inline	bool								operator!=		( effect_options_descriptor::const_iterator const& arg_0 ) { /* no source */ }

private:
	/* 0x0000 */	effect_options_descriptor*		value;
	/* 0x0004 */	u32								child_index;
	/* 0x0008 */	u32								size;
}; // struct effect_options_descriptor::const_iterator

STATIC_SIZE_ASSERT(effect_options_descriptor::const_iterator, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_OPTIONS_DESCRIPTOR_CONST_ITERATOR_H_INCLUDED
