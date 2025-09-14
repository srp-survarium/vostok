////////////////////////////////////////////////////////////////////////////
//	Created 	: 23.01.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PROPERTY_GRID_HPP_INCLUDED
#define PROPERTY_GRID_HPP_INCLUDED

#include <vostok/editor/controls/property_value.h>
#include <vostok/editor/controls/property_mouse_events.h>

namespace vostok {
namespace editor {
namespace controls {

public ref class property_grid : public System::Windows::Forms::PropertyGrid
{
private:
	typedef System::Windows::Forms::Control			Control;
	typedef System::Object							Object;
	typedef Microsoft::Win32::RegistryKey			RegistryKey;
	typedef System::String							String;
	typedef System::Drawing::Point					Point;

public:
					property_grid					();
			void	initialize_grid_view			();
			void	OnChildControlMouseDoubleClick	(Object^ object, System::Windows::Forms::MouseEventArgs^ e);
			void	OnChildControlMouseMove			(Object^ object, System::Windows::Forms::MouseEventArgs^ e);
			void	OnChildControlMouseDown			(Object^ object, System::Windows::Forms::MouseEventArgs^ e);
			void	setup_event_handlers			();
			void	save							(RegistryKey^ root, String^ key);
			void	load							(RegistryKey^ root, String^ key);

private:
			int		splitter_width					();

private:
	Control^		m_property_grid_view;
	Point			m_previous_location;
}; // ref class property_grid

} // namespace controls
} // namespace editor
} // namespace vostok

#endif // #define PROPERTY_GRID_HPP_INCLUDED