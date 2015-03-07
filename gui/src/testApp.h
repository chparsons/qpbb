#pragma once

#include "ofMain.h"
#include "Data.h"
#include "Params.h"
#include "Timeline.h"
#include "ParallelCoords.h"

class testApp : public ofBaseApp
{

public:

  void setup();
  void update();
  void draw();
  void exit();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);


  Data data;
  Params params;
  Timeline chart_timeline;
  ParallelCoords chart_parallelcoords;

};

