////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

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

class flash_external_handler;
class flash_function_handler;
class flash_value;

struct flash_movie {
	// nested enums have no PDB type record (referenced only); AlignType and
	// ScaleModeType wrap Scaleform::GFx::Movie's, the *_btn_action values are guesses
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

	enum keyb_btn_action {
		keyb_btn_down,
		keyb_btn_up
	};

	enum mouse_btn_action {
		mouse_btn_down,
		mouse_btn_up
	};

	inline			flash_movie			( ) { /* no source */ }

	inline	void	SetViewport			( u32 arg_0, u32 arg_1 ) { /* no source */ }

	inline	void	Advance				( const float arg_0, const u32 arg_1 ) { /* no source */ }

	inline	void	SetBackgroundAlpha	( const float arg_0 ) { /* no source */ }

	inline	void	SetViewAlignment	( flash_movie::AlignType arg_0 ) { /* no source */ }

	inline	void	SetViewScaleMode	( flash_movie::ScaleModeType arg_0 ) { /* no source */ }

	inline	void	SetVariable			( pcstr arg_0, pcstr arg_1 ) { /* no source */ }
	inline	void	SetVariable			( pcstr arg_0, flash_value const& arg_1 ) { /* no source */ }

	inline	void	Restart				( ) { /* no source */ }

	inline	void	SetExternalInterface( flash_external_handler* arg_0 ) { /* no source */ }

	inline	void	HandleMouseMove		( const float arg_0, const float arg_1, const float arg_2 ) { /* no source */ }

	inline	void	HandleMouseBtn		(
						flash_movie::mouse_btn_action		arg_0,
						u32									arg_1,
						const float							arg_2,
						const float							arg_3
					) { /* no source */ }

	inline	void	HandleKeyboard		( flash_movie::keyb_btn_action arg_0, s32 arg_1 ) { /* no source */ }

	inline	void	HandleChar			( wchar_t arg_0 ) { /* no source */ }

	inline	void	GetVariable			( flash_value* arg_0, pcstr arg_1 ) { /* no source */ }

	inline	void	CreateArray			( flash_value* arg_0 ) { /* no source */ }

	inline	void	CreateObject		( flash_value* arg_0 ) { /* no source */ }

	inline	void	CreateFunction		( flash_value* arg_0, flash_function_handler* arg_1 ) { /* no source */ }

	inline	void	ForceCollectGarbage	( ) { /* no source */ }

	inline	bool	Invoke				(
						pcstr					arg_0,
						flash_value*			arg_1,
						flash_value const*		arg_2,
						u32						arg_3
					) { /* no source */ return false; }

	inline	void	SetPriority			( u8 arg_0 ) { /* no source */ }

	inline	u8		GetPriority			( ) { /* no source */ return 0; }

public:
	/* 0x0000 */	Scaleform::GFx::MovieDef*		m_movie_def;
	/* 0x0004 */	Scaleform::GFx::Movie*			m_movie;
	/* 0x0008 */	Scaleform::Render::ContextImpl::DisplayHandle< Scaleform::Render::TreeRoot > const*	m_handle;
	/* 0x000c */	u32								m_output_width;
	/* 0x0010 */	u32								m_output_height;
	/* 0x0014 */	u32								m_last_keyb_hold_time;
	/* 0x0018 */	u8								m_priority;
}; // struct flash_movie

STATIC_SIZE_ASSERT(flash_movie, 0x1C);

} // namespace survarium

#endif // #ifndef FLASH_MOVIE_H_INCLUDED
