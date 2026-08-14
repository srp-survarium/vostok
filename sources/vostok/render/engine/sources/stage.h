#ifndef VOSTOK_RENDER_ENGINE_STAGE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_H_INCLUDED

#include <boost/noncopyable.hpp>

namespace vostok {
namespace render {

class renderer;
class renderer_context;

class stage : private boost::noncopyable {
	// claude@MATCH: renderer::draw_debug reads m_context off m_stages[..] directly
	// (0x64b312: mov ecx,[edi+1Ch]; mov edx,[ecx+4]) - codegen-neutral access grant.
	friend class renderer;

public:
	stage( renderer* in_renderer, renderer_context* in_context );
	virtual ~stage( ) { }

	virtual void execute( );

	virtual void execute_disabled( )
	{
	}

	virtual void debug_render( )
	{
	}

	virtual pcstr stage_name( ) const
	{
		return "";
	}

	void set_enabled( bool enable );
	virtual bool is_enabled( ) const;

	void try_disable( )
	{
	}

	void try_enable( )
	{
	}

	static void rm_near( );
	static void rm_normal( );
	static void rm_far( );

	void u_compute_texgen_screen( float4x4& );
	void u_compute_texgen_jitter( float4x4& );

protected:
	renderer_context*	m_context;
	renderer*			m_renderer;
	bool				m_enabled;
	bool				m_prev_enabled;
};

STATIC_SIZE_ASSERT( stage, 0x10 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_STAGE_H_INCLUDED
