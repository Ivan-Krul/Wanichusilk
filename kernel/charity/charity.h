#pragma once
#include "../../abstractes/name.h"
#include "../../abstractes/loader.h"
#include "../../abstractes/description.h"
namespace kernel
{
	class charity : public abstractes::name, public abstractes::loader, public abstractes::description {
	protected:
		float _prod = 1.0f;
		uint16_t _def = 0;
		int16_t _atk = 0;
		int16_t _edibleHeal = 0;

		void _digestion();
	public:
		charity() {}
		charity(std::string name, uint16_t def, int16_t atk, std::string description_);
		void create(std::string name, uint16_t def, int16_t atk, std::string description_);

		float damage(charity& charit);

		uint16_t defense_point();
		int16_t attack_point();
		int16_t edible_heal();

		void heal(float prod_);
		void hurt(float prod_);
		void eat(int16_t benefit_);

		void load(std::string dir_, size_t line_);
		void parse(std::string raw_);

		void tick();

		std::string get_description();

		~charity() {};
	};
	#include "charity.cpp"
}