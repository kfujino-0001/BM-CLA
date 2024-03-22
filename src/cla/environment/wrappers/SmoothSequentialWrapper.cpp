// SmoothSequentialWrapper.cpp

/** 
 * @file
 * Implementation of SmoothSequentialWrapper.cpp
 */

#include "cla/utils/MathHelpers.hpp"
#include "cla/utils/Checker.hpp"
#include "cla/utils/VectorHelpers.hpp"
#include "cla/environment/wrappers/SmoothSequentialWrapper.hpp"

namespace cla {

/************************************************
 * SmoothSequentialWrapper private functions.
 ***********************************************/

const Value SmoothSequentialWrapper::sig_(const Value x) const {
	return 1.0 / (1.0 + std::exp(-x));
}

const Value SmoothSequentialWrapper::theta_(
	const Step length,
	const Step step
) const {
	const Step dos = 2u * overlappedStep_;

	if((0u <= step) && (step < dos)) {
		return sig_(sigc_ * map(step, static_cast<Step>(0), dos, -1.0, 1.0));
	} else if((dos <= step) && (step < length)) {
		return static_cast<Value>(1.0);
	} else if((length <= step) && (step < length + dos)) {
		return sig_(sigc_ * map(step, length, length + dos, 1.0, -1.0));
	} else {
		return static_cast<Value>(0.0);
	}	
}

const Value SmoothSequentialWrapper::delta_(
	const Step length,
	const Step step,
	const Step section,
	const Step fullLength
) const {
	return theta_(
		length, 
		(step + overlappedStep_ - section + fullLength) % fullLength
	);
}



/************************************************
 * SmoothSequentialWrapper public functions.
 ***********************************************/

SmoothSequentialWrapper::SmoothSequentialWrapper(
	const std::vector<EnvSection>& sections,
	const Step overlappedStep
) {
	initialize(sections, overlappedStep);
}

void SmoothSequentialWrapper::initialize(
	const std::vector<EnvSection>& sections,
	const Step overlappedStep
) {
	CLA_ASSERT(!sections.empty());
	CLA_ASSERT(sections.front().first);

	const Dim& dim = sections.front().first->getDimension();
	for(std::size_t i = 0u, size = sections.size(); i < size; ++i) {
		CLA_ASSERT(sections.at(i).first);
		CLA_ASSERT(sections.at(i).first->getDimension() == dim);
		CLA_ASSERT(sections.at(i).second >= 2 * overlappedStep);
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

	overlappedStep_ = overlappedStep;

	sections_.resize(sections.size() + 1u);
	lengths_.resize(sections.size());
	envs_.resize(sections.size());

	sections_.at(0) = static_cast<Step>(0);
	for(std::size_t i = 0u, size = sections.size(); i < size; ++i) {
		envs_.at(i) = sections.at(i).first;
		lengths_.at(i) = sections.at(i).second;
		sections_.at(i + 1) = sections_.at(i) + sections.at(i).second;
	}

	wavelength_ = sections_.back();
	reset();
}

void SmoothSequentialWrapper::reset() {
	CoreEnv::reset();

	for(auto&& env : envs_)
		env->reset();
}

const Values SmoothSequentialWrapper::getValues() const {
	const Dim dim = getDimension();
	Values values(dim, static_cast<Value>(0));

	for(std::size_t i = 0u, size = envs_.size(); i < size; ++i) {
		const auto& outputs = envs_.at(i)->getValues();
		const Value delta 
			= delta_(lengths_.at(i), getStep(), sections_.at(i), wavelength_);
	
		CLA_ASSERT(static_cast<Dim>(outputs.size()) == dim);

		for(Dim j = 0u; j < dim; ++j) {
			values.at(j) += delta * outputs.at(j);
		}
	}

	return values;
}

const json SmoothSequentialWrapper::getJsonConfig() const {
	const std::vector<Step> sectionStep = getSectionSteps();
	CLA_ASSERT(envs_.size() == sectionStep.size())

	json j;
	j[name]["overlap"] = overlappedStep_;

	for(std::size_t i = 0u, size = envs_.size(); i < size; ++i) {
		json subj = envs_.at(i)->getJsonConfig();
		subj["step"] = sectionStep.at(i);

		j[name]["envs"].emplace_back(subj);
	}

	return j;
}

void SmoothSequentialWrapper::increment() {
	CoreEnv::increment();

	for(auto&& env : envs_)
		env->increment();
}

const std::vector<PEnv>& SmoothSequentialWrapper::getEnvs() const {
	return envs_;
}

const std::vector<Step> SmoothSequentialWrapper::getSectionSteps() const {
	CLA_ASSERT(sections_.size() > 1u);
	std::vector<Step> sections(sections_.size() - 1u);

	for(std::size_t i = 0u, size = sections.size(); i < size; ++i)
		sections.at(i) = sections_.at(i + 1u) - sections_.at(i);
	
	return sections;
}

} // namespace cla