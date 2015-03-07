#pragma once

#include "ofMain.h"
#include "Data.h"
#include "Params.h"
#include "utils.h"

namespace fns
{

  float indice( 
      float _indice,
      Data& data,
      Params& params );

  float indice_n( 
      float _indice,
      Data& data,
      Params& params );
  
  float date_to_t( Data::Sustancia& s, Data& data );
  bool in_range(float t, Params& params);

};

