#pragma once

#include "ofxOscParameterSync.h"
#include "ofxGui.h"

#include "Data.h"
#include "ParamData.h"
#include "utils.h"

class Params
{

public:

  Params(){};
  //~Params(){}; 

  void setup( Data& data, bool sender, string host = "localhost" )
  {

    int port_local = sender 
      ? Port::sender 
      : Port::receiver;

    int port_remote = sender
      ? Port::receiver 
      : Port::sender;

    int port_sustancias_local = sender 
      ? PortSustancias::sender 
      : PortSustancias::receiver;

    int port_sustancias_remote = sender
      ? PortSustancias::receiver 
      : PortSustancias::sender;

    int port_plantas_local = sender 
      ? PortPlantas::sender 
      : PortPlantas::receiver;

    int port_plantas_remote = sender
      ? PortPlantas::receiver 
      : PortPlantas::sender;


    _sustancias_vis = true;
    _plantas_vis = true;


    _params.setName("params");

    _params.add( 
      timeline.set(
        pdls.timeline.id,
        pdls.timeline.ini,
        pdls.timeline.min,
        pdls.timeline.max 
      ));

    _params.add( 
      timerange.set(
        pdls.timerange.id,
        pdls.timerange.ini,
        pdls.timerange.min,
        pdls.timerange.max
      )); 

    _params.add( 
      indice_pow.set(
        pdls.indice_pow.id,
        pdls.indice_pow.ini,
        pdls.indice_pow.min,
        pdls.indice_pow.max
      ));

    _params.add( 
      indice_lin.set(
        pdls.indice_lin.id,
        pdls.indice_lin.ini,
        pdls.indice_lin.min,
        pdls.indice_lin.max
      ));

    _params.add( 
      alfa_min.set(
        pdls.alfa_min.id,
        pdls.alfa_min.ini,
        pdls.alfa_min.min,
        pdls.alfa_min.max
      )); 

    //_params.add( 
        //iumbral.set("umbral_indice",
          //100,0,30000) ); 
    //_params.add( 
        //alfa_min_pos.set(
          //"alfa_min_pos_umbral",
          //0.1,0,0.3) ); 

    _params.add( 
      vmode.set(
        pdls.vmode.id,
        pdls.vmode.ini,
        pdls.vmode.min,
        pdls.vmode.max
      ));

    //_params.add( 
      //imode.set(
        //pdls.imode.id,
        //pdls.imode.ini,
        //pdls.imode.min,
        //pdls.imode.max
      //));

    _params.add( 
      rmin.set(
        pdls.rmin.id, pdls.rmin.ini,
        pdls.rmin.min, pdls.rmin.max
      ));

    //_params.add( 
      //rmax.set(
        //pdls.rmax.id, pdls.rmax.ini,
        //pdls.rmax.min, pdls.rmax.max
      //));

    gui.setup( _params );

    //gui.add( 
    //vmode_bt.setup("modo vis") );
    //gui.add( 
    //rmode_bt.setup("modo radio") ); 

    sync.setup( (ofParameterGroup&)gui.getParameter(), port_local, host, port_remote );

    //vmode_bt.addListener(this,&testApp::vmode_change);
    //rmode_bt.addListener(this,&testApp::rmode_change);


    // gui info

    _params_info.setName("info"); 

    //_params_info.add( 
      //rmin.set(
        //pdls.rmin.id, pdls.rmin.ini,
        //pdls.rmin.min, pdls.rmin.max
      //));

    _params_info.add( 
      rmax.set(
        pdls.rmax.id, pdls.rmax.ini,
        pdls.rmax.min, pdls.rmax.max
      ));

    gui_info.setup( _params_info );

    //gui_info.add( 
      //indice_str.set("_indice",""));
    gui_info.add( 
      d_desde_str.set("_desde",""));
    gui_info.add( 
      d_hasta_str.set("_hasta",""));
    //gui_info.add( 
      //timerange_str.set("_fecha_rango_",""));


    // _params sustancias activas

    _params_sustancias.setName("sustancias ordenadas x indice de desvio");

    //lista de sustancias 
    //sorted x indices sum

    vector<Data::sustancia_name_stats_pair>& stats = data.stats_x_sustancia_sorted();

    for (int i = 0; i < stats.size(); i++)
    {
      string analito = stats[i].first; 
      sustancias_visibles
        [ analito ] = _sustancias_vis;
    }

    for (int i = 0; i < stats.size(); i++)
    {
      string analito = stats[i].first; 
      ofParameter<bool>& p = sustancias_visibles[ analito ];

      _params_sustancias.add( p.set( analito, _sustancias_vis ) );
    }

    gui_sustancias.setup( _params_sustancias );

    gui_sustancias.add( toggle_sustancias_bt.setup("toggle_sustancias") );

    toggle_sustancias_bt.addListener( this, &Params::toggle_sustancias );

    sync_sustancias.setup( (ofParameterGroup&)gui_sustancias.getParameter(), port_sustancias_local, host, port_sustancias_remote );


    // _params plantas

    _params_plantas.setName("plantas");

    vector<Data::Planta>& plantas = 
      data.plantas();

    for ( int i = 0; 
        i < plantas.size(); 
        i++ )
    { 
      Data::Planta& planta = plantas[ i ];

      string planta_nombre = data.planta_nombre( planta.id );
      //str for osc done in data...
      //utils::str_for_osc(planta_nombre);

      plantas_visibles
        [ planta_nombre ] = _plantas_vis;

      ofParameter<bool>& p = 
        plantas_visibles[ planta_nombre ];

      _params_plantas.add( p.set( planta_nombre, _plantas_vis ) );
    }

    gui_plantas.setup( _params_plantas );

    gui_plantas.add( toggle_plantas_bt.setup("toggle_plantas") );

    toggle_plantas_bt.addListener( this, &Params::toggle_plantas );

    sync_plantas.setup( (ofParameterGroup&)gui_plantas.getParameter(), port_plantas_local, host, port_plantas_remote );


  };

  void update( Data& data )
  {

    //timeline = sin( ofGetElapsedTimef() * 0.4 ) * 0.5 + 0.5;

    sync.update();
    sync_sustancias.update();
    sync_plantas.update();

    //indice_str = ofToString(indice_lin*indice_xNN);


    ofxDate dmin = data.min_date();
    ofxDate dmax = data.max_date();

    float tmin = timerange_min();
    float tmax = timerange_max();

    d_desde_str = utils::t_to_date( 
        tmin, dmin, dmax )
      .getIsoDate();

    d_hasta_str = utils::t_to_date( 
        tmax, dmin, dmax )
      .getIsoDate();

    //timerange_str = utils::t_to_date( 
        //timerange, dmin, dmax )
      //.getIsoDate(); 

  };

  void draw()
  {
    gui.draw();
    gui_info.draw();
    gui_sustancias.draw();
    gui_plantas.draw();
  };

  void exit()
  {

    gui.clear();
    gui_info.clear();
    gui_sustancias.clear();
    gui_plantas.clear();

    _params.clear();
    _params_info.clear();
    _params_sustancias.clear();
    _params_plantas.clear();

    toggle_sustancias_bt.removeListener(this,&Params::toggle_sustancias);

    toggle_plantas_bt.removeListener(this,&Params::toggle_plantas);

    //vmode_bt.removeListener(this,&testApp::vmode_change);
    //rmode_bt.removeListener(this,&testApp::rmode_change);

  };

  float timerange_min() const
  {
    return CLAMP( timeline - timerange * 0.5, 0., 1. );
  };

  float timerange_max() const
  {
    return CLAMP( timeline + timerange * 0.5, 0., 1. );
  };

  void log()
  {
    ofLog() << "============"; 

    ofLog() << "sustancias visibles"; 
    for ( map< string,ofParameter<bool> >::iterator it = sustancias_visibles.begin(); it != sustancias_visibles.end(); ++it )
    {
      string analito = it->first;
      ofParameter<bool>& p = it->second;
      ofLog() << analito << " " << p;
    }

    ofLog() << "plantas visibles"; 
    for ( map< string,ofParameter<bool> >::iterator it = plantas_visibles.begin(); it != plantas_visibles.end(); ++it )
    {
      string pnombre = it->first;
      ofParameter<bool>& p = it->second;
      ofLog() << pnombre << " " << p;
    }

  };

  ofParameter<float> timeline;
  ofParameter<float> timerange;
  ofParameter<float> alfa_min;
  //ofParameter<float> alfa_min_pos;
  //ofParameter<float> iumbral;
  ofParameter<float> rmin,rmax;
  ofParameter<float> indice_pow;
  ofParameter<float> indice_lin;

  //ofParameter<string> indice_str;
  ofParameter<string> d_desde_str;
  ofParameter<string> d_hasta_str;
  //ofParameter<string> timerange_str;
  ofParameter<int> vmode; 
  //ofParameter<int> imode; 

  map< string,ofParameter<bool> > sustancias_visibles; 
  map< string,ofParameter<bool> > plantas_visibles; 

  ofxPanel gui;
  ofxPanel gui_info;
  ofxPanel gui_sustancias;
  ofxPanel gui_plantas;

private:

  ParamDataList pdls;

  ofParameterGroup _params;
  ofParameterGroup _params_info;
  ofParameterGroup _params_sustancias;
  ofParameterGroup _params_plantas;

  ofxOscParameterSync sync; 
  //ofxOscParameterSync sync_info; 
  ofxOscParameterSync sync_sustancias;
  ofxOscParameterSync sync_plantas;

  ofxButton toggle_sustancias_bt;
  bool _sustancias_vis;

  ofxButton toggle_plantas_bt;
  bool _plantas_vis;

  //ofxButton vmode_bt;
  //void vmode_change(); 

  //ofxButton rmode_bt;
  //void rmode_change(); 

  void toggle_sustancias()
  {
    _sustancias_vis = !_sustancias_vis;

    for ( map< string,ofParameter<bool> >::iterator it = sustancias_visibles.begin(); it != sustancias_visibles.end(); ++it )
    {
      string analito = it->first;
      ofParameter<bool>& p = it->second;
      p.set( analito, _sustancias_vis );
    }

  };

  void toggle_plantas()
  {
    _plantas_vis = !_plantas_vis;

    for ( map< string,ofParameter<bool> >::iterator it = plantas_visibles.begin(); it != plantas_visibles.end(); ++it )
    {
      string pnombre = it->first;
      ofParameter<bool>& p = it->second;
      p.set( pnombre, _plantas_vis );
    }

  };

  //void vmode_change()
  //{
    //vmode = (vmode+1) % vmode_length;
  //}

  //void rmode_change()
  //{
    //imode = (imode+1) % rmode_length;
  //}

};

