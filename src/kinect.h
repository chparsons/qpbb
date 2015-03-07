#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

class Kinect
{

public:

  Kinect(){};
  ~Kinect(){};  

  void init()
  {
    kinect.setRegistration(true);
    kinect.init(false, false);
    kinect.enableDepthNearValueWhite(true);
    kinect.open();
  };

  void update()
  {
    kinect.update();
    if ( ! kinect.isFrameNew() )
      return;
  };

private:

	ofxKinect kinect;

};

