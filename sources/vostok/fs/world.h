////////////////////////////////////////////////////////////////////////////
//	Created		: 15.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_FS_WORLD_H_INCLUDED
#define VOSTOK_FS_WORLD_H_INCLUDED

namespace vostok {
namespace fs_new {

struct VOSTOK_NOVTABLE world {
	virtual	void	tick		( ) = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( world );
}; // class world

} // namespace fs_new
} // namespace vostok

#endif // #ifndef VOSTOK_FS_WORLD_H_INCLUDED