// SequentialWrapper.cpp

/** 
 * @file
 * Implementation of SequentialWrapper.cpp
 */

#include "cla/utils/Checker.hpp"
#include "cla/utils/VectorHelpers.hpp"
#include "cla/environment/wrappers/SequentialWrapper.hpp"

namespace cla {

/************************************************
 * SequentialWrapper public functions.
 ***********************************************/

SequentialWrapper::SequentialWrapper(const std::vector<EnvSection>& sections) {
	initialize(sections);
}

void SequentialWrapper::initialize(const std::vector<EnvSection>& sections) {
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

	sections_.resize(sections.size() + 1u);
	envs_.resize(sections.size());

	sections_.at(0) = static_cast<Step>(0);
	for(std::size_t i = 0u, size = sections.size(); i < size; ++i) {
		envs_.at(i) = sections.at(i).first;
		sections_.at(i + 1) = sections_.at(i) + sections.at(i).second;
	}

	wavelength_ = sections_.back();
	reset();
}

void SequentialWrapper::reset() {
	CoreEnv::reset();
	selected_ = 0u;

	for(auto&& env : envs_)
		env->reset();
}

const Values SequentialWrapper::getValues() const {
	return envs_.at(selected_)->getValues();
}

const json SequentialWrapper::getJsonConfig() const {
	const std::vector<Step> sectionStep = getSectionSteps();
	CLA_ASSERT(envs_.size() == sectionStep.size())

	json j;
	
	for(std::size_t i = 0u, size = envs_.size(); i < size; ++i) {
		json subj = envs_.at(i)->getJsonConfig();
		subj["step"] = sectionStep.at(i);

		j[name]["envs"].emplace_back(subj);
	}

	return j;
}

void SequentialWrapper::increment() {
	envs_.at(selected_)->increment();
	CoreEnv::increment();

	const Step step = getStep() % wavelength_;

	// If the steps in this class exceed the steps in the current section, 
	// move to the next section.
	if(	
		(step == 0u) ||
		(step >= sections_.at(selected_ + 1u))
	) {
		++selected_;

		if(selected_ == envs_.size()) selected_ = 0u;
	} 
}

const std::vector<PEnv>& SequentialWrapper::getEnvs() const {
	return envs_;
}

const std::vector<Step> SequentialWrapper::getSectionSteps() const {
	CLA_ASSERT(sections_.size() > 1u);
	std::vector<Step> sections(sections_.size() - 1u);

	for(std::size_t i = 0u, size = sections.size(); i < size; ++i)
		sections.at(i) = sections_.at(i + 1u) - sections_.at(i);
	
	return sections;
}

} // namespace cla