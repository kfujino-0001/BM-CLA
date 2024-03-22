// Evaluations.cpp

/** 
 * @file
 * Implementation of Evaluations
 */

#include <cmath>

#include "cla/utils/Evaluations.hpp"

namespace evaluations {

/**
 * Evaluations methods
 */
Evaluations::Evaluations() {
	initialize();
}

void Evaluations::initialize() {
	sumSquare_ = 0.0;
	sumAbsolute_ = 0.0;
	nbElem_ = 0u;
}

double Evaluations::getRMSE() const {
	return std::sqrt(sumSquare_ / static_cast<double>(nbElem_));
}

double Evaluations::getMAE() const {
	return sumAbsolute_ / static_cast<double>(nbElem_);
}


} // namespace evaluations