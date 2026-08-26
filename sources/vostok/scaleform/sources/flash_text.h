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
	inline				flash_text		( ) : text_impl( NULL ), owner( NULL ), visible( false ) { }

	enum alignment {
		align_left		= 0,
		align_default	= 0,
		align_right		= 1,
		align_center	= 2,
		align_justify	= 3
	};

			float		get_width		( );
			float		get_height		( );

			void		set_visible		( bool arg_0 );
			void		set_font_size	( const float arg_0 );
			void		set_position	( const float arg_0, const float arg_1 );
	inline	void		set_alignment	( flash_text::alignment arg_0 ) { }
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
