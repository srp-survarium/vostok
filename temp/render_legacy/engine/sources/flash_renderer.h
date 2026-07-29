////////////////////////////////////////////////////////////////////////////
//	Created		: 07.03.2012
//	Author		: Plichko Alexander
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_RENDERER_H_INCLUDED
#define FLASH_RENDERER_H_INCLUDED

namespace Scaleform
{
	namespace Render{
		class Renderer2D;
		namespace D3D1x
		{
			class HAL;
		}
	}
}

namespace survarium
{
	struct flash_movie;
}

namespace vostok {
namespace render {

struct flash_renderer {
public:
	flash_renderer();
	~flash_renderer();
	void present			( );
	void on_reset_device	( );
	void on_create_device	( );
	void on_lost_device		( );

	void show_movie			( survarium::flash_movie* movie );
	void hide_movie			( survarium::flash_movie* movie );

private:
	Scaleform::Render::D3D1x::HAL*	m_HALRenderer;
	Scaleform::Render::Renderer2D*	m_R2dRenderer;

	vector<survarium::flash_movie*>	m_movies;
	
//	DWORD							m_render_last_time;

}; // class flash_renderer

} // namespace render
} // namespace vostok

#endif // #ifndef FLASH_RENDERER_H_INCLUDED