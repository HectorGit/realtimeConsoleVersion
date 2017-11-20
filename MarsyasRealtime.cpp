#pragma once

#include "stdafx.h"
#include "MarsyasRealtime.h"

MarsyasRealtime::MarsyasRealtime() {

	//myClassifier = new OpenNNClassifier();

	sropt = 44100.0;
	copt = 1;
	length = 5;
	bufferSize = 1024; //vs 512

	cout << "realtime feat extract into a single vector" << endl;

	//then the extractor net
	net = mng.create("Series", "net");
	net->updControl("mrs_real/israte", sropt);
	net->updControl("mrs_natural/inSamples", bufferSize); //origin 512

	net->addMarSystem(mng.create("AudioSource", "otherSrc"));

	net->updControl("AudioSource/otherSrc/mrs_natural/nChannels", copt);
	net->updControl("AudioSource/otherSrc/mrs_natural/inSamples", bufferSize); //origin 512
	net->updControl("AudioSource/otherSrc/mrs_natural/bufferSize", bufferSize); //origin 512
	net->updControl("AudioSource/otherSrc/mrs_real/israte", sropt);

	//print info out
	srate = net->getctrl("mrs_real/israte")->to<mrs_real>();
	nChannels = net->getctrl("AudioSource/otherSrc/mrs_natural/nChannels")->to<mrs_natural>();
	cout << "AudioSource srate =  " << srate << endl;
	cout << "AudioSource nChannels = " << nChannels << endl;
	inSamples = net->getctrl("mrs_natural/inSamples")->to<mrs_natural>();
	cout << "net inSamples = " << inSamples << endl;

	MarSystem* spectrumOperation = mng.create("Series", "spectrumOperation");
	spectrumOperation->addMarSystem(mng.create("Windowing", "ham"));
	spectrumOperation->addMarSystem(mng.create("Spectrum", "spec"));
	spectrumOperation->addMarSystem(mng.create("PowerSpectrum", "pspk"));

	net->addMarSystem(spectrumOperation);

	MarSystem* featureFanout = mng.create("Fanout", "featureFanout");
	featureFanout->addMarSystem(mng.create("Centroid", "centroid"));
	featureFanout->addMarSystem(mng.create("Rolloff", "rolloff"));
	featureFanout->addMarSystem(mng.create("MFCC", "mfcc"));
	featureFanout->addMarSystem(mng.create("Kurtosis", "kurtosis"));
	featureFanout->addMarSystem(mng.create("Skewness", "skewness"));

	net->addMarSystem(featureFanout);
	// FOR SOME REASON SETTING UP THIS - might be unnecessary

	//MarSystem* acc = mng.create("Accumulator", "acc");
	//acc->addMarSystem(net);

	total = mng.create("Series", "total");
	//total->addMarSystem(acc);
	//total->updControl("Accumulator/acc/mrs_natural/nTimes", 30); //was 30.
	//total->addMarSystem(mng.create("Mean", "mn"));
	total->updControl("mrs_real/israte", 44100.0);
	total->updControl("mrs_natural/inSamples", bufferSize); //origin 512
	total->addMarSystem(net);

	//out2.create(total->getctrl("mrs_natural/onObservations")->to<mrs_natural>(),
	//total->getctrl("mrs_natural/onSamples")->to<mrs_natural>());

}

MarsyasRealtime::~MarsyasRealtime() {

}

//might have to move this to main (in realTimeClassificaiton.cpp)
mrs_realvec MarsyasRealtime::startExtraction() {

	/*net->updControl("AudioSource/otherSrc/mrs_natural/nChannels", copt);
	net->updControl("AudioSource/otherSrc/mrs_natural/inSamples", bufferSize); //origin 512
	net->updControl("AudioSource/otherSrc/mrs_natural/bufferSize", bufferSize); //origin 512
	net->updControl("AudioSource/otherSrc/mrs_real/israte", sropt);
	net->updControl("AudioSource/otherSrc/mrs_bool/initAudio", true);
	mrs_natural iterations = (mrs_natural)((srate*length) / inSamples);
	cout << "Iterations = " << iterations << endl;
	for (mrs_natural t = 0; t < iterations; t++)
	{
	*/
	total->tick();
	const mrs_realvec src_data =
		total->getctrl("mrs_realvec/processedData")->to<mrs_realvec>();
	return src_data;

	/*}*/
}