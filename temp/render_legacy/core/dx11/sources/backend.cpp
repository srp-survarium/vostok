////////////////////////////////////////////////////////////////////////////
//	Created		: 16.02.2009
//	Author		: Mykhailo Parfeniuk
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/core/backend.h>
#include "com_utils.h"
#include <vostok/render/core/res_declaration.h>
#include <vostok/render/core/res_xs_hw.h>
#include <vostok/render/core/render_target.h>
#include <vostok/render/core/res_sampler_list.h>

namespace vostok {
namespace render {

// REMAINDER: find_constant_host + reset_constant_update_markers (canonical
// header-inline stubs) - port with the backend.h inline pass.




// static inline bool sorted_vector_predicate( shader_constant_host const * first, shader_constant_host const * second)
// {
// 	return first->name < second->name;
// }




shader_constant_host const * backend::find_constant_host		( shared_string const& name, enum_constant_type const type , bool const create_if_not )
{
	constant_hosts::iterator it = std::lower_bound( m_constant_hosts.begin(), m_constant_hosts.end(), name, sorted_vector_predicate);

	if ( it == m_constant_hosts.end() || !(**it == name) )
	{
		if( !create_if_not)
			return  NULL;
		else
			return register_constant_host( name, type );
	}
	else
		return *it;
}

void backend::reset_constant_update_markers	()
{
	LOG_INFO("reset_constant_update_markers called !!!");
	constant_hosts::iterator		it =	m_constant_hosts.begin();
	constant_hosts::const_iterator	end =	m_constant_hosts.end();

	for( ; it!= end; ++it)
		(*it)->reset_update_markers	( );

	m_constant_update_counter = 1;
}





} // namespace render
} // namespace vostok