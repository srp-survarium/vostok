////////////////////////////////////////////////////////////////////////////
//	Created		: 04.02.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef STAGE_FR_TEST_H_INCLUDED
#define STAGE_FR_TEST_H_INCLUDED

#include "stage.h"
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/res_declaration.h>

namespace vostok {
namespace render {

class res_effect;
class resource_intrusive_base;
typedef	vostok::resources::resource_ptr < vostok::render::res_effect, vostok::resources::unmanaged_intrusive_base > ref_effect;

class res_declaration;
typedef vostok::intrusive_ptr<res_declaration, resource_intrusive_base, threading::single_threading_policy>	ref_declaration;

class stage_fr_test: public stage
{
public:
	stage_fr_test	( renderer_context* context);
	~stage_fr_test	( ){}

	void execute( );

private:
	ref_effect			m_sh;
	ref_declaration		m_decl_ptr;
}; // class stage_fr_test

} // namespace render
} // namespace vostok


#endif // #ifndef STAGE_TEXTURE_MIP_CLAMP_TEST_H_INCLUDED