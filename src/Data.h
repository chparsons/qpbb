#pragma once

#include "ofxJSONElement.h"
#include "ofxRegex.h"
#include "utils.h"

class Data
{

public:

  Data(){};
  ~Data(){};  

  class SustanciaStats
  {
    public:
    vector<float> indices;
    float min, max, sum;
  };

  // =~ Medicion
  class Sustancia
  {
    public:

    string id_efluente;
    string id_planta; //es el nombre!
    string fecha;
    string analito;
    string metodo;
    string deteccion;
    string legislacion;
    string resultado;

    //indice de contaminacion
    //indice = resultado / legislacion;
    float resultado_f;
    float legislacion_f;
    float indice;

    ofxDate date;

    void log( string tab = "\t\t" )
    {
      ofLog() << "sustancia"

        << "\n" << tab
        << "id_planta " << id_planta
        << "\n" << tab
        << "id_efluente " << id_efluente

        << "\n" << tab
        << "fecha " << fecha
        << "\n" << tab
        << "analito " << analito
        << "\n" << tab
        << "deteccion " << deteccion
        << "\n" << tab
        << "legislacion " << legislacion 
        << "\n" << tab 
        << "resultado " << resultado 
        << "\n" << tab 

        << "resultado_f " 
        << resultado_f 
        << "\n" << tab 
        << "legislacion_f " 
        << legislacion_f 
        << "\n" << tab 
        << "indice " << indice 
        << "";
    };
  };

  static bool sustancias_date_comp( 
      Sustancia x, Sustancia y )
  {
    return x.date < y.date;
  };

  static bool sustancias_indice_comp( 
      Sustancia x, Sustancia y )
  {
    return x.indice < y.indice;
  };

  //static bool sustancia_stats_sum_comp( 
    //SustanciaStats x, SustanciaStats y )
  //{
    //return x.sum < y.sum;
  //};

  typedef std::pair<string, SustanciaStats> sustancia_name_stats_pair;
  struct sustancia_stats_sum_comp
  {
    bool operator()(const sustancia_name_stats_pair& lhs, const sustancia_name_stats_pair& rhs) const
    {
      //compare by stats.sum
      return lhs.second.sum > rhs.second.sum;
    }
  };

  class Efluente
  {
    public:

    string id;

    vector<Sustancia> sustancias;

    void log()
    {
      ofLog() << "efluente id: " << id;

      for (int i = 0; 
          i < sustancias.size(); i++)
        sustancias[ i ].log(); 
    }

  };

  class Planta
  {
    public:

    string id;
    string nombre;
    string dir;
    string lat;
    string lon;

    vector<Efluente> efluentes;
    vector<Sustancia> sustancias;

    void log( string tab = "\t")
    {

      ofLog() << "planta"
        << "\n" << tab 
        << "id " << id
        << "\n" << tab 
        << "nombre " << nombre
        << "\n" << tab 
        << "dir " << dir
        << "\n" << tab 
        << "lat " << lat
        << "\n" << tab 
        << "lon " << lon
        << "\n" << tab 
        << "cant efluentes " 
        << efluentes.size()
        << "";

      for (int i = 0; 
          i < efluentes.size(); i++)
        efluentes[ i ].log(); 

    };

  };

  // stats sustancias totales
  // TODO hacer Stats

  ofxDate& min_date()
  { return _min_date.date; }

  ofxDate& max_date()
  { return _max_date.date; }

  float min_indice()
  { return _min_indice.indice; }

  float max_indice()
  { return _max_indice.indice; }


  vector<Planta>& plantas()
  {
    return _plantas;
  };

  string planta_nombre( string id )
  {
    return _plantas_nombres[ id ];
  };

  map<string,SustanciaStats>& stats_x_sustancia()
  {
    return _stats_x_sustancia;
  };

  vector<sustancia_name_stats_pair>& stats_x_sustancia_sorted()
  {
    return _stats_x_sustancia_sorted;
  };

  vector<Sustancia>& sustancias()
  {
    return _sustancias;
  };

  void log()
  {
    for (int i = 0; 
        i < _plantas.size(); i++)
    {
      _plantas[i].log();
    }
  };

  void load()
  {

    load_json_plantas();

    _plantas = parse_plantas( 
        json_plantas );

    for (int i = 0; 
        i < _plantas.size(); i++)
    {
      Planta& planta = _plantas[i]; 

      load_json_efluentes( planta.id );

      planta.efluentes = 
        parse_efluentes( 
          json_efluentes
          [ planta.id ] );

      //agregar data a otras listas...

      _plantas_nombres
        [ planta.id ] = planta.nombre;

      for (int j = 0; 
        j < planta.efluentes.size(); 
        j++ )
      {
        Efluente& e = 
          planta.efluentes[j]; 

        for (int k = 0; 
            k < e.sustancias.size(); 
            k++ )
        {
          Sustancia& s = 
            e.sustancias[k]; 

          planta.sustancias
            .push_back( s );

          _sustancias
            .push_back( s );

          SustanciaStats& st = 
            _stats_x_sustancia
            [ s.analito ];

          st.indices.push_back(s.indice);

          st.sum = std::accumulate( st.indices.begin(), st.indices.end(), 0.0 );
          
          st.min = *std::min_element( st.indices.begin(), st.indices.end() );

          st.max = *std::max_element( st.indices.begin(), st.indices.end() );

        }
      }

      // sustancias de planta 
      // sort x date
      std::sort( 
          planta.sustancias.begin(),
          planta.sustancias.end(),
          sustancias_date_comp );

    }

    // stats sustancias totales

    // sort x date
    std::sort( 
        _sustancias.begin(),
        _sustancias.end(),
        sustancias_date_comp );

    _min_date = *std::min_element(
        _sustancias.begin(), 
        _sustancias.end(),
        sustancias_date_comp );

    _max_date = *std::max_element(
        _sustancias.begin(), 
        _sustancias.end(),
        sustancias_date_comp );

    _min_indice = *std::min_element(
        _sustancias.begin(), 
        _sustancias.end(),
        sustancias_indice_comp );

    _max_indice = *std::max_element(
        _sustancias.begin(), 
        _sustancias.end(),
        sustancias_indice_comp );

    _stats_x_sustancia_sorted = vector<sustancia_name_stats_pair>( _stats_x_sustancia.begin(), _stats_x_sustancia.end() );
    std::sort( 
        _stats_x_sustancia_sorted.begin(),
        _stats_x_sustancia_sorted.end(),
        sustancia_stats_sum_comp() );

  };  

private:

  // stats sustancias totales
  // TODO hacer Stats
  Sustancia _min_date, _max_date;
  Sustancia _min_indice, _max_indice;

  vector<Planta> _plantas;
  map<string,string> _plantas_nombres; //<id,nombre>

  vector<Sustancia> _sustancias; //sorted x date
  map<string,SustanciaStats> _stats_x_sustancia;
  vector<sustancia_name_stats_pair> _stats_x_sustancia_sorted;
  //map< string,vector<float> > _indices_x_sustancia;

  ofxJSONElement json_plantas;
  map<string,ofxJSONElement> json_efluentes;
  map<string,ofxJSONElement> json_efluentes_data;

  vector<Planta> parse_plantas( 
      ofxJSONElement& json )
  {
    vector<Planta> plantas;

    Json::Value pres = json["result"];

    for (int i = 1; i < pres.size(); i++)
    {
      Planta p;

      p.id = pres[i][0].asString();
      p.nombre = pres[i][1].asString();
      p.dir = pres[i][2].asString();
      p.lat = pres[i][3].asString();
      p.lon = pres[i][4].asString();

      //same as sustancia.id_planta
      utils::str_for_osc( p.nombre );

      plantas.push_back( p );
    }

    return plantas;
  };

  vector<Efluente> parse_efluentes( 
      ofxJSONElement& json )
  {

    vector<Efluente> efluentes;

    Json::Value eres = json["result"];

    for (int i = 1; i < eres.size(); i++)
    { 
      Efluente e;

      //["ID Análisis"
      //,"ID Planta"
      //,"Planta"
      //,"Tipo Muestra"
      //,"Fecha",
      //"Acta"
      //,"Lugar Muestra"
      //,"Procedimiento"
      //,"Metodología"
      //,"Observación"]

      e.id = eres[i][0].asString();

      load_json_efluentes_data( e.id ); 

      e.sustancias = 
        parse_sustancias( 
          json_efluentes_data
          [ e.id ] ); 

      efluentes.push_back( e );

    }

    return efluentes;
  };

  vector<Sustancia> parse_sustancias( 
      ofxJSONElement& json )
  {

    vector<Sustancia> sustancias;

    Json::Value dres = json["result"];

    for (int i = 1; i < dres.size(); i++)
    {

      Sustancia s;

      //["ID Análisis"
      //,"ID Planta"
      //,"Tipo Muestra"
      //,"Fecha"
      //,"Acta"
      //,"Lugar Muestra"
      //,"Procedimiento"
      //,"Metodología"
      //,"Observación"
      //,"Analito"
      //,"Método"
      //,"Detección"
      //,"Legislación"
      //,"Resultado"]

      s.id_efluente = dres[i][0]
        .asString();

      //es el nombre!
      s.id_planta = dres[i][1]
        .asString();

      s.fecha = dres[i][3]
        .asString();

      s.analito = dres[i][9]
        .asString();

      s.metodo = dres[i][10]
        .asString();

      s.deteccion = dres[i][11]
        .asString();

      s.legislacion = dres[i][12]
        .asString();

      s.resultado = dres[i][13]
        .asString();

      s.resultado_f = parse_float( 
          s.resultado );

      s.legislacion_f = parse_float( 
          s.legislacion );

      s.indice = s.resultado_f / s.legislacion_f;

      s.indice = utils::is_num(
          s.indice ) ? s.indice : 0;

      utils::str_for_osc( s.analito );
      utils::str_for_osc( s.id_planta );

      utils::to_date( s.fecha, s.date );

      if ( s.indice != 0 )
        sustancias.push_back( s );

    }

    return sustancias;

  };

  static float parse_float( string str )
  {
    ofStringReplace( str, ",", "." );

    ofxRegex re;
    vector<string> res = re.getMatchedStrings(str,"[^\\.0123456789]");

    for (int i = 0; i < res.size(); i++)
      ofStringReplace( str, res[i], "");

    return ofToFloat( str );
  }; 

  void load_json_plantas()
  {

    string url = 
      "plantas/plantas.json";

    if ( ! json_plantas
        .open( url ) )
      ofLogError() 
        << "failed to get plantas";

  };

  void load_json_efluentes(
      string id_planta )
  {

    string url = 
        "plantas/efluentes_" 
        + id_planta 
        + ".json";

      if ( ! json_efluentes
          [ id_planta ]
          .open( url ) )
        ofLogError() 
          << "failed to get efluentes"
          << " de planta " << id_planta;

  };

  void load_json_efluentes_data( 
      string id_efluente )
  {

    string url =
      "plantas/efluentes_data_" 
      + id_efluente 
      + ".json";

    if ( ! json_efluentes_data
        [ id_efluente ]
        .open( url ) )
      ofLogError() 
        << "failed to get data"
        << "de efluente " 
        << id_efluente;

  };  

};

