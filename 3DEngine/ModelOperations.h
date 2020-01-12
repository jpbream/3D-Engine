#pragma once
#include "Model.h"

class ModelOperations {

	ModelOperations() = delete;

public:
	static void CalculateTangents(const Model& m);

};

