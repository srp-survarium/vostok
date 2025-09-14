
inline void rename_cfg(vostok::configs::lua_config_value* v, System::String^ new_field_id )
{
	vostok::editor::unmanaged_string		s(new_field_id);
	v->rename							(s.c_str());
}

inline void set_c_string(vostok::configs::lua_config_value v, System::String^ str )
{
	vostok::editor::unmanaged_string		s(str);
	v									= (s.c_str());
}
