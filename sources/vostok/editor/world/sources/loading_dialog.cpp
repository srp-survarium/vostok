#include "pch.h"
#include "loading_dialog.h"
//#include "editor_world.h"

namespace vostok {
namespace editor {

System::Void loading_dialog::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	VOSTOK_UNREFERENCED_PARAMETERS(sender,e);
	//VOSTOK_UNREFERENCED_PARAMETERS	( sender, e );
	//progressBar1->Maximum	= m_world.project_items_requested();
	//progressBar1->Value		= m_world.project_items_loaded();

	//label_elements_loaded->Text	= m_world.project_items_loaded() + " of " + m_world.project_items_requested() + " Loaded";


	//if( m_world.is_project_loaded() || !m_world.is_project_loading() )
	//	Close();
}

} // namespace editor
} // namespace vostok