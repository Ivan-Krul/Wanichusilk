#pragma once
#include "../../define.h"
#include "../../abstractes/name.h"
#include "../../abstractes/loader.h"
#include "../../abstractes/description.h"
#include <sstream>
namespace kernel
{
	enum class type : char
	{
		nothing = 'n',
		edible = 'e',
		heal = 'h',
		armor = 'a',
		weapon = 'w'
	};

	class item : public abstractes::name, public abstractes::loader, public abstractes::description
	{
	protected:
		type _type = type::nothing;
		int _use = 0;

	public:
		item() {}
		item(abstractes::name name_, std::string description_, type type_, int use_) { create(name_, description_, type_, use_); }
		void create(abstractes::name name_, std::string description_, type type_, int use_);

		std::string get_description();
		type type_c();
		int usage();
		void load(std::string dir_, size_t line_);
		void parse(std::string raw_);

		virtual ~item() {}
	};
#include "item.cpp"
};