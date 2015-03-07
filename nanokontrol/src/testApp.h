#pragma once

#include "ofMain.h"
#include "NanoKontrol2.h"
#include "ofxOsc.h"
#include "ParamData.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    NanoKontrol2 kontrol;
    void midiReceive(const MidiMessage& msg);

		ofxOscSender send2map, send2gui;
    void send( string param_name, float value );

    vector<ParamData> kmap;
    //bool kmap_contains( string param );

};

