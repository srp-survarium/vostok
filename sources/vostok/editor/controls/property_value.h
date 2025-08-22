////////////////////////////////////////////////////////////////////////////
//	Created 	: 07.12.2007
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PROPERTY_VALUE_H_INCLUDED
#define PROPERTY_VALUE_H_INCLUDED

namespace vostok {
namespace editor {
namespace controls {

public interface class erproperty_value {
public:
	virtual System::Object	^get_value		() = 0;
	virtual void			set_value		(System::Object ^object) = 0;
}; // interface class property_value

} // namespace controls
} // namespace editor
} // namespace vostok

#endif // ifndef PROPERTY_VALUE_H_INCLUDED