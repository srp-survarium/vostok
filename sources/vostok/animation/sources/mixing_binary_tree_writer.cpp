////////////////////////////////////////////////////////////////////////////
//	Created		: 27.10.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#ifndef MASTER_GOLD
#include "mixing_binary_tree_writer.h"
#include <vostok/animation/mixing_binary_tree_animation_node.h>
#include <vostok/animation/mixing_binary_tree_weight_node.h>
#include <vostok/animation/mixing_binary_tree_addition_node.h>
#include <vostok/animation/mixing_binary_tree_subtraction_node.h>
#include <vostok/animation/mixing_binary_tree_multiplication_node.h>
#include <vostok/animation/instant_interpolator.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/animation/fermi_interpolator.h>
#include <vostok/animation/skeleton_animation.h>
#include <vostok/animation/cubic_spline_skeleton_animation.h>

using vostok::animation::mixing::binary_tree_writer;
using vostok::animation::mixing::binary_tree_animation_node;
using vostok::animation::mixing::binary_tree_weight_node;
using vostok::animation::mixing::binary_tree_addition_node;
using vostok::animation::mixing::binary_tree_multiplication_node;
using vostok::animation::mixing::binary_tree_subtraction_node;
using vostok::animation::mixing::binary_tree_binary_operation_node;
using vostok::animation::instant_interpolator;
using vostok::animation::linear_interpolator;
using vostok::animation::fermi_interpolator;
///////////////////////////////////////////////////////////////////////////////////
// template<class AnimationNodeListItemType> struct binary_tree_writer_predicate //
///////////////////////////////////////////////////////////////////////////////////
template<class AnimationNodeListItemType>
struct binary_tree_writer_predicate: private boost::noncopyable
{
	binary_tree_writer_predicate(vostok::configs::lua_config_value& cfg)
	:m_cfg(cfg)
	{}
	
	void operator () (AnimationNodeListItemType const* cb) const
	{
		ASSERT(cb);
		vostok::animation::cubic_spline_skeleton_animation_pinned animation(cb->animation().animation());
		m_cfg[intervals_counter]["animation"] = animation.c_ptr()->get_file_name().c_str();
		m_cfg[intervals_counter]["length"] = cb->length();
		m_cfg[intervals_counter]["offset"] = cb->start_time();
		++intervals_counter;
	}

	vostok::configs::lua_config_value& m_cfg;
	static u32 intervals_counter;
}; // struct binary_tree_writer_predicate

u32 binary_tree_writer_predicate<vostok::animation::mixing::animation_interval>::intervals_counter = 0;

///////////////////////////////////////////////////////////////////////////////////
// class binary_tree_writer ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
binary_tree_writer::binary_tree_writer(vostok::configs::lua_config_value& cfg, vostok::math::float2& start_pos) 
:m_config(cfg),
m_position(start_pos)
{
	m_last_id = 1; // starts from one because of root "TARGET" node;
}

void binary_tree_writer::visit(binary_tree_animation_node& node)
{
	vostok::configs::lua_config_value old_cfg = m_config;
	m_config["cycle_from_interval_id"] = node.start_cycle_animation_interval_id();
	m_config["mixing_type"] = (u32)node.playing_type();
	m_config["offset"] = 0.f;
	m_config["time_scale"] = node.time_scale();
	m_config["type"] = (u8)animation_node_type;
	m_config["id"] = m_last_id++;
	m_config["position"] = m_position;

	//TODO: insert synchronized animations saving!

	m_config = old_cfg["intervals"];
	//binary_tree_writer_predicate<animation_interval> pred(m_config);
	//pred.intervals_counter = 0;
	//node.intervals().for_each(pred);
	animation_interval const* i						= node.animation_intervals_begin();
	animation_interval const* const e				= node.animation_intervals_end();
	for (u32 counter = 0; i != e; ++i, ++counter ) {
		configs::lua_config_value interval			= m_config[counter];
		cubic_spline_skeleton_animation_pinned		pinned_animation( (*i).animation() );
		interval["animation"]						= pinned_animation.c_ptr()->get_file_name().c_str();
		interval["length"]							= (*i).length();
		interval["offset"]							= (*i).start_time();
	}

	m_config = old_cfg["interpolator"];
	node.time_scale_interpolator()->accept(*this);
	m_config = old_cfg;
}

void binary_tree_writer::visit(binary_tree_weight_node& node)
{
	m_config["type"] = (u8)weight_node_type;
	m_config["weight"] = node.weight();
	m_config["id"] = m_last_id++;

	vostok::configs::lua_config_value old_cfg = m_config;
	m_config = old_cfg["interpolator"];
	node.interpolator().accept(*this);
	m_config = old_cfg;
}

void binary_tree_writer::visit(binary_tree_addition_node& node)
{
	m_config["type"] = (u8)addition_node_type;
	m_config["id"] = m_last_id++;
	m_config["position"] = m_position;
	m_position.x += 250.0f;
	vostok::configs::lua_config_value old_cfg = m_config;
	m_config = old_cfg["childs"][0];
	node.left().accept(*this);
	m_position.y += 100.0f;
	m_config = old_cfg["childs"][1];
	node.right().accept(*this);
	m_position.x -= 250.0f;
	m_config = old_cfg;
}

void binary_tree_writer::visit(binary_tree_subtraction_node& node)
{
	m_config["type"] = (u8)substraction_node_type;
	m_config["id"] = m_last_id++;
	m_config["position"] = m_position;
	m_position.x += 250.0f;
	vostok::configs::lua_config_value old_cfg = m_config;
	m_config = old_cfg["childs"][0];
	node.left().accept(*this);
	m_position.y += 100.0f;
	m_config = old_cfg["childs"][1];
	node.right().accept(*this);
	m_config = old_cfg;
}

void binary_tree_writer::visit(binary_tree_multiplication_node& node)
{
	m_config["type"] = (u8)multiplication_node_type;
	m_config["id"] = m_last_id++;
	vostok::configs::lua_config_value old_cfg = m_config;
	m_config = old_cfg["childs"][0];
	node.left().accept(*this);
	m_config = old_cfg["childs"][1];
	node.right().accept(*this);
	m_config = old_cfg;
}

void binary_tree_writer::visit(instant_interpolator const& interpolator)
{
	m_config["type"] = (u8)instant;
	m_config["time"] = interpolator.transition_time();
}

void binary_tree_writer::visit(linear_interpolator const& interpolator)
{
	m_config["type"] = (u8)linear;
	m_config["time"] = interpolator.transition_time();
}

void binary_tree_writer::visit(fermi_interpolator const& interpolator)
{
	m_config["type"] = (u8)fermi;
	m_config["time"] = interpolator.transition_time();
	m_config["epsilon"] = interpolator.epsilon();
}

#endif // #ifndef MASTER_GOLD