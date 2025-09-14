////////////////////////////////////////////////////////////////////////////
//	Created 	: 19.12.2007
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PROPERTY_STRING_SHARED_STR_HPP_INCLUDED
#define PROPERTY_STRING_SHARED_STR_HPP_INCLUDED

public ref class property_string_shared_str : public vostok::editor::wpf_controls::i_property_value {
public:
							property_string_shared_str	(vostok::shared_string& value);
	virtual					~property_string_shared_str	();
							!property_string_shared_str	();
	virtual System::Object	^get_value					();
	virtual void			set_value					(System::Object ^object);
	virtual property System::Type^			value_type
	{
		System::Type^ get() {return System::String::typeid;}
	}
private:
	vostok::shared_string*	m_value;
}; // ref class property_string_shared_str

#endif // ifndef PROPERTY_STRING_SHARED_STR_HPP_INCLUDED