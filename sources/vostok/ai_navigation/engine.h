////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_NAVIGATION_ENGINE_H_INCLUDED
#define VOSTOK_AI_NAVIGATION_ENGINE_H_INCLUDED

namespace vostok {
namespace ai {
namespace navigation {

struct VOSTOK_NOVTABLE engine
{
	virtual	void	set_navmesh_info	( pcstr text ) const = 0;
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( engine )
}; // class engine

} // namespace navigation
} // namespace ai
} // namespace vostok

#endif // #ifndef VOSTOK_AI_NAVIGATION_ENGINE_H_INCLUDED