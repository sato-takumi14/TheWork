#pragma once

#include"Transform.h"
#include"DrawInterface.h"

typedef struct UI {

public:

	string classname;
	string name;
	Transform transform;
	DrawInfo info;

public:

	UI() {}
	UI(crstring classname, crstring name, const Transform& transform, const DrawInfo& info)
		: classname(classname), name(name), transform(transform), info(info) {}

	template <class DATA>
	void serialize(DATA& data) {
		data(CEREAL_NVP(classname), CEREAL_NVP(name), CEREAL_NVP(transform), CEREAL_NVP(info));
	}

}UI;