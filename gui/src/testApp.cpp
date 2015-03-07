#include "testApp.h"


void testApp::setup(){

  ofSetFrameRate(30);
  ofSetVerticalSync(true);

  data.load();

  int winw = ofGetWidth();
  int winh = ofGetHeight();

  int m = 4;
  int pw = (winw - m*3) / 2;

  ofxGuiSetDefaultWidth( pw ); 

  params.setup( data, true );

  params.gui.setPosition( 
    m, winh - m - params.gui_plantas.getHeight() - m - params.gui.getHeight() );

  //invisible
  params.gui_info.setPosition(winw,0); 

  params.gui_plantas.setPosition( 
    m, winh - m - params.gui_plantas.getHeight() );

  params.gui_sustancias.setPosition( 
    pw + m*2, 
    winh - m - params.gui_sustancias.getHeight() );

  int chart_w = winw / 2.2;
  int chart_h = 100;
  chart_parallelcoords.init( 10, chart_h + 20, chart_w, chart_h );
  chart_timeline.init( winw - chart_w - 10, chart_h + 20, chart_w, chart_h );

}


void testApp::update(){

  ofSetWindowTitle( ofToString( ofGetFrameRate(), 2 ) );

  //params.timeline.set( sin( ofGetElapsedTimef() * 0.4 ) * 0.5 + 0.5 );

  params.update( data );

}


void testApp::draw(){

  ofBackground(255); 

  params.draw();

  chart_parallelcoords.render( params, data ); 
  chart_timeline.render( params, data ); 

}


void testApp::exit(){

  params.exit();

}


void testApp::keyPressed(int key)
{
  switch( key )
  {
    case 'f':
      ofToggleFullscreen();
      break;
  }
}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y ){

}


void testApp::mouseDragged(int x, int y, int button){

}


void testApp::mousePressed(int x, int y, int button){

}


void testApp::mouseReleased(int x, int y, int button){

}


void testApp::windowResized(int w, int h){

}


void testApp::gotMessage(ofMessage msg){

}


void testApp::dragEvent(ofDragInfo dragInfo){ 

}


