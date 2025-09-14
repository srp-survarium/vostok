////////////////////////////////////////////////////////////////////////////
//	Created 	: 31.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_EDITOR_WORLD_H_INCLUDED
#define VOSTOK_EDITOR_WORLD_H_INCLUDED

namespace vostok {
namespace editor_base {
	class property_holder;
	class property_holder_collection;
	class property_holder_holder;
} // namespace editor_base
namespace editor {

struct VOSTOK_NOVTABLE engine;

struct VOSTOK_NOVTABLE world {
	virtual	void	load			( ) = 0;
	virtual	void	run				( ) = 0;
	virtual	void	clear_resources	( ) = 0;
	virtual	HWND	main_handle		( ) = 0;
	virtual	HWND	view_handle		( ) = 0;
	virtual	void	editor_mode		( bool beditor_mode) = 0;
	virtual	void	render			( u32 frame_id, math::float4x4 view_matrix ) = 0;
	virtual	int		exit_code		( ) const = 0;
	virtual void	message_out		( pcstr message ) = 0;
protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( world )
}; // class world

} // namespace editor
} // namespace vostok

#endif // #ifndef VOSTOK_EDITOR_WORLD_H_INCLUDED