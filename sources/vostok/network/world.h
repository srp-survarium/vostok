////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_NETWORK_WORLD_H_INCLUDED
#define VOSTOK_NETWORK_WORLD_H_INCLUDED

namespace vostok {
namespace network {

struct handler;

struct VOSTOK_NOVTABLE world {
	virtual	void	tick			( u32 const logic_frame_id ) = 0;
	virtual	void	clear_resources	( ) = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( world )
}; // class world

} // namespace network
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_WORLD_H_INCLUDED