////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_NETWORK_WORLD_H_INCLUDED
#define VOSTOK_NETWORK_WORLD_H_INCLUDED

namespace vostok {
namespace network {

struct VOSTOK_NOVTABLE world {
	virtual	void	initialize			( ) = 0;
	virtual	void	finalize			( ) = 0;
	virtual	void	tick				( bool single_threaded ) = 0;
	virtual	void	clear_resources		( ) = 0;
	virtual	void	dispatch_callbacks	( ) = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( world );
}; // struct world

STATIC_SIZE_ASSERT(world, 0x4);

} // namespace network
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_WORLD_H_INCLUDED
