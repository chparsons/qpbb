#pragma once

#include "ofMain.h"
#include "Params.h"
#include "fns.h"
#include "utils.h"

class Timeline
{

public:

  Timeline(){};
  //~Timeline(){}; 

  void init(
      float _x, float _y,
      float _w, float _h )
  {
    update( _x, _y, _w, _h );
    mesh.setMode( OF_PRIMITIVE_LINES );
  };

  void update( 
      float _x, float _y,
      float _w, float _h )
  {
    tx = _x;
    ty = _y;
    //ty = _y - 40;
    tw = _w;
    th = _h;

    txmin = tx;
    txmax = txmin + tw;

    BL = ofVec3f( txmin, ty, 0 );
    BR = ofVec3f( txmax, ty, 0 );
  };

  void render(Params& params, Data& data)
  { 

    // fecha seleccionada
    //ofDrawBitmapStringHighlight( "[ " + params.d_desde_str.get() + " -- " + params.d_hasta_str.get() + " ]", x, y, ofColor::yellow, ofColor::magenta );

    // timeline

    mesh.clear();

    ofColor color = ofColor::black;

    ofSetLineWidth( 0.01 );

    ofSetColor( color );
    ofDrawBitmapString( data.min_date().getIsoDate().substr(0,4), txmin, ty + 15 );
    ofDrawBitmapString( data.max_date().getIsoDate().substr(0,4), txmax - 35, ty + 15 );

    line( BL, BR, color );

    ofVec3f vhoff( 0, 10, 0 );

    float tmin = tw * params.timerange_min();
    ofVec3f tvmin( txmin + tmin, ty, 0 );
    line( tvmin, tvmin + vhoff, color );

    float tmax = tw * params.timerange_max();
    ofVec3f tvmax( txmin + tmax, ty, 0 );
    line( tvmax, tvmax + vhoff, color );

    sustancias( params, data );

    mesh.draw();
  };

private:

  ofVboMesh mesh;

  float tw, th, tx, ty;
  float txmin, txmax;

  ofVec3f BL, BR;

  void sustancias( 
      Params& params, Data& data )
  {

    float _alfa = 0.1;

    vector<Data::Planta>& plantas = 
      data.plantas();

    for ( int i = 0; 
        i < plantas.size(); 
        i++ )
    { 
      Data::Planta& planta = plantas[i];
      int slen=planta.sustancias.size(); 

      for ( int j = 0; j < slen; j++ )
      {
        Data::Sustancia& s = 
          planta.sustancias[ j ];

        if ( s.indice == 0 )
          continue;

        float _indice_n = fns::indice_n( s.indice, data, params ); 

        float _time_n = fns::date_to_t( s, data );
        bool in_range = fns::in_range( _time_n, params );

        ofFloatColor color = in_range
          ? ofFloatColor( 1,0,0, _alfa )
          : ofFloatColor(.3,.3,.3,_alfa);

        ofVec3f off_src = ofVec3f( _time_n * tw, 0. );
        ofVec3f off_dst = ofVec3f( _time_n * tw, -_indice_n * th );
        ofVec3f src = BL + off_src;
        ofVec3f dst = BL + off_dst;
        line( src, dst, color );

      }
    }

  };

  void line( const ofVec3f& src, const ofVec3f& dst, const ofColor& color )
  {
    mesh.addVertex( src );
    mesh.addColor( color );
    mesh.addVertex( dst );
    mesh.addColor( color );
  };

};


