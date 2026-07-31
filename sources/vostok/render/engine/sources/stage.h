#ifndef VOSTOK_RENDER_ENGINE_STAGE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_STAGE_H_INCLUDED

#include <boost/noncopyable.hpp>

namespace vostok {
namespace render {

class renderer;
class renderer_context;

class stage : public boost::noncopyable {
public:
	stage( renderer* in_renderer, renderer_context* in_context );
	virtual ~stage( ) { }

	virtual void execute( );

	virtual void execute_disabled( )
	{
		// claude@NOTE: no legacy ancestor - the surviving legacy engine/stage.h is the dead render_dx10 generation; the same-generation stage.h was consumed with only an empty execute_disabled; matcher-phase work.
		// STATE[STUB]
		// FUNCTION BODY[0x4f210]
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

	void u_compute_texgen_screen( float4x4& );
	void u_compute_texgen_jitter( float4x4& );

	static void rm_near( );
	static void rm_normal( );
	static void rm_far( );

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
