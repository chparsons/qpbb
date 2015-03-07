#include "testApp.h"

struct JSONData
{

  ofxJSONElement plantas;
  map<string,ofxJSONElement> efluentes;
  map<string,ofxJSONElement> efluentes_data;
  map<string,ofxJSONElement>::iterator it;

  void load_plantas()
  {
    string endpoint = "PLANT-DE-EFLUE-LIQUI";

    string src = make_url( endpoint );

    ofxJSONElement& json = plantas;

    if ( ! json.open( src ) )
      ofLogError() << "failed to get plantas";
  };

  void load_efluentes( string planta_id )
  {
    string endpoint = "ANALI-DE-EFLUE-LIQUI";
    string args = "&pArgument1=1900-01-01&pArgument2=2100-01-01&pArgument0="+planta_id;

    string src = make_url(endpoint, args);

    ofxJSONElement& json = get_efluentes( planta_id );

    if ( !json.open( src ) )
      ofLogError() << "failed to get efluentes de planta " << planta_id;
  };

  ofxJSONElement& get_efluentes( string planta_id )
  {
    return efluentes[ planta_id ];
  };

  void load_efluentes_data( string efluentes_id )
  {
    string endpoint = "DETAL-ANALI-EFLUE-LIQUI";
    string args = "&pArgument0=" + efluentes_id;

    string src = make_url(endpoint, args);

    ofxJSONElement& json = get_efluentes_data( efluentes_id );

    if ( !json.open( src ) )
      ofLogError() << "failed to get data de efluente " << efluentes_id;
  };

  ofxJSONElement& get_efluentes_data( string efluentes_id )
  {
    return efluentes_data[ efluentes_id ];
  };

  string make_url( string endpoint, string args = "" )
  {
    string baseurl = "http://bahiablanca.cloudapi.junar.com/datastreams/invoke/";
    string key = "ee47e8ecdc5adc1a3f1bd3d902b53b6f1acf6e06";
    return baseurl + endpoint + "?output=json_array&auth_key=" + key + args;
  };

  void save()
  {
    plantas.save(
        ofToDataPath("plantas.json") );

    for ( it = efluentes.begin(); it != efluentes.end(); ++it )
    {
      it->second.save( ofToDataPath(
            "efluentes_"+ it->first +".json") );
    }

    for ( it = efluentes_data.begin(); it != efluentes_data.end(); ++it )
    {
      it->second.save( ofToDataPath(
            "efluentes_data_"+ it->first +".json") );
    }
  };

} json_data;

struct Efluentes
{
  string id;
  string planta_id;
  string fecha;
  string analito;
  string metodo;
  string deteccion;
  string legislacion;
  string resultado;

  void log()
  {
    ofLogVerbose() << "efluente:"
      << "\n\t" << "id " << id
      << "\n\t" << "planta_id " << planta_id
      << "\n\t" << "fecha " << fecha
      << "\n\t" << "analito " << analito
      << "\n\t" << "deteccion " << deteccion
      << "\n\t" << "legislacion " <<legislacion 
      << "\n\t" << "resultado " << resultado; 
  };
};

struct Planta
{
  string id;
  string nombre;
  string dir;
  string lat;
  string lng;

  vector<Efluentes> efluentes;
};

vector<Planta> parse_plantas( ofxJSONElement& json )
{
  vector<Planta> plantas;

  Json::Value res = json["result"];

  for ( int i = 1; i < res.size(); i++ )
  {
    Planta p;
    p.id = res[i][0].asString();
    p.nombre = res[i][1].asString();
    p.dir = res[i][2].asString();
    p.lat = res[i][3].asString();
    p.lng = res[i][4].asString();
    plantas.push_back( p );
  }

  return plantas;
};

vector<Efluentes> parse_efluentes( ofxJSONElement& json )
{
  vector<Efluentes> efluentes;
  
  Json::Value res = json["result"];

  for ( int i = 1; i < res.size(); i++ )
  {
    Efluentes e;

    e.id = res[i][0].asString();
    e.planta_id = res[i][1].asString();
    e.fecha = res[i][4].asString();

    ofLogVerbose() 
        << "loading data de efluentes " << e.id;

    json_data.load_efluentes_data( e.id );

    Json::Value dres = json_data.get_efluentes_data(e.id)["result"];

    for ( int j = 1; j < dres.size(); j++ )
    {
      e.analito = dres[j][9].asString();
      e.metodo = dres[j][10].asString();
      e.deteccion = dres[j][11].asString();
      e.legislacion = dres[j][12].asString();
      e.resultado = dres[j][13].asString();
    }

    efluentes.push_back( e );
  }

  return efluentes;
};


void testApp::setup()
{

	ofSetLogLevel( OF_LOG_VERBOSE );

  ofLogVerbose() << "loading plantas...";

  json_data.load_plantas();

  vector<Planta> plantas = parse_plantas(
      json_data.plantas );

  for ( int i = 0; i < plantas.size(); i++ )
  {
    Planta p = plantas[i];

    ofLogVerbose() 
      << "loading efluentes de planta " << p.id;

    json_data.load_efluentes( p.id );

    p.efluentes = parse_efluentes( 
        json_data.get_efluentes( p.id ) );

    for (int j = 0; j < p.efluentes.size(); j++)
    {
      Efluentes e = p.efluentes[j]; 
      //e.log();
    }
  }

  json_data.save();

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
