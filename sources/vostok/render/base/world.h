////////////////////////////////////////////////////////////////////////////
//	Created 	: 18.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_BASE_WORLD_H_INCLUDED
#define VOSTOK_RENDER_BASE_WORLD_H_INCLUDED

#include <vostok/render/world.h>
#include <vostok/render/base/api.h>

#ifdef _MSC_VER
template class VOSTOK_RENDER_BASE_API vostok::intrusive_ptr< vostok::resources::managed_resource, 
												  vostok::resources::managed_intrusive_base, 
												  vostok::threading::multi_threading_mutex_policy >;
#endif // #ifdef _MSC_VER

namespace vostok {
namespace render {

namespace extended	{	struct renderer;	}
namespace engine	{	struct renderer;	struct wrapper;		}
namespace game		{	struct renderer;	}
namespace ui		{	struct renderer;	}
namespace editor	{	struct renderer;	}

struct platform;
struct buffer_fragment;
struct buffer_fragment_NEW;
struct terrain_data;
class command_processor;

class VOSTOK_RENDER_BASE_API base_world : public render::world {
public:
								base_world					( engine::wrapper& wrapper, HWND window_handle );
	virtual						~base_world					( );
	virtual	engine::renderer&	engine						( );
	virtual	game::renderer&		game						( );
	virtual	ui::renderer&		ui							( );
	virtual	editor::renderer&	editor						( );
			render::platform&	platform					( );

	virtual	void				clear_resources				( );
	virtual	void				set_editor_render_mode		( bool draw_editor, bool draw_game);

	virtual	void				push_command				( engine::command* command );
	virtual	void				destroy_command				( engine::command* command );

	virtual	void				push_command_editor			( engine::command* command );
	virtual	void				destroy_command_editor		( engine::command* command );

public:
	inline	command_processor&	commands					( ) const { ASSERT( m_processor ); return ( *m_processor ); }
	inline	command_processor&	commands_editor				( ) const { ASSERT( m_editor_commands_processor ); return ( *m_editor_commands_processor ); }

			void				draw_frame_logic			( );
			void				draw_frame_editor			( );
			

protected:
	inline	engine::wrapper&	wrapper						( ) const { return m_wrapper; }
	
private:
								base_world					( base_world const& other );
			world&				operator =					( base_world const& other );

private:
	resources::managed_resource_ptr		m_static;
	command_processor*			m_processor;
	command_processor*			m_editor_commands_processor;
	engine::wrapper&			m_wrapper;
	engine::renderer*			m_engine;
	game::renderer*				m_game;
	ui::renderer*				m_ui;
	editor::renderer*			m_editor;
	render::platform*			m_platform;
}; // class base_world

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_BASE_WORLD_H_INCLUDED