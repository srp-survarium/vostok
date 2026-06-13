////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_TEXT_H_INCLUDED
#define FLASH_TEXT_H_INCLUDED

namespace Scaleform {
namespace GFx {
	class DrawText;
} // namespace GFx
} // namespace Scaleform

namespace survarium {

struct flash_text_manager;

struct flash_text {
	// nested enum has no PDB type record (referenced only); enumerators mirror
	// Scaleform::GFx::DrawText::Alignment, unverifiable from the PDB
	enum alignment {
		align_left,
		align_default,
		align_right,
		align_center,
		align_justify
	};

	// all out-of-line in movie.cpp per the rich index, except the ctor and
	// set_alignment (unaddressed - true inlines)
	inline				flash_text		( ) { /* no source */ }

			float		get_width		( );
			float		get_height		( );

			void		set_visible		( bool arg_0 );
			void		set_font_size	( const float arg_0 );
			void		set_position	( const float arg_0, const float arg_1 );
	inline	void		set_alignment	( flash_text::alignment arg_0 ) { /* no source */ }
			void		set_color		(
							u8		arg_0,
							u8		arg_1,
							u8		arg_2,
							u8		arg_3
						);
			void		set_text		( pcstr arg_0 );

public:
	/* 0x0000 */	Scaleform::GFx::DrawText*		text_impl;
	/* 0x0004 */	flash_text_manager*				owner;
	/* 0x0008 */	bool							visible;
}; // struct flash_text

STATIC_SIZE_ASSERT(flash_text, 0xC);

} // namespace survarium

#endif // #ifndef FLASH_TEXT_H_INCLUDED
