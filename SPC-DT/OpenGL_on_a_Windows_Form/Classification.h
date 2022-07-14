#include "stdafx.h"
#pragma once

struct Classification {
	int caseNum;
	int actualClass;
	int predictedClass;
	bool isMisclassification;
	Classification(int caseNum, int actualClass, int predictedClass) {
		this->caseNum = caseNum;
		this->actualClass = actualClass;
		this->predictedClass = predictedClass;
		this->isMisclassification = actualClass == predictedClass;
	}
};