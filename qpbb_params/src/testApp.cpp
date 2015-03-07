#include "testApp.h"

void testApp::setup()
{

  string host = "192.168.0.103";
  bool sender = true;


  int port_local = sender ? 6667 : 6666;
  int port_remote = sender ? 6666 : 6667; 


  ofSetVerticalSync(true);

	// change default sizes for ofxGui so it's usable in small/high density screens
	ofxGuiSetFont("verdana.ttf",18,true,true);
	ofxGuiSetTextPadding(20);
	ofxGuiSetDefaultWidth(300);
	ofxGuiSetDefaultHeight(40);


  // params

  _params.setName("params");

  _params.add( 
      timeline.set("timeline",
        0.5, 0, 1 ));

  _params.add( 
      timerange.set("timerange",
        0.25, 0.001, 1 ));

  _params.add(
      indice_pow.set("indice_^N",
        0.3, 0., 1. ));

  _params.add(
      indice_lin.set("indice_xN",
        1000., 100., 1000. ));

  _params.add( 
      alfa_min.set("alfa_min",
        0.025, 0, 0.2 ));	

  int x = 10;
  int y = 10;
  int w = ofGetWidth() - x * 2;
  ofxGuiSetDefaultWidth( w );

  gui.setup( _params, "settings.xml", x, y );
  //gui.setSize( w, gui.getHeight() );

  sync.setup((ofParameterGroup&)gui.getParameter(), port_local, host, port_remote);

}

void testApp::update()
{
  sync.update();
}

void testApp::draw()
{

  ofBackgroundGradient(ofColor::white, ofColor::gray);

  //ofFill();
	//ofSetColor(ofColor(100,100,140));
  //ofCircle(ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5), 140 );

  gui.draw();

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void testApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void testApp::pause(){

}

//--------------------------------------------------------------
void testApp::stop(){

}

//--------------------------------------------------------------
void testApp::resume(){

}

//--------------------------------------------------------------
void testApp::reloadTextures(){

}

//--------------------------------------------------------------
bool testApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void testApp::okPressed(){

}

//--------------------------------------------------------------
void testApp::cancelPressed(){

}

