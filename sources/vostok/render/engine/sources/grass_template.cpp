#include "pch.h"
#include "grass_template.h"
#include "grass_instance.h"

namespace vostok {
namespace render {

grass_template::grass_template( u32 const in_index, grass_render_model_ptr const& in_model_ptr ) :
	m_render_model( in_model_ptr )
{
	m_index = in_index;
	m_sizes = float3( 0.f, 0.f, 0.f );
}

grass_template::~grass_template( )
{
	vector<grass_instance*>::iterator	it_i		=	m_instances.begin();
	vector<grass_instance*>::iterator	end_i		=	m_instances.end();

	for (; it_i != end_i; ++it_i)
	{
		grass_instance* inst					=	(*it_i);
		DELETE									(inst);
	}
}

} // namespace render
} // namespace vostok
