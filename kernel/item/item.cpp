#include "item.h"

void kernel::item::create(abstractes::name name_, std::string description_, type type_, int use_)
{
	_name = name_.get_name();
	_description = description_;
	_type = type_;
	_use = use_;
}

std::string kernel::item::get_description()
{
	std::stringstream desc;
	desc << _name << " ";
	switch(_type)
	{
		case type::nothing:
			break;
		case type::edible:
			desc << "(edible " << _use << ")";
			break;
		case type::heal:
			desc << "(heal " << _use << ")";
			break;
		case type::armor:
			desc << "(armor " << _use << ")";
			break;
		case type::weapon:
			desc << "(weapon " << _use << ")";
			break;
		default:
			desc << "([type] " << _use << ")";
			break;
	}

	desc << " - " << _description;

	return desc.str();
}

kernel::type kernel::item::type_c()
{
	return _type;
}

int kernel::item::usage()
{
	return _use;
}

void kernel::item::load(std::string dir_, size_t line_)
{
	hardware hardware;
	hardware.read_to_buffer_ln(dir_, line_);
	std::string raw = hardware.buffer();

 	parse(raw);
}

void kernel::item::parse(std::string raw_)
{
	checkParse(raw_, _name);
	checkParse(raw_, _use);
	checkParse(raw_, (char&)_type);
	checkParse(raw_, _description);
}
