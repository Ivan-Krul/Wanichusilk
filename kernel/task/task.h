#pragma once
#include "../../abstractes/name.h"
#include "../../abstractes/loader.h"
#include "../../abstractes/description.h"
#include "../item/item.h"
#include "../charity/charity.h"
namespace kernel
{
	enum class typetask
	{
		bring = 0b00000001,
		destroy = 0b00000100,
		interact = 0b00100000,
		have = 0b01000000
	};
	enum class typeentitytask
	{
		item = 0b00000001,
		charity = 0b00000100,
		tile = 0b00100000,
		key = 0b01000000
	};

	class task : public abstractes::name, public abstractes::loader, public abstractes::description
	{
	protected:
		unsigned char types;
		unsigned char entities;
	public:

	};
};