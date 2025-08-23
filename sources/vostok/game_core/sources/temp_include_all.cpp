#include "pch.h"
#include "temp_include_all.h"

#include <vostok/ai/npc_statistics.h>

namespace survarium
{

void bpp_dump_stats(u32, float, float, pcstr) { }


IncludeAll::IncludeAll()
{
	hit_type_parameters* htp = new hit_type_parameters("hand", 1., 1., 1., 0);
	htp->apply_damage(10., 100);
	htp->set_parameters(10., 20., 30.);

	booster_damage_protector* bdp = new booster_damage_protector("hand", 0.5, 1.);
	bdp->reduce_damage("__whatever", "hand", 100);

	damage_model owner = damage_model(type_apply_directly);
	owner.apply_med_kit("head", 1000);

	body_part_parameters* bpp = new body_part_parameters(
		"name",
		10.f,
		10.f,
		10.f,
		true,
		owner,
		1);

	vostok::ai::npc_statistics stats = vostok::ai::npc_statistics();
	
	bpp->add_hit_type(NULL);
	bpp->add_threshold(NULL);
	bpp->hit_by_type("hit_type", 10, 10., 10., false, NULL);
	bpp->increase_health(10);
	bpp->decrease_health(20);
	bpp->regenerate(10, 20);
	bpp->dump_state(&bpp_dump_stats, 0);
	bpp->dump_state(stats, 10);
	bpp->reset();

	bpp->apply_affect_by_force(affects_type_blindness, affect_canceling, 20);
	bpp->can_affect_death();
	bpp->has_affect_protector(affects_type_blindness);
	bpp->get_health_in_percentage();
	bpp->cancel_affect_by_force(affects_type_blindness);
	bpp->add_damage_protector(NULL);
	bpp->remove_damage_protector(NULL);
	bpp->pop_hit_type();
	bpp->pop_threshold();

	bpp->is_affect_applied(affects_type_blindness);
	bpp->get_hit_parameters("hit_params");
	bpp->set_parameters(10.f, 20.f);

	bpp->check_affects(10);
	bpp->update_affects(20);
	bpp->apply_affects(NULL, 30);

	Callback1 cb1;
	Callback2 cb2;
}


Callback1::Callback1()
{
	m_complete = boost::bind(&Callback1::complete, this, _1);
	m_partial  = boost::bind(&Callback1::partial , this, _1, 10);
}



void Callback1::complete(const char *name)
{
	m_complete = boost::bind(&Callback1::complete, this, _1);
}

void Callback1::partial(const char *name, int value)
{
	m_partial  = boost::bind(&Callback1::partial , this, _1, 10);
}



Callback2::Callback2()
{
	m_complete = boost::bind(&Callback2::complete, this, _1, _2);
	m_partial  = boost::bind(&Callback2::partial , this, _1, _2, 10.f);
}

void Callback2::complete(const char *name, int value)
{
	m_complete = boost::bind(&Callback2::complete, this, _1, _2);
}

void Callback2::partial(const char *name, int value, float precision)
{
	m_partial  = boost::bind(&Callback2::partial , this, _1, _2, 10.f);
}


}