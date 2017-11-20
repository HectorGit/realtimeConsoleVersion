// realTimeClassification.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "opennn/vector.h" //wtf?
#include "OpenNNClassifier.h" 
#include "MarsyasRealTime.h" //commented out for now.
#include "MarsyasPlayerNet.h"

//used ConsoleVersionMLADrums project to generate .dat file
//used mixedDataSet project to generate the .xml network file

int main()
{
	OpenNNClassifier* myClassifier = new OpenNNClassifier();

	MarsyasRealtime* marsyasRealTime = new MarsyasRealtime();

	MarsyasPlayerNet* marsyasPlayerNet = new MarsyasPlayerNet("center.wav");

	marsyasRealTime->net->updControl("AudioSource/otherSrc/mrs_natural/nChannels", marsyasRealTime->copt);
	marsyasRealTime->net->updControl("AudioSource/otherSrc/mrs_natural/inSamples", marsyasRealTime->bufferSize); //origin 512
	marsyasRealTime->net->updControl("AudioSource/otherSrc/mrs_natural/bufferSize", marsyasRealTime->bufferSize); //origin 512
	marsyasRealTime->net->updControl("AudioSource/otherSrc/mrs_real/israte", marsyasRealTime->sropt);
	marsyasRealTime->net->updControl("AudioSource/otherSrc/mrs_bool/initAudio", true);

	mrs_natural iterations = (mrs_natural)((marsyasRealTime->srate*marsyasRealTime->length) / marsyasRealTime->inSamples);

	cout << "Iterations = " << iterations << endl;

	for (mrs_natural t = 0; t < iterations; t++)
	{
		mrs_realvec processedData = marsyasRealTime->startExtraction();
		cout << processedData << endl;

		Vector<double> toClassify = myClassifier->convert(processedData);
		Vector<double> outputs = myClassifier->obtainOutputs(toClassify);
		size_t classificationIndex = myClassifier->obtainClassification(outputs);
		double value = outputs.at(classificationIndex);

		if (value < 0.5) {
			cout << "Below Threshold" << endl;
		}
		else {
			cout << "Above Threshold" << endl;
			if (classificationIndex == 0) {
				cout << "classif == 0" << endl;
				marsyasPlayerNet->setSoundFileName("center.wav");
				marsyasPlayerNet->playSound();
			}
			else if (classificationIndex == 1) {
				cout << "classif == 1" << endl;
				marsyasPlayerNet->setSoundFileName("halfedge.wav");
				marsyasPlayerNet->playSound();
			}
			else if (classificationIndex == 2) {
				cout << "classif == 2" << endl;
				marsyasPlayerNet->setSoundFileName("rimshot.wav");
				marsyasPlayerNet->playSound();
			}
			else {
				//nothing
			}
		}
		if (myClassifier->debug) { cout << "------------------------------------------" << endl; }

	}

	cin.get();

	delete marsyasRealTime->total;

	return 0;
}
