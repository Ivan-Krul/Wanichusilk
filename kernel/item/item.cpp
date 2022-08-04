#include "item.h"

void item::create(name name_, std::string description_, type type_, int use_)
{
	_name = name_.get_name();
	_description = description_;
	_type = type_;
	_use = use_;
}

std::string item::description()
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

type item::type_c()
{
	return _type;
}

int item::usage()
{
	return _use;
}
