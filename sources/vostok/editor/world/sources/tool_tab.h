////////////////////////////////////////////////////////////////////////////
//	Created		: 13.01.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef TOOL_TAB_H_INCLUDED
#define TOOL_TAB_H_INCLUDED

namespace vostok {
namespace editor {

public interface class tool_tab
{
public:
	System::Windows::Forms::UserControl^	frame			( ) abstract;
	void									on_activate		( ) abstract;
	void									on_deactivate	( ) abstract;
}; // class tool_tab

} // namespace editor
} // namespace vostok

#endif // #ifndef TOOL_BASE_TAB_H_INCLUDED