#pragma once

/*******************************************************************************************
*
*																制作者：佐藤匠
*																制作日：12/11
*
********************************************************************************************
* ランダムの値を生成する
********************************************************************************************/

#include<random>

#include"Main.h"

class Random
{
private:



public:

	static cint Int(crint max) {

		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<> dist(0, max);
		return dist(mt);

	}

	static cint Int(crint min, crint max) {

		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<> dist(min, max);
		return dist(mt);

	}

	static cfloat Float(crfloat max) {

		random_device rd;
		default_random_engine eng(rd());
		uniform_real_distribution<float> distr(0.0f, max);
		return distr(eng);

	}

	static cfloat Float(crfloat min, crfloat max) {

		random_device rd;
		default_random_engine eng(rd());
		uniform_real_distribution<float> distr(min, max);
		return distr(eng);

	}

	static cver3 Vector3(crfloat max) {

		random_device rd;
		default_random_engine eng(rd());
		uniform_real_distribution<float> distr(0.0f, max);
		
		return cver3(distr(eng), distr(eng), distr(eng));

	}

	static cver3 Vector3(crfloat min, crfloat max) {

		random_device rd;
		default_random_engine eng(rd());
		uniform_real_distribution<float> distr(min, max);

		return cver3(distr(eng), distr(eng), distr(eng));

	}

	static ccolor Color(crfloat max) {

		random_device rd;
		default_random_engine eng(rd());
		uniform_real_distribution<float> distr(0.0f, max);

		return ccolor(distr(eng), distr(eng), distr(eng), 1.0f);

	}

};