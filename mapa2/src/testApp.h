#pragma once

#include "ofMain.h"
#include "ofxQuadWarp.h"
#include "ofxDate.h"
#include "ofxXmlSettings.h"
#include "ofxOscParameterSync.h"
#include "ofxGui.h"
#include "ofxColorBrewer.h"

#include "homography.h"
#include "key.h"
#include "utils.h"
#include "fns.h"
#include "Data.h"
#include "Params.h"
#include "Coords.h"
//#include "CoordsMercator.h"
#include "CoordsHardcoded.h"
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

  bool pressed[512];


  Data data;
  Params params;
  Coords* coords;
  Timeline chart_timeline;
  ParallelCoords chart_parallelcoords;

  bool draw_params;
  bool draw_charts;
  bool draw_map;
  bool calib_ktex;


  ofxQuadWarp warper;
  ofxXmlSettings ktex_cfg;
  ofFbo fbo;
  ofImage mapimg;
  ofVboMesh mesh_lineas;
  ofVboMesh mesh_circulos;

  ofxColorBrewer colorbrewer;
  map<string,ofFloatColor> sustancias_colores;

  // homography calib
  bool render_calib();
  ofMatrix4x4 H;
  vector<ofPoint> src;
  vector<ofPoint> dst;
  //float H[16];
  //ofPoint src[4];
  //ofPoint	dst[4];


  void fbo_clear();

  void load_ktex();
  void save_ktex();
  void reset_ktex();
  //void center_ktex();
  //void reset_tex_ts();
  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();


  void render();
  bool filter_sustancia( 
      Data::Sustancia& s ); 

  void render_sustancias(
    const ofPoint& p,
    vector<Data::Sustancia*>& sust_vis );

  void render_circulo(
    int j, int slen, float r,
    const ofPoint& p, 
    const ofFloatColor& color );

  void render_linea(
    int j, int slen, float r,
    const ofPoint& p, 
    const ofFloatColor& color );

  void render_planta_props( 
    const ofPoint& p,
    vector<Data::Sustancia*>& sust_vis );


  float radio_map_x_sustancia(
      Data::Sustancia& s );


  void log_sustancias();
  void log_plantas_coords();


};

