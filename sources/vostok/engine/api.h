////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ENGINE_API_H_INCLUDED
#define VOSTOK_ENGINE_API_H_INCLUDED



#ifndef VOSTOK_ENGINE_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_ENGINE_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_ENGINE_LIBRARY_BUILDING
#			define VOSTOK_ENGINE_API				VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_ENGINE_LIBRARY_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_ENGINE_API			VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_ENGINE_API			VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_ENGINE_LIBRARY_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_ENGINE_API

namespace vostok {

namespace engine_user {
	struct module_proxy;
} // namespace engine_user

namespace engine {

VOSTOK_ENGINE_API	void	preinitialize					(
							engine_user::module_proxy& proxy,
							pcstr command_line,
							pcstr application,
							pcstr build_date
						);
VOSTOK_ENGINE_API	void	initialize						( );
VOSTOK_ENGINE_API	void	execute							( );
VOSTOK_ENGINE_API	int		get_exit_code					( );
VOSTOK_ENGINE_API	void	finalize						( );
VOSTOK_ENGINE_API	bool	command_line_editor				( );
VOSTOK_ENGINE_API	bool	command_line_no_splash_screen	( );

} // namespace engine
} // namespace vostok

#endif // #ifndef VOSTOK_ENGINE_API_H_INCLUDED