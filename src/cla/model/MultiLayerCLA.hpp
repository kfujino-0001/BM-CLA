// MultiLayerCLA.hpp

/**
 * @file
 * Definitions for the MultiLayerCLA class in C++
 */

#ifndef MULTI_LAYER_CLA_HPP
#define MULTI_LAYER_CLA_HPP

#include "cla/model/core/CoreCLA.hpp"
#include "cla/model/core/CoreIO.hpp"
#include "cla/model/core/CoreLayer.hpp"

namespace cla {

/**
 * MultiLayerCLA implementation in C++.
 *
 * @b Description
 * The MultiLayer is the CLA which has some layers. The layer has some
 * types of modules: accepter, adapter, spatial pooler, temporal memory,
 * sender, and receiver. One layer is the same as the MultiRegionCLA. This
 * class has a role in implementing some CLA models by changing the
 * structure and type of module.
 * 
 * WARNING
 * The order in which layers are registered is as follows.
 * 
 * +----------+----------+----------+
 * |   Type   |  Number  | Relation |
 * +----------+----------+----------+
 * |   Layer  | Layer[0] |   TOP    |
 * |   Layer  | Layer[1] |    ^     |
 * |   Layer  | Layer[2] |    |     |
 * |   Layer  | Layer[3] |    v     |
 * |    IO    |   IO     |  Bottom  |
 * +----------+----------+----------+
 * 
 */
class MultiLayerCLA : public CoreCLA {

private:

	std::vector<PLayer> layers_;
	PIO io_;

public:

	/**
	 * MultiLayerCLA constructor.
	 */
	MultiLayerCLA() = default;

	/**
	 * MultiLayerCLA constructor with the parameters.
	 *
	 * @param layers The pointers of the layer modules.
	 * @param io The pointer of the io module.
	 */
	MultiLayerCLA(std::vector<PLayer> layers, PIO io);

	/**
	 * MultiLayerCLA destructor.
	 */
	~MultiLayerCLA() = default;

	/**
	 * Reset the cla model.
	 */
	void reset() override;

	/**
	 * Summarize the cla model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Show the detail of the cla model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void detail(std::ostream& os = std::cout) const override;

	/**
	 * Feedforward input values to the prediction values. This function is
	 * uses in the fit and test.
	 *
	 * @param inputs The input values which is vector of float.
	 * @param learn The boolean value whether learning mode.
	 * @return const Values: The prediction values.
	 */
	const Values feedforward(const Values& inputs, const bool learn) override;

	/**
	 * Feedback the next values to the cla model.
	 * This function performs additional learning from input values from
	 * the environment at a given next time step.
	 *
	 * @param nexts The input values at a given next time step.
	 * @param learn The boolean value whether learning model.
	 */
	void feedback(const Values& nexts, const bool learn) override;

	/**
	 * Get the layer proxies of the cla model.
	 * 
	 * @return const std::vector<PLayerProxy>: The layer proxies of the cla model.
	 */
	const std::vector<PLayerProxy> getLayers() const override;

	/**
	 * Get the pointer of the io module.
	 * 
	 * @return const PIO& The pointer io module.
	 */
	const PIO& getIO() const override;

};

} // namespace cla

#endif // MULTI_LAYER_CLA_HPP