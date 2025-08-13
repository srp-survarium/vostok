#include "pch.h"
#include "temp_include_all.h"

namespace stalker2
{

IncludeAll::IncludeAll()
{
	hit_type_parameters* htp = new hit_type_parameters("hand", 1., 1., 1., 0);
	htp->apply_damage(10., 100);
	htp->set_parameters(10., 20., 30.);

	booster_damage_protector* bdp = new booster_damage_protector("hand", 0.5, 1.);
	bdp->reduce_damage("__whatever", "hand", 100);

	body_part_parameters* bpp = new body_part_parameters();
	bpp->hit_by_type("hit_type", 10, 10., 10., false, NULL);
}

Callback1::Callback1()
{
	// m_fn = boost::bind(&Callback1::partial, this);
}

void Callback1::partial(const char *name)
{

}

}