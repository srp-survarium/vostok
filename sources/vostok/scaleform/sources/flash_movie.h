// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLASH_MOVIE_H_INCLUDED
#define FLASH_MOVIE_H_INCLUDED

namespace Scaleform {
namespace Render {
	class TreeRoot;
	namespace ContextImpl {
		template < class C > class DisplayHandle;
	} // namespace ContextImpl
	using ContextImpl::DisplayHandle;
} // namespace Render
namespace GFx {
	class Movie;
	class MovieDef;
} // namespace GFx
} // namespace Scaleform

namespace survarium {

struct flash_external_handler;
struct flash_function_handler;
struct flash_value;

struct flash_movie {
	enum AlignType {
		Align_Center,
		Align_TopCenter,
		Align_BottomCenter,
		Align_CenterLeft,
		Align_CenterRight,
		Align_TopLeft,
		Align_TopRight,
		Align_BottomLeft,
		Align_BottomRight
	};

	enum ScaleModeType {
		SM_NoScale,
		SM_ShowAll,
		SM_ExactFit,
		SM_NoBorder
	};

	enum mouse_btn_action {
		ms_key_down,
		ms_key_up,
		ms_key_hold,
		ms_key_click,
		ms_key_dbl_click
	};

	enum keyb_btn_action {
		kb_key_unknown,
		kb_key_down,
		kb_key_up,
		kb_key_hold
	};

	inline			flash_movie			( )
		:	m_output_width			( 0 ),
			m_output_height			( 0 ),
			m_last_keyb_hold_time	( 0 ),
			m_priority				( 0 )
	{}
			void	SetViewport			( u32 arg_0, u32 arg_1 );

			void	Advance				( const float arg_0, const u32 arg_1 );

			void	SetBackgroundAlpha	( const float arg_0 );

			void	SetViewAlignment	( flash_movie::AlignType arg_0 );

			void	SetViewScaleMode	( flash_movie::ScaleModeType arg_0 );

			void	SetVariable			( pcstr arg_0, pcstr arg_1 );
			void	SetVariable			( pcstr arg_0, flash_value const& arg_1 );

			void	Restart				( );

			void	SetExternalInterface( flash_external_handler* arg_0 );

			void	HandleMouseMove		( const float arg_0, const float arg_1, const float arg_2 );

			void	HandleMouseBtn		(
						flash_movie::mouse_btn_action		arg_0,
						u32									arg_1,
						const float							arg_2,
						const float							arg_3
					);

			void	HandleKeyboard		( flash_movie::keyb_btn_action arg_0, s32 arg_1 );

			void	HandleChar			( wchar_t arg_0 );

			void	GetVariable			( flash_value* arg_0, pcstr arg_1 );

			void	CreateArray			( flash_value* arg_0 );

			void	CreateObject		( flash_value* arg_0 );

			void	CreateFunction		( flash_value* arg_0, flash_function_handler* arg_1 );

			void	ForceCollectGarbage	( );

			bool	Invoke				(
						pcstr					arg_0,
						flash_value*			arg_1,
						flash_value const*		arg_2,
						u32						arg_3
					);

	inline	void	SetPriority			( u8 arg_0 ) { m_priority = arg_0; }

	inline	u8		GetPriority			( ) { return m_priority; }

public:
	/* 0x0000 */	Scaleform::GFx::MovieDef*		m_movie_def;
	/* 0x0004 */	Scaleform::GFx::Movie*			m_movie;
	/* 0x0008 */	Scaleform::Render::ContextImpl::DisplayHandle< Scaleform::Render::TreeRoot > const*	m_handle;
	/* 0x000c */	u32								m_output_width;
	/* 0x0010 */	u32								m_output_height;
	/* 0x0014 */	u32								m_last_keyb_hold_time;
private:
	/* 0x0018 */	u8								m_priority;
}; // struct flash_movie

STATIC_SIZE_ASSERT(flash_movie, 0x1C);

} // namespace survarium

#endif // #ifndef FLASH_MOVIE_H_INCLUDED
