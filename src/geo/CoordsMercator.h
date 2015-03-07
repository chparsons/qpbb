#pragma once

#include "Coords.h"
#include "ofxMercatorMap.h"

class CoordsMercator : public Coords
{

public:

  CoordsMercator(){}; 
  ~CoordsMercator();

  ofPoint get_screen_loc( Data::Planta& planta )
  {
    return mercator
      .getScreenLocationFromLatLon(
        ofToFloat( planta.lat ), 
        ofToFloat( planta.lon ) ); 
  };

  void init(
    int width, int height,
    float O, float S, float E, float N )
  {
    mercator.setup( 
        width, height, 
        O,S,E,N ); 
  };

private:

  ofxMercatorMap mercator;

};

