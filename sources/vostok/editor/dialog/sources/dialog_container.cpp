////////////////////////////////////////////////////////////////////////////
//	Created		: 01.07.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "dialog_container.h"
#include "dialog.h"
#include "dialogs_manager.h"
#include "dialog_editor_resource.h"

#pragma unmanaged
using vostok::dialog_editor::dialog_container;
using vostok::dialog_editor::dialog;

dialog_container::dialog_container()
{
	m_resource = NEW(dialog_resources_ptr)();//NULL;
	m_text = NULL;
}

dialog_container::~dialog_container()
{
	(*m_resource) = NULL;
	DELETE(m_resource);
	FREE(m_text);
}

void dialog_container::save(vostok::configs::lua_config_value cfg)
{
	if(!(*m_resource))
		return;

	(*m_resource)->get_dialog()->save(cfg);
}

void dialog_container::remove_node(vostok::dialog_editor::dialog_node_base* n)
{
	if(!(*m_resource))
		return;

	(*m_resource)->get_dialog()->remove_node(n);
}

void dialog_container::remove_node(u32 id)
{
	if(!(*m_resource))
		return;

	(*m_resource)->get_dialog()->remove_node(get_node(id));
}

void dialog_container::add_node(vostok::dialog_editor::dialog_node_base* n)
{
	if(!(*m_resource))
		return;

	if((*m_resource)->get_dialog()->get_node(n->id()))
		return;

	(*m_resource)->get_dialog()->add_node(n);
}

vostok::dialog_editor::dialog_node_base* dialog_container::get_node(u32 id)
{
	if(!(*m_resource))
		return NULL;

	return (*m_resource)->get_dialog()->get_node(id);
}

void dialog_container::set_text(pcstr new_text)
{
	if(!(*m_resource))
		m_text = strings::duplicate(g_allocator, new_text);
	else
		(*m_resource)->get_dialog()->set_name(new_text);
}

pcstr dialog_container::text()
{
	if(!(*m_resource))
		return m_text;
	else
		return (*m_resource)->get_dialog()->name();
}

void dialog_container::request_dialog()
{
	dialog_manager::get_dialogs_manager()->request_editor_dialog(m_text, boost::bind(&dialog_container::on_dialog_loaded, this, _1));
}

void dialog_container::on_dialog_loaded(vostok::dialog_editor::dialog_resources_ptr p)
{
	(*m_resource) = p.c_ptr();
	(*m_resource)->get_dialog()->set_name(m_text);
}