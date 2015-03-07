#pragma once

/*
 * gui -> mapa
 * nanokontrol -> mapa
 * nanokontrol -> gui
 */

struct Port
{ 
  static const int sender = 6667;
  static const int receiver = 6666;
};

struct PortSustancias
{
  static const int sender = 6669;
  static const int receiver = 6668;
};

struct PortPlantas
{
  static const int sender = 6671;
  static const int receiver = 6670;
};

struct ParamData
{
  string id = "undefined";
  float ini = 0;
  float min = 0;
  float max = 1;
};

class ParamDataList
{
public:

  ParamDataList()
  {

    timeline.id = "timeline";
    timeline.ini = 0.5;

    timerange.id = "timerange";
    timerange.ini = 0.25;
    timerange.min = 0.001;
    timerange.max = 1;

    rmin.id = "rmin";
    rmin.ini = 10;
    rmin.min = 0;
    rmin.max = 30;

    rmax.id = "rmax";
    rmax.ini = 30;
    rmax.min = 0;
    rmax.max = 300;

    indice_pow.id = "indice_^N";
    indice_pow.ini = 0.3;
    indice_pow.min = 0.;
    indice_pow.max = 1.;

    indice_lin.id = "indice_xN";
    indice_lin.ini = 1000.;
    indice_lin.min = 100.;
    indice_lin.max = 1000.;

    alfa_min.id = "alfa_min";
    alfa_min.ini = 0.025;
    alfa_min.min = 0.;
    alfa_min.max = 0.2;

    vmode.id = "modo_vis";
    vmode.ini = 0;
    vmode.min = 0;
    vmode.max = 2;

    //imode.id = "fn_indice";
    //imode.ini = 0;
    //imode.min = 0;
    //imode.max = 2;

  };

  ParamData timeline;
  ParamData timerange;

  ParamData rmin;
  ParamData rmax;
  ParamData indice_pow;
  ParamData indice_lin;

  ParamData alfa_min; 

  ParamData vmode; 
  //ParamData imode; 
};

