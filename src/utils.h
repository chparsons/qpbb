#pragma once

#include "ofMain.h"
#include "ofxDate.h"

namespace utils
{

  bool is_nan( float n );
  bool is_inf( float n );
  bool is_num( float n );
  bool equals( float n, float m );
  void str_for_osc( string& str );

  template <typename M, typename V> 
  void map_values( const M & m, V & v );

  void ofSetColorf( 
    float r, float g, float b, float a )
  {
    ofSetColor( ofFloatColor(r,g,b,a) );
  };

  // time

  /*
   * converts a date
   * to t days past
   * normalized [0,1]
   */
  float date_to_t(
      ofxDate& date,
      ofxDate& min, 
      ofxDate& max );

  /*
   * converts t days past
   * normalized [0,1]
   * to date
   */
  ofxDate t_to_date(
      float t,
      ofxDate& min, 
      ofxDate& max );

  ofxDate& to_date(
      const string& isodate,
      ofxDate& f ); 

};

