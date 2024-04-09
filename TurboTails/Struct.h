#pragma once

#include"Main.h"

#include<list>

typedef struct FielData {

public:

	char file[1024];
	char tag[256];

}FileData;

typedef struct AnimFileData : public FileData
{

	

}AnimFileData;