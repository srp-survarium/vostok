////////////////////////////////////////////////////////////////////////////
//	Created		: 28.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_environment.h"
#include <vostok/collision/api.h>

namespace vostok {
namespace sound {

sound_environment::sound_environment	( u32 env_params_id ) :
	m_env_params_id					( env_params_id )
{
	struct constructor_locals {
		float4x4						source_matrix;
		float4x4						matrix;
		float							diagonal_i;
		float							diagonal_j;
		float							diagonal_k;
		collision::geometry_instance*	instance;
	} locals;

	locals.diagonal_i							= 1.f;
	locals.diagonal_j							= 1.f;
	locals.diagonal_k							= 1.f;
	memset										( &locals.source_matrix, 0, sizeof(locals.source_matrix) );
	locals.source_matrix.e00					= locals.diagonal_i;
	locals.source_matrix.e11					= locals.diagonal_j;
	locals.source_matrix.e22					= locals.diagonal_k;
	locals.source_matrix.e33					= 1.f;

	locals.matrix								 = locals.source_matrix;
	locals.instance								 = &*collision::new_box_geometry_instance
													( g_allocator, locals.matrix );
	m_collision								= &*collision::new_collision_object
												( g_allocator, 1, locals.instance, this );
}

sound_environment::~sound_environment	( )
{
}

} // namespace sound
} // namespace vostok
