////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef SURVARIUM_XBOX360_APPLICATION_H_INCLUDED
#define SURVARIUM_XBOX360_APPLICATION_H_INCLUDED

#include <vostok/survarium/game_module_proxy.h>

namespace survarium {

class application {
public:
			void		initialize		( );
			void		execute			( );
			void		finalize		( );
	inline	u32			get_exit_code	( ) const { return m_exit_code; }

private:
	game_module_proxy	m_game_proxy;
	u32					m_exit_code;
}; // class application

} // namespace survarium

#endif // #ifndef SURVARIUM_XBOX360_APPLICATION_H_INCLUDED