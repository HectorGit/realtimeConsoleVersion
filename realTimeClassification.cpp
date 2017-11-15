// realTimeClassification.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "opennn/vector.h" //wtf?
#include "OpenNNClassifier.h" 
#include "MarsyasRealTime.h" //commented out for now.

//used ConsoleVersionMLADrums project to generate .dat file
//used mixedDataSet project to generate the .xml network file

int main()
{
	OpenNNClassifier* myClassifier = new OpenNNClassifier();

	MarsyasRealtime* marsyasRealTime = new MarsyasRealtime();

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
		}
		if (myClassifier->debug) { cout << "------------------------------------------" << endl; }

	}

	cin.get();

	delete marsyasRealTime->total;

    return 0;
}

