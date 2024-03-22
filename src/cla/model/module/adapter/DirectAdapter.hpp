// DirectAdapter.hpp

/**
 * @file
 * Definitions for the DirectAdapter class in C++
 */

#ifndef DIRECT_ADAPTER_HPP
#define DIRECT_ADAPTER_HPP

#include "cla/model/core/CoreAdapter.hpp"

namespace cla {

/**
 * DirectAdapter implementation in C++.
 *
 * @b Description
 * The DirectAdapter is extended class of CoreAdapter.
 * The DirectAdapter is a class that directory links an input sdr to an
 * active sdr.
 */
class DirectAdapter : public CoreAdapter {

public:

	/**
	 * DirectAdapter constructor.
	 */
	DirectAdapter() = default;

	/**
	 * DirectAdapter destructor.
	 */
	~DirectAdapter() = default;

	/**
	 * reset the internal state of the adapter. The function is assumed to
	 * be executed at every time step.
	 */
	void restate() override {}

	/**
	 * Reset internal state of the adapter.
	 */
	void reset() override {}

	/**
	 * Summarize the adapter model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Show the detail of the adapter model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void detail(std::ostream& os = std::cout) const override;

	/**
	 * Adapt the input sdr to an active bits.
	 *
	 * @param inputSDR The input sdr from the lower layer.
	 * @param activeBits The active Bits. (This param has a return value.)
	 */
	void adapt(const htm::SDR& inputSDR, htm::SDR& activeBits) const override;
};

} // namespace cla

#endif // DIRECT_ADAPTER_HPP