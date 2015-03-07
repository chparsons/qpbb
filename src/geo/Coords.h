#pragma once

#include "ofMain.h"
#include "Data.h"

class Coords
{

public:

  virtual ofPoint get_screen_loc( Data::Planta& planta ) = 0;

};

