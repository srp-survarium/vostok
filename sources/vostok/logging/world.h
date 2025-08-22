////////////////////////////////////////////////////////////////////////////
//	Created		: 15.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOGGING_WORLD_H_INCLUDED
#define VOSTOK_LOGGING_WORLD_H_INCLUDED

namespace vostok {
namespace logging {

struct VOSTOK_NOVTABLE world {
	virtual	void	tick		( ) = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( world );
}; // class world

} // namespace logging
} // namespace vostok

#endif // #ifndef VOSTOK_LOGGING_WORLD_H_INCLUDED