// ProbabilitySequentialWrapper.cpp

/** 
 * @file
 * Implementation of ProbabilitySequentialWrapper.cpp
 */

#include <algorithm> // for min, max

#include "cla/utils/Checker.hpp"
#include "cla/utils/VectorHelpers.hpp"
#include "cla/environment/wrappers\ProbabilitySequentialWrapper.hpp"

namespace cla {

/************************************************
 * ProbabilitySequentialWrapper public functions.
 ***********************************************/

ProbabilitySequentialWrapper::ProbabilitySequentialWrapper(
	const std::vector<EnvSection>& sections,
	const int seed
) {
	initialize(sections, seed);
}

void ProbabilitySequentialWrapper::initialize(
	const std::vector<EnvSection>& sections,
	const int seed
) {
	CLA_ASSERT(!sections.empty());
	CLA_ASSERT(sections.front().first);

	const Dim& dim = sections.front().first->getDimension();
	for(std::size_t i = 0u, size = sections.size(); i < size; ++i) {
		CLA_ASSERT(sections.at(i).first);
		CLA_ASSERT(sections.at(i).first->getDimension() == dim);
	}

	_setDimension(dim);

	Values mins = sections.front().first->getMins();
	Values maxs = sections.front().first->getMaxs();

	for(std::size_t idx = 1u, size = sections.size(); idx < size; ++idx) {
		mins = select_min_elems(mins, sections.at(idx).first->getMins());
		maxs = select_max_elems(maxs, sections.at(idx).first->getMaxs());
	}

	_setMins(mins);
	_setMaxs(maxs);

	sections_.resize(sections.size());
	envs_.resize(sections.size());

	for(std::size_t i = 0u, size = sections.size(); i < size; ++i) {
		envs_.at(i) = sections.at(i).first;
		sections_.at(i) = sections.at(i).second;
	}

	seed_ = seed;
	mt_ = std::mt19937(seed_);

	reset();
}

void ProbabilitySequentialWrapper::reset() {
	CoreEnv::reset();
	selected_ = select_();
	subStep_ = static_cast<Step>(0);

	for(auto&& env : envs_)
		env->reset();
}

const Values ProbabilitySequentialWrapper::getValues() const {
	return envs_.at(selected_)->getValues();
}

const json ProbabilitySequentialWrapper::getJsonConfig() const {
	const std::vector<Step> sectionStep = getSectionSteps();
	CLA_ASSERT(envs_.size() == sectionStep.size())
	
	json j;
	j[name]["seed"] = seed_;

	for(std::size_t i = 0u, size = envs_.size(); i < size; ++i) {
		json subj = envs_.at(i)->getJsonConfig();
		subj["step"] = sectionStep.at(i);

		j[name]["envs"].emplace_back(subj);
	}

	return j;
}

void ProbabilitySequentialWrapper::increment() {
	envs_.at(selected_)->increment();
	CoreEnv::increment();
	++subStep_;

	// If the sub step in this class exceed the steps in the current section, 
	// selected a new env.
	if(subStep_ >= sections_.at(selected_)) {
		subStep_ = 0u;
		selected_ = select_();
	}
}

const std::vector<PEnv>& ProbabilitySequentialWrapper::getEnvs() const {
	return envs_;
}

const std::vector<Step>& ProbabilitySequentialWrapper::getSectionSteps() const {
	return sections_;
}


/************************************************
 * ProbabilitySequentialWrapper private functions.
 ***********************************************/

const ProbabilitySequentialWrapper::Idx ProbabilitySequentialWrapper::select_() {
	std::uniform_int_distribution<> dist(0, static_cast<int>(envs_.size() - 1u));
	Idx idx = static_cast<Idx>(dist(mt_));

	CLA_CHECK(
		idx < envs_.size(), 
		"Specifies a vector element where idx does not exist"
	);

	return idx;
}

} // namespace cla