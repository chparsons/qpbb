#pragma once

#include "ofMain.h"
#include "ofxKeystoneTexture.h"
#include "ofxMercatorMap.h"
#include "ofxDate.h"
#include "ofxXmlSettings.h"
#include "ofxOscParameterSync.h"
#include "ofxGui.h"

#include "homography.h"
#include "key.h"
#include "utils.h"
#include "Data.h"
#include "Params.h"
#include "Timeline.h"
#include "ColorBrewer.h"


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
  Timeline timeline;

  bool draw_params;
  bool draw_map;


  ofxXmlSettings ktex_cfg;
  ofxKeystoneTexture ktex;
  ofFbo fbo;
  ofImage mapimg;
  ofxMercatorMap mercator;
  ofVboMesh lineas;


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


  void render_sustancias();
  bool do_render_sustancia( 
      const Data::Sustancia& s );

  void render_circulo(
      int j, int slen,
      const ofPoint& p, 
      float r, float _alfa );

  void render_linea(
      int j, int slen,
      const ofPoint& p, 
      float r, float _alfa );


  float radio(
      Data::Sustancia& s );

  float radio_mapeado_x_sustancia(
      Data::Sustancia& s );

  float radio_normal(
      Data::Sustancia& s );


  void log_sustancias();


};

