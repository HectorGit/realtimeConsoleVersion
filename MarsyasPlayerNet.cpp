#pragma once

#include "stdafx.h"
#include "MarsyasPlayerNet.h"

using namespace Marsyas;
using namespace std;

MarsyasPlayerNet::MarsyasPlayerNet(string soundFileNameParam) {
	cout << "Constructing MarsyasPlayerNet" << endl;

	soundFileName = soundFileNameParam;

	dnet = mng.create("Series", "dnet");

	dnet->addMarSystem(mng.create("SoundFileSource", "src"));
	dnet->addMarSystem(mng.create("Gain", "gain"));
	dnet->addMarSystem(mng.create("AudioSink", "adest"));
	//dnet->addMarSystem(mng.create("AudioSource", "asrc")); // do we need this?
	//dnet->addMarSystem(mng.create("SoundFileSink", "dest"));


	// dnet->updControl("mrs_real/israte", 44100.0);
	// dnet->updControl("AudioSource/asrc/mrs_natural/nChannels", 1);


	dnet->updControl("SoundFileSource/src/mrs_string/filename", soundFileName);
	//dnet->updControl("SoundFileSink/dest/mrs_string/filename", "duplex2.wav");
	dnet->updControl("AudioSink/adest/mrs_bool/initAudio", true);
	//dnet->updControl("AudioSource/asrc/mrs_bool/initAudio", true);

}

MarsyasPlayerNet::~MarsyasPlayerNet() {
	delete dnet;
}

//change the control
void MarsyasPlayerNet::setSoundFileName(string newSoundFileName) {
	dnet->updControl("SoundFileSource/src/mrs_string/filename", newSoundFileName);
	//is this enough? 
	//what else should be done?
}

//tick
void MarsyasPlayerNet::playSound() {

	cout << "playing sound" << endl;

	while (dnet->getctrl("SoundFileSource/src/mrs_bool/hasData")->to<mrs_bool>())
	{
		dnet->tick();
	}
}