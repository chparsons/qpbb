#include "fns.h"

namespace fns
{

  float indice( 
      float _indice,
      Data& data,
      Params& params )
  { 

    float _indice_n = ofMap( 
        _indice, 
        //data.min_indice(), 
        0., data.max_indice(), 
        0., 1. );

    return pow( _indice_n, params.indice_pow ) * params.indice_lin;

  };

  float indice_n( 
      float _indice,
      Data& data,
      Params& params )
  { 

    float _indice_fn = fns::indice( _indice, data, params );

    float _indice_max_fn = pow( 1.f, params.indice_pow ) * params.indice_lin;

    return _indice_fn / _indice_max_fn;
  };

  bool in_range(float t, Params& params)
  {
    return t > params.timerange_min() && t < params.timerange_max();
  };

  float date_to_t( Data::Sustancia& s, Data& data )
  {
    return utils::date_to_t(
        s.date,
        data.min_date(),
        data.max_date() );
  };

};

