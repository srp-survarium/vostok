////////////////////////////////////////////////////////////////////////////
//	Created		: 15.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_VFS_WORLD_H_INCLUDED
#define VOSTOK_VFS_WORLD_H_INCLUDED

namespace vostok {
namespace vfs {

struct VOSTOK_NOVTABLE world {
	virtual	void	tick		( ) = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( world );
}; // class world

} // namespace vfs
} // namespace vostok

#endif // #ifndef VOSTOK_VFS_WORLD_H_INCLUDED