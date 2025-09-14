#include "pch.h"
#include "about_dialog.h"

#pragma managed( push, off )
namespace vostok {
namespace build {
	VOSTOK_CORE_API	u32		calculate_build_id			( pcstr current_date );
	VOSTOK_CORE_API	pcstr	build_date					( );
	VOSTOK_CORE_API	u32		build_station_build_id		( );
} // namespace build
} // namespace vostok
#pragma managed( pop )

using namespace System;

namespace vostok {
namespace editor {

void about_dialog::button_ok_Click		(System::Object^  sender, System::EventArgs^  e)
{
	VOSTOK_UNREFERENCED_PARAMETERS			( sender, e );
	this->Close();
}

void about_dialog::about_dialog_Load	(System::Object^  sender, System::EventArgs^  e)
{
	VOSTOK_UNREFERENCED_PARAMETERS		( sender, e );

	u32 const build_station_build_id	= vostok::build::build_station_build_id();
	label1->Text	= 
		"              X-Ray Editor v2.0a\r\n"
		"\r\n"
		"         build #" +
		build_station_build_id + ", " + gcnew System::String( vostok::build::build_date() ) +
		"\r\n"
		"\r\n"
		"Copyright(C) GSC Game World - 2011";
}

} // namespace editor 
} // namespace vostok