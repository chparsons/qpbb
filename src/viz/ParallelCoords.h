#pragma once

#include "ofMain.h"
#include "Params.h"
#include "fns.h"
#include "utils.h"

class ParallelCoords
{

public:

  ParallelCoords(){};
  //~ParallelCoords(){}; 

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
    tw = _w;
    th = _h;

    txmin = tx;
    txmax = txmin + tw;

    tymin = ty;
    tymax = tymin - th;

    BL = ofVec3f( txmin, tymin );
    BR = ofVec3f( txmax, tymin );
    TR = ofVec3f( txmax, tymax );

    //tiempo,indices,plantas
    dims = 3;
    xspan = tw / (dims-1);
  };

  void render(Params& params, Data& data)
  { 

    // fecha seleccionada
    //ofDrawBitmapStringHighlight( "[ " + params.d_desde_str.get() + " -- " + params.d_hasta_str.get() + " ]", x, y, ofColor::yellow, ofColor::magenta );

    mesh.clear();

    ofSetLineWidth( 0.01 );

    //xaxis
    line( BL, BR, ofColor::black );

    //time
    yaxis( 0, data.min_date().getIsoDate().substr(0,4), data.max_date().getIsoDate().substr(0,4) );
    time_ticks( params ); 

    //indices
    float _indice_n = fns::indice_n( data.max_indice() / 10., data, params ); 
    yaxis( 1, 
      //ofToString( data.min_indice() ), 
      "0.",
      //ofToString( data.max_indice() ) );
      ofToString( _indice_n * data.max_indice() ) );

    //plantas
    yaxis( 2, "", "plantas" );

    sustancias( params, data );

    mesh.draw();
  };

private:

  ofVboMesh mesh;

  int dims;
  float xspan;

  float tw, th, tx, ty;
  float txmin, txmax;
  float tymin, tymax;

  ofVec3f BL, BR, TR;

  void yaxis( int xloc, string min = "", string max = "" )
  {
    ofColor color = ofColor::black;

    ofVec3f xoff = ofVec3f(xspan*xloc,0);
    ofVec3f _ymin = BR - xoff;
    ofVec3f _ymax = TR - xoff;

    line( _ymin, _ymax, color );

    ofSetColor( color );
    ofDrawBitmapString( min, _ymin.x - 10, _ymin.y + 15 );
    ofDrawBitmapString( max, _ymax.x - 10, _ymax.y - 10 );

  };

  void time_ticks( Params& params )
  {
    ofColor color = ofColor::black; 

    ofVec3f xoff( 10, 0 );

    float tmin = th * params.timerange_min();
    float tmax = th * params.timerange_max();

    ofVec3f tvmin( txmax, ty - tmin );
    ofVec3f tvmin_dst( tvmin + xoff );
    line( tvmin, tvmin_dst, color );

    ofVec3f tvmax( txmax, ty - tmax );
    ofVec3f tvmax_dst( tvmax + xoff );
    line( tvmax, tvmax_dst, color ); 

  };

  void sustancias(
      Params& params, Data& data )
  {

    float alfa = 0.01;

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

        float _indice_n = fns::indice_n( s.indice, data, params ) * 10.; 
        if ( _indice_n > 1. )
          continue;

        float _time_n = fns::date_to_t( s, data );
        bool in_range = fns::in_range( _time_n, params );

        float _planta_n = (float)i / plantas.size();

        ofFloatColor color = in_range
          ? ofFloatColor( 1,0,0, alfa )
          : ofFloatColor(.2,.2,.2,alfa);

        // time -> indice
        parallel_coord( 0, _time_n, _indice_n, color );
        // indice -> planta
        parallel_coord( 1, _indice_n, _planta_n, color );

      }
    }

  };

  void parallel_coord( int xloc, float src_n, float dst_n, ofFloatColor& color )
  {
    ofVec3f off_src = ofVec3f( xspan * xloc, src_n * th );
    ofVec3f off_dst = ofVec3f( xspan * (xloc+1), dst_n * th );
    ofVec3f src = BR - off_src;
    ofVec3f dst = BR - off_dst;
    line( src, dst, color );
  };

  void line( const ofVec3f& src, const ofVec3f& dst, const ofColor& color )
  {
    mesh.addVertex( src );
    mesh.addColor( color );
    mesh.addVertex( dst );
    mesh.addColor( color );
  };

};


