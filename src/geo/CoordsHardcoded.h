#pragma once

#include "Coords.h"

class CoordsHardcoded : public Coords
{

public:

  CoordsHardcoded()
  {
    loc["30"] = ofPoint(872.0,372.0);
    loc["31"] = ofPoint(-10.0,-10.0);
    loc["15"] = ofPoint(472.0,340.0);
    loc["22"] = ofPoint(49.0,440.0);
    loc["23"] = ofPoint(600.0,-10.0);
    loc["12"] = ofPoint(508.0,568.0);
    loc["18"] = ofPoint(963.0,666.0);
    loc["19"] = ofPoint(241.0,472.0);
    loc["21"] = ofPoint(43.0,204.0);
    loc["5"] = ofPoint(149.0,237.0);
    loc["4"] = ofPoint(330.0,277.0);
    loc["3"] = ofPoint(190.0,286.0);
    loc["1"] = ofPoint(285.0,230.0);
    loc["2"] = ofPoint(238.0,194.0);
    loc["6"] = ofPoint(47.0,459.0);
    loc["11"] = ofPoint(106.0,-250.0);
    loc["10"] = ofPoint(397.0,535.0);
    loc["7"] = ofPoint(386.0,366.0);
    loc["20"] = ofPoint(664.0,597.0);
    loc["16"] = ofPoint(-10.0,-10.0);
    loc["29"] = ofPoint(242.0,589.0);
  };

  ~CoordsHardcoded();

  ofPoint get_screen_loc( Data::Planta& planta )
  {
    return loc[ planta.id ]; 
  };

private:

  map<string,ofPoint> loc;

};

