////////////////////////////////////////////////////////////////////////////
//	Created		: 09.02.2009
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef COMMAND_PROJECTION_MATRIX_H_INCLUDED
#define COMMAND_PROJECTION_MATRIX_H_INCLUDED

#include <vostok/render/base/engine_wrapper.h>

namespace vostok {
namespace render {

struct platform;
class command_projection_matrix :
	public engine::command,
	private boost::noncopyable
{



public:
	
	command_projection_matrix	( platform& platform, float4x4 const& projection );
	virtual	void	execute		( );

private:
	float4x4		m_projection;
	platform&		m_platform;
}; // class command_projection_matrix

} // namespace render
} // namespace vostok

#endif // #ifndef COMMAND_PROJECTION_MATRIX_H_INCLUDED