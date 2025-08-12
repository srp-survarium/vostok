////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BODY_PART_PARAMETERS_H_INCLUDED
#define BODY_PART_PARAMETERS_H_INCLUDED

namespace stalker2 {

typedef void damage_protector;

class body_part_parameters : private boost::noncopyable {
public:
	body_part_parameters();

	void hit_by_type(
		const char *hit_type,
		unsigned int time_in_ms,
		float amount,
		float armor_piercing,
		bool formal,
		damage_protector *prot
		);

private:

}; // class body_part_parameters 

} // namespace stalker2

#endif // #ifndef BODY_PART_PARAMETERS_H_INCLUDED