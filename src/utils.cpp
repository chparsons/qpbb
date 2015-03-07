#include "utils.h"

namespace utils
{

  bool is_nan( float n )
  {
    return n != n;
  };

  bool is_inf( float n )
  {
    return n > 0 && n/n != n/n;
  };

  bool is_num( float n )
  {
    return !is_nan(n) && !is_inf(n);
  };

  bool equals( float n, float m )
  {
    return ABS( n - m ) < FLT_EPSILON;
  };

  template <typename M, typename V> 
  void map_values( const M & m, V & v ) 
  {
    for ( typename M::const_iterator it = m.begin(); it != m.end(); ++it ) 
      v.push_back( it->second );
  };

  void str_for_osc( string& str )
  {
    ofStringReplace(str, " ", "_");
    ofStringReplace(str, "/", "_");
    ofStringReplace(str, ".", "_");
    ofStringReplace(str, ",", "_");
    ofStringReplace(str, "(", "_");
    ofStringReplace(str, ")", "_");
    ofStringReplace(str, "á", "a");
    ofStringReplace(str, "é", "e");
    ofStringReplace(str, "í", "i");
    ofStringReplace(str, "ó", "o");
    ofStringReplace(str, "ú", "u");
  };

  // time

  float date_to_t(
      ofxDate& date,
      ofxDate& min, 
      ofxDate& max )
  {

    int days = date.getDaysPast();
    int min_days = min.getDaysPast();
    int max_days = max.getDaysPast();

    return ofMap( days, 
        min_days, max_days, 0.0, 1.0 );
  };

  ofxDate t_to_date(
      float t,
      ofxDate& min, 
      ofxDate& max )
  {

    int min_days = min.getDaysPast();
    int max_days = max.getDaysPast();

    float days = ofMap( t,
        0.0, 1.0, min_days, max_days,
        true );

    return ofxDate( days );
  };

  ofxDate& to_date(
      const string& isodate,
      ofxDate& f )
  {
    //2011-01-27T11:00:00-03:00

    vector<string> fstr = ofSplitString( isodate.substr(0,10), "-" );

    //ofxDate f;
    f.setYear( ofToInt( fstr[0] ) );
    f.setMonth( ofToInt( fstr[1] ) );
    f.setDay( ofToInt( fstr[2] ) );

    return f;
  }; 

};

