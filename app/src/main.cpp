
//#define USE_PROGRAMMABLE_GL

#include "ofMain.h"
#include "testApp.h"

int main()
{

  //ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);

  ofSetupOpenGL(1024,768,OF_WINDOW);	

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}