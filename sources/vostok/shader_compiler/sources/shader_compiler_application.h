////////////////////////////////////////////////////////////////////////////
//	Created		: 18.05.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SHADER_COMPILER_APPLICATION_H_INCLUDED
#define SHADER_COMPILER_APPLICATION_H_INCLUDED

#include <vostok\memory_extensions.h>

namespace vostok {
namespace shader_compiler {



class application {
public:
			void	initialize		( );
			void	execute			( );
			void	finalize		( );
	inline	u32		get_exit_code	( ) const	{ return m_exit_code; }

private:
	u32		m_exit_code;
}; // class application

} // namespace shader_compiler
} // namespace vostok

#endif // #ifndef SHADER_COMPILER_APPLICATION_H_INCLUDED