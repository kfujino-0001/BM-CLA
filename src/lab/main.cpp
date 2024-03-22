// main.cpp

#define _USE_MATH_DEFINES

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <nlohmann/json.hpp>

#include "cla/model/core/CoreCLA.hpp"
#include "cla/model/generator/CallbackGenerator.hpp"
#include "cla/config/ModelConfig.hpp"

#include "cla/environment/Envs.hpp"
#include "cla/environment/Wrappers.hpp"
#include "cla/utils/Checker.hpp"


using json = nlohmann::json;
namespace fs = std::filesystem;
template<typename CallbackType>
using Callback = cla::CallbackGenerator<CallbackType>;



int main(int argc, char** argv) {
	const std::string baseDir = "..\\..\\..\\";

	const std::string configFile = baseDir + "config\\cla_params.json";					// cla
	// const std::string configFile = baseDir + "config\\cla_asa_params.json";			// cla + asa
	// const std::string configFile = baseDir + "config\\bm_cla_params.json";			// bm-cla
	// const std::string configFile = baseDir + "config\\bm_cla_asa_params.json";		// bm-cla + asa
	// const std::string configFile = baseDir + "config\\decay_bm_cla_params.json";		// decay-bm-cla
	// const std::string configFile = baseDir + "config\\decay_bm_cla_asa_params.json";	// decay-bm-cla + asa

	const std::string outputDir =  baseDir + "log\\";
	const int nbTrain = 100000;
	const int nbCycle = 100;
	const int seed = 1;

	// create time-series environment.
	cla::PEnv env = cla::Env<cla::SinEnv>::make(nbCycle);

	// create callback
	cla::PCallback callback = Callback<cla::CompositeCallback>::generate({
		Callback<cla::EvalCallback>::generate(env->getDimension(), nbCycle, 1),
		Callback<cla::SaveModelLogCallback>::generate(outputDir + "train\\", "log.csv", env->getDimension(), nbCycle),
		Callback<cla::SaveLayerLogCallback>::generate(outputDir + "train\\", "layerLog.csv", nbCycle),
	});

	// load model config
	std::ifstream json_ifs(configFile);
	cla::JsonConfig config;

	CLA_CHECK(!json_ifs.fail(), "Cannot find param json file.")

	json_ifs >> config;
	json_ifs.close();

	// copy env info to the model config. 
	config.getModel().getIO().setMins(env->getMins());
	config.getModel().getIO().setMaxs(env->getMaxs());
	config.getModel().setSeed(seed);


	cla::PCLA model = config.buildModel();

	// output model info and environment info to stdout
	model->summary();
	model->detail();
	env->summary();
	std::cout << std::endl << std::endl;


	// output model info and info to stdout
	fs::create_directories(outputDir);
	std::ofstream ofs(outputDir + "\\run.txt");
	CLA_CHECK(!ofs.fail(), "Cannot open the config file.");

	// model->summary(ofs);
	model->detail(ofs);
	env->summary(ofs);
	 
	ofs << std::endl << std::endl
		<< "Train = " << nbTrain << std::endl
		<< "save file = " << outputDir << std::endl;

	ofs.close();


	// learn the environment
	model->fit(nbTrain, 0u, env, callback);

	return 0;
}