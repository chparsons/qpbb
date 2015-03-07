#include "testApp.h"

ParamDataList pdls;
ParamData nullp;

void testApp::setup()
{

  ofBackground(0);
  ofSetFrameRate(30);
	ofSetVerticalSync(true);

  kontrol.setup();

  ofAddListener(kontrol.midiReceiveEvent, this, &testApp::midiReceive);

	send2map.setup( "localhost", Port::receiver );
	send2gui.setup( "localhost", Port::sender );

  kmap.insert(kmap.begin(), 100, nullp);

  kmap[22] = pdls.timeline;
  kmap[21] = pdls.timerange;
  kmap[20] = pdls.indice_pow;
  kmap[19] = pdls.indice_lin;
  kmap[18] = pdls.alfa_min;
  //TODO
  //kmap[17] = pdls.vmode;
  //kmap[16] = pdls.rmode;
  //kmap[20] = pdls.rmin;
  //kmap[19] = pdls.rmax;

}

void testApp::update()
{

  ofSetWindowTitle( ofToString( ofGetFrameRate(), 2 ) );

}

void testApp::draw()
{

  //kontrol.draw();

}

void testApp::midiReceive(const MidiMessage& msg)
{
  if (msg.buffer.empty())
    return;
  
  MidiMessage::MessageType messageType = (MidiMessage::MessageType)msg.buffer.at(0);

  switch ((messageType&0xf0))
  {
    case MidiMessage::ControlChange:
    {
      size_t idx = msg.buffer.at(1);
      //float val = ofMap(msg.buffer.at(2), 0, 127, 0.0, 1.0);
      int val = msg.buffer.at(2);

      ParamData param = kmap[idx];
      if ( param.id != "undefined" )
      {
        send( param.id, ofMap( val, 0, 127, param.min, param.max ) );
      }

      ofLog() << "nanokontrol "
        << idx << ": " << val
        << " param " << param.id
        << ""; 

      break;
    }

  }

}

//bool testApp::kmap_contains( string param )
//{
  //vector<string>::iterator it = std::find(kmap.begin(), kmap.end(), param);
  //return it != kmap.end();
//};

void testApp::send( string param_name, float value )
{

  ofxOscMessage m;
  m.setAddress("/params/"+param_name);
  m.addFloatArg( value );
  send2map.sendMessage(m);
  send2gui.sendMessage(m);

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

