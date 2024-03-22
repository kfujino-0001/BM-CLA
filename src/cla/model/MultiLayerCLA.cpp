// MultiLayerCLA.cpp

/** 
 * @file
 * Implementation of MultiLayerCLA.cpp
 */

#include "cla/utils/Checker.hpp"
#include "cla/utils/SdrHelpers.hpp"
#include "cla/model/MultiLayerCLA.hpp"

namespace cla {

/************************************************
 * MultiLayerCLA public functions.
 ***********************************************/

MultiLayerCLA::MultiLayerCLA(
	std::vector<PLayer> layers,
	PIO io
):
	layers_(std::move(layers)),
	io_(std::move(io))
{
	CLA_ASSERT(!layers_.empty());
	for(const auto& layer : layers_){
		CLA_ASSERT(layer);
	}

	CLA_ASSERT(io_);
}

void MultiLayerCLA::reset() {
	for(auto&& layer : layers_)
		layer->reset();
}

void MultiLayerCLA::summary(std::ostream& os) const {
	os << " ## MLCLA ################################################"
	   << std::endl
	   << std::endl;

	for(auto&& layer : layers_)
		layer->summary(os);

	os << " =========================================================" << std::endl;
	os << "  ";
	io_->summary(os);
	os << std::endl;
	os << " =========================================================" << std::endl;
	os << " #########################################################" << std::endl;
	os << std::endl << std::endl;
}

void MultiLayerCLA::detail(std::ostream& os) const {
	os << " ## MLCLA ################################################"
	   << std::endl
	   << std::endl;

	for(auto&& layer : layers_)
		layer->detail(os);

	io_->detail(os);

	os << " #########################################################" << std::endl;
}

const Values MultiLayerCLA::feedforward(
	const Values& inputs,
	const bool learn
) {
	// The preparation process.
	for(auto&& layer : layers_) {
		layer->restate();
	}

	// Initialize local variables.
	htm::SDR inputSDR, activeSDR;
	PLayerProxy proxy;
	int nbLayers = static_cast<int>(layers_.size()), idx = nbLayers - 1;
	bool isContinueRun;

	// forward process.
	io_->encode(inputs, inputSDR);

	for(; idx >= 0; --idx) {
		isContinueRun = layers_.at(idx)->forward(inputSDR, learn, activeSDR);
	
		if(isContinueRun && idx > 0) copy(activeSDR, inputSDR);
		else break;
	}


	// backward process.
	for(; idx < nbLayers; ++idx) {
		if(idx == 0) {
			// The case when the top layer run.
			proxy = layers_.at(idx)->backward(learn);
		} else {
			// The case when the middle layer run.
			proxy = layers_.at(idx)->backward(proxy, learn);
		}
	}

	return io_->decode(proxy);
}

void MultiLayerCLA::feedback(
	const Values& nexts,
	const bool learn
) {
	if(learn) {
		io_->learn(layers_.back()->getLayerProxy(), nexts);
	}
}

const std::vector<PLayerProxy> MultiLayerCLA::getLayers() const {
	std::vector<PLayerProxy> proxies(layers_.size());

	for(std::size_t i = 0u, size = layers_.size(); i < size; ++i)
		proxies.at(i) = layers_.at(i)->getLayerProxy();

	return proxies;
}

const PIO& MultiLayerCLA::getIO() const {
	return io_;
}

} // namespace cla