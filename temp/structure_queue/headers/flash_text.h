////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_TEXT_H_INCLUDED
#define FLASH_TEXT_H_INCLUDED

/* INCLUDES */
class Scaleform::GFx::DrawText;
struct survarium::flash_text_manager;
class survarium::flash_text::alignment;

namespace survarium {

struct flash_text {
	inline				flash_text		( ) { /* no source */ }

	inline	float		get_width		( ) { /* no source */ }
	inline	float		get_height		( ) { /* no source */ }

	inline	void		set_visible		( bool arg_0 ) { /* no source */ }
	inline	void		set_font_size	( const float arg_0 ) { /* no source */ }
	inline	void		set_position	( const float arg_0, const float arg_1 ) { /* no source */ }
	inline	void		set_alignment	( flash_text::alignment arg_0 ) { /* no source */ }
	inline	void		set_color		(
							u8		arg_0,
							u8		arg_1,
							u8		arg_2,
							u8		arg_3
						) { /* no source */ }
	inline	void		set_text		( pcstr arg_0 ) { /* no source */ }

public:
	/* 0x0000 */	Scaleform::GFx::DrawText*		text_impl;
	/* 0x0004 */	flash_text_manager*				owner;
	/* 0x0008 */	bool							visible;
}; // struct flash_text

STATIC_SIZE_ASSERT(flash_text, 0xC);

} // namespace survarium

#endif // #ifndef FLASH_TEXT_H_INCLUDED
