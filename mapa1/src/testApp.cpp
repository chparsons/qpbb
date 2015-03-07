#include "testApp.h"

enum vmode_t {
  vmode_circulos = 0, 
  vmode_lineas, 
  vmode_length
};

enum rmode_t {
  rmode_normal = 0, 
  rmode_mapeado, 
  rmode_length
};

int npts = 4;
int curpt = 0;
int step1 = 1;
int step2 = 10;

//ofVec2f m_startdrag;
//ofVec2f tex_t;
//ofVec2f tex_t_startdrag;
//float tex_s;
//float tex_s_startdrag;

string mapfile = "qpbb_1024x768_-62.3088,-38.7948,-62.2414,-38.7554";
//string mapfile = "qpbb_1024x768_-62.3368,-38.8013,-62.2199,-38.7329";

int tex_w, tex_h; 
float O,S,E,N; 

int width, height;

//float rref;
//float rpow;


void testApp::setup()
{

  ofLog() 
    << "renderer: "
    << ofGetCurrentRenderer()->getType()
    << ", screen: "
    << ofGetScreenWidth()
    << " x "
    << ofGetScreenHeight();

  ofSetFrameRate(60);

  draw_params = false;
  draw_map = true;

  vector<string> res = ofSplitString( ofSplitString( mapfile, "_")[1], "x" );
  tex_w = ofToInt( res[0] ); 
  tex_h = ofToInt( res[1] ); 

  vector<string> bounds = ofSplitString( ofSplitString( mapfile, "_")[2], "," );
  O = ofToFloat( bounds[0] );
  S = ofToFloat( bounds[1] );
  E = ofToFloat( bounds[2] );
  N = ofToFloat( bounds[3] );

  width = ofGetWidth();
  height = ofGetHeight();

  for (int i = 0; i < 512; i++) 
		pressed[i] = false;

  //reset_tex_ts();

  mapimg.loadImage( mapfile + ".png" );

  ofFbo::Settings s;
  s.width = width;
  s.height = height;
  s.numSamples = 8;
  //s.numColorbuffers = 1;
  //s.internalformat = GL_RGBA;
  fbo.allocate( s );

  fbo_clear();

  ktex.setTexture( 
    fbo.getTextureReference(),
    //mapimg.getTextureReference(),
    false );

  load_ktex();
  //reset_ktex();
  //center_ktex(); 

  //for ( int i = 0; i < npts; i++ )
  //{
    //ofPoint p = ktex.getPoint( i );
    //src[i] = p;
    //dst[i] = p;
    //findHomography(src, dst, H);
  //}

  data.load();
  //data.log();

  //vector<Data::Planta>& plantas = 
    //data.plantas();

  mercator.setup( 
      width, height, 
      O,S,E,N ); 

  //log_sustancias();

  //vector<Data::Sustancia>& sustancias =
    //data.sustancias();

  //for (int i = 0; 
      //i < sustancias.size(); 
      //i++ )
  //{
    //ofxDate f;
    //utils::to_date(
      //sustancias[i].fecha, f );
  //}

  lineas.setMode( OF_PRIMITIVE_LINES );

  params.setup( data, false );
	
}

void testApp::update()
{
  ofSetWindowTitle( ofToString( ofGetFrameRate(), 2 ) );

  params.update( data, timeline );

}


void testApp::draw()
{ 

  ofBackground(0);

  ofEnableSmoothing();
  ofSetCircleResolution( 40 );

  ofSetColor( 255 ); 
  ofFill();

  //render to texture

  fbo.bind();
  {

    ofBackground(255);
    if ( draw_map )
      mapimg.draw( 0, 0, width, height );

    render_sustancias(); 
 
  }
  fbo.unbind();

  ktex.setTexture( 
    fbo.getTextureReference(),
    //mapimg.getTextureReference(), 
    false );

  
  //render to screen

  ofSetColor( 255 ); 
  ofPushMatrix();
  {
    //ofTranslate( tex_t.x, tex_t.y );
    //ofScale( tex_s, tex_s );
    ofTranslate( 0, height );
    ofScale( 1, -1 );
    ktex.draw();
    //glMultMatrixf( H );
    //fbo.draw(0,0);

    ofSetColor( 255,0,0 );
    ofCircle(
      ktex.getPoint(curpt), 
      10 );
  }
  ofPopMatrix();

  if ( draw_params )
    params.draw();

}

bool testApp::do_render_sustancia(
    const Data::Sustancia& s )
{

  ofxDate sdate = s.date;
  float t = timeline.date_to_t(
      sdate,
      data.min_date(),
      data.max_date() );
  float tmin = params.timeline - params.timerange * 0.5;
  float tmax = params.timeline + params.timerange * 0.5;
  bool in_range = t > tmin && t < tmax;

  // está visible por gui
  return params.sustancias_visibles
    [s.analito] 
  // tiene indice
    && s.indice != 0
  // esta dentro del timeline
    && in_range;
}

void testApp::render_sustancias()
{

  //rref = 8.0;
  //rpow = 0.65;

  //vector<string> colors;
  //vector<float> colors_thres;
  //vector<float> rmins;
  //vector<float> rmaxs;

  //colors.push_back("0xfecc5c");
  //colors_thres.push_back(10);
  //rmins.push_back(params.rmin);
  //rmaxs.push_back(params.rmax);
  //colors.push_back("0xfd8d3c");
  //colors_thres.push_back(100);
  //rmins.push_back(params.rmin);
  //rmaxs.push_back(params.rmax * 1.2);
  //colors.push_back("0xe31a1c");
  //colors_thres.push_back(1000);
  //rmins.push_back(params.rmin);
  //rmaxs.push_back(params.rmax * 1.4);
  //colors.push_back("0xe31a1c");
  //colors_thres.push_back(10000);
  //rmins.push_back(params.rmin);
  //rmaxs.push_back(params.rmax * 1.4);
  //colors.push_back("0xe31a1c");
  //colors_thres.push_back(10000);
  //rmins.push_back(params.rmin);
  //rmaxs.push_back(params.rmax * 1.4);


  if ( params.vmode == vmode_lineas )
    lineas.clear();


  vector<Data::Planta>& plantas = 
    data.plantas();

  for (int i = 0; i < plantas.size(); i++)
  { 

    Data::Planta& planta = plantas[ i ];

    ofPoint p = mercator
      .getScreenLocationFromLatLon(
        ofToFloat( planta.lat ), 
        ofToFloat( planta.lon ) ); 

    int slen = planta.sustancias.size(); 
    //int slent=data.sustancias().size(); 

    vector<Data::Sustancia> 
      planta_sustancias_visibles;
    for ( int j = 0; j < slen; j++ )
    {
      Data::Sustancia& s = 
        planta.sustancias[ j ];

      if ( ! do_render_sustancia(s) ) 
        continue;

      planta_sustancias_visibles.push_back(s);
    }
    int slen_vis = planta_sustancias_visibles.size(); 

    float _alfa = ofMap( 1.0/slen_vis,
        0.0, 1.0, params.alfa_min, 1.0 );
    //float _alfa_pos=ofMap(1.0/slen_vis,
    //0.0, 1.0, params.alfa_min_pos, 1.0);

    for ( int j = 0; j < slen_vis; j++ )
    {

      Data::Sustancia& s = 
        //planta.sustancias[ j ];
        planta_sustancias_visibles[ j ];

      //Data::SustanciaStats& st = data.stats_x_sustancia()[ s.analito ];

      //if ( ! params.sustancias_visibles
          //[ s.analito ] )
        //continue;

      ////if ( s.indice > params.iumbral )
        ////continue;

      //if ( s.indice == 0 )
        //continue; 

      float r = radio(s); 

      switch ( params.vmode )
      {
        case vmode_circulos:
          render_circulo(
              j, slen_vis, p, r, _alfa );
          break;

        case vmode_lineas:
          render_linea( 
              j, slen_vis, p, r, _alfa );
          break;

        default:
          render_circulo(
              j, slen_vis, p, r, _alfa );
          break;
      }

    }

    //ref legal
    //ofNoFill();
    //ofSetLineWidth( 0.7 );
    //ofSetColorf( 1, 1, 0, 1 );
    //ofCircle( p.x, p.y, rref );

  }

  if ( params.vmode == vmode_lineas )
    lineas.draw();

}

void testApp::render_circulo(
    int j, int slen,
    const ofPoint& p, 
    float r, float _alfa )
{
  //ofFloatColor color = 
    //ofFloatColor::fromHex(
        //ofHexToInt( ihex ), _alfa );

  ofFloatColor color( 1,0,0, _alfa );
  //ofFloatColor color( 1,0,0, 
      //s.indice > params.iumbral
      //? _alfa_pos : _alfa );

  ofSetColor( color );
  //ofFill(); 
  //ofCircle( p.x, p.y, r );
  ofNoFill();
  ofSetLineWidth( 0.001 );
  ofCircle( p.x, p.y, r );

  //ref legal
  //cada sustancia medida tiene 
  //un radio de ref legal diferente
  //ofSetColor( 200,200,0, 180 );
  //ofCircle( p.x, p.y, rref );
}

void testApp::render_linea(
    int j, int slen,
    const ofPoint& p, 
    float r, float _alfa )
{

  float theta = (float)j / slen * TWO_PI;
  float x = r * cos(theta);
  float y = r * sin(theta);
  float xo = params.rmin * cos(theta);
  float yo = params.rmin * sin(theta);

  //string ihex;
  //float ithres;
  //int clen = colors.size();
  //for ( int c = 0; c < clen; c++ )
  //{
    ////10,100,1000,10000,...
    //ithres = pow(10, (float)(c+1));
    //if ( s.indice < ithres 
        //|| c == clen-1 )
    //{
      //ihex = colors[c];
      //break;
    //}
  //}

  //ofLog() 
    //<< s.analito
    //<< ", indice: " << s.indice
    //<< ", ithres: " << ithres
    //<< ", ihex: " << ihex;

  //string ihex = colors
      //[ (j%(colors.size()-1)) ];
  //ofFloatColor color = 
    //ofFloatColor::fromHex(
        //ofHexToInt( ihex ), _alfa );

  ofFloatColor color( 1,0,0, _alfa );
  //ofFloatColor color( 1,0,0, 
      //s.indice > params.iumbral
      //? _alfa_pos : _alfa );

  //ofSetColor( color );
  ofSetLineWidth( 4 );

  ofVec3f src(p.x+xo, p.y+yo, 0);
  ofVec3f dst(p.x+x, p.y+y, 0);

  lineas.addVertex( src );
  lineas.addColor( color );

  lineas.addVertex( dst );
  lineas.addColor( color ); 

}

float testApp::radio(
    Data::Sustancia& s )
{
  switch( params.rmode )
  {
  case rmode_mapeado:
    return radio_mapeado_x_sustancia(s); 
    break;

  case rmode_normal:
    return radio_normal(s);
    break;

  default:
    return radio_normal(s);
    break;
  }
}

float testApp::radio_normal(
    Data::Sustancia& s )
{
  return ofMap( s.indice, 
      0, params.rindice * params.rindice_mult, 
      params.rmin, params.rmax );

  //float r;

  //string ihex;
  //float ithres;
  //int ic;
  //for (int c = 0;c < colors.size(); c++)
  //{
    ////100,1000,10000,...
    ////ithres = pow(10, (float)(c+2));
    //ithres = colors_thres[c];
    //if ( s.indice < ithres )
    //{
      //ihex = colors[c];
      //ic = c;
      //break;
    //}
  //}

  //r = ofMap( s.indice, 
      //0, ithres, 
      //rmins[ic], rmaxs[ic] );

  //r = s.indice*rref*(100.0/ithres);
  //r = s.indice * rref;
  //r = pow(s.indice,rpow)*rref;
}

float testApp::radio_mapeado_x_sustancia(
    Data::Sustancia& s )
{

  Data::SustanciaStats& st = data.stats_x_sustancia()[ s.analito ];

  float r;
  //float _rref;

  if ( ! utils::equals(st.min,st.max) )
  {
    //_rref = ofMap( 1, 
        //st.min, st.max, params.rmin, params.rmax );
    r = ofMap( s.indice, 
        st.min, st.max, 
        params.rmin, params.rmax );
  }
  else
  {
    //_rref = params.rmin;
    r = s.indice * params.rmin;
  }

  return r;
}

void testApp::log_sustancias()
{
  map<string,Data::SustanciaStats> stats = data.stats_x_sustancia();

  vector<Data::Sustancia>& sustancias = data.sustancias();
  int slen = sustancias.size(); 

  for ( int i = 0; i < slen; i++ )
  {
    Data::Sustancia& s = sustancias[i];

  //for ( map<string,Data::SustanciaStats>::iterator it = stats.begin(); it != stats.end(); ++it )
  //{

    //string analito = it->first;
    //Data::SustanciaStats st=it->second;

    string analito = s.analito;
    Data::SustanciaStats& st = stats
      [ analito ];

    ofLog() 
      << analito 
      << " [" << st.min 
      << "," << st.max << "]"
      << ", sum: " << st.sum
      << ", indice: " << s.indice
      << ", date: " << s.date.getIsoDate()
      << ", fecha: " << s.fecha
      << "";

  }

}

void testApp::fbo_clear()
{
  fbo.begin();
  ofClear(0,0,0,255);
  fbo.end();
}

void testApp::exit() 
{
  ofLog() << "exit";

  mapimg.clear();
  params.exit();

}

void testApp::keyPressed(int key)
{

  pressed[ key ] = true;

  switch( key )
  {

    case key::projector:
      ofSetWindowPosition( ofGetWindowPositionX() == 0 ? ofGetScreenWidth()+1 : 0, 0 );
      break;

    case key::fullscreen:
      ofToggleFullscreen();
      break;

    case key::draw_params:
      draw_params = !draw_params;
      break;

    case key::draw_map:
      draw_map = !draw_map;
      fbo_clear();
      break;

    //case key::reset_tex_ts:
      //reset_tex_ts();
      //break;

    case key::reset_ktex:
      reset_ktex();
      break;

    case key::save_ktex:
      save_ktex();
      break;

    case key::load_ktex:
      load_ktex();
      break;

    case OF_KEY_RETURN:
      curpt = (curpt+1)%4;
      break;

    case OF_KEY_LEFT:
      moveLeft();
      break;

    case OF_KEY_RIGHT:
      moveRight();
      break;

    case OF_KEY_UP:
      moveDown();
      break;

    case OF_KEY_DOWN:
      moveUp();
      break;
 
  }

}


void testApp::keyReleased(int key)
{
  pressed[key] = false; 
}


void testApp::mouseMoved(int x, int y ){

}


void testApp::mouseDragged(int x, int y, int button)
{

  //ofVec2f d = ofVec2f(x,y)-m_startdrag;
  //if ( pressed['z'] )
  //{
    //tex_s = tex_s_startdrag-d.y*0.001;
  //}
  //else
  //{
    //tex_t.set( tex_t_startdrag + d );
  //}

}


void testApp::mousePressed(int x, int y, int button)
{

  //int n;
  //if ( pressed[ '1' ] ) n = 0;
  //if ( pressed[ '2' ] ) n = 1;
  //if ( pressed[ '3' ] ) n = 2;
  //if ( pressed[ '4' ] ) n = 3;

  //if ( pressed[ key::set_H_src ] )
  //{
    //src[n] = ofPoint(x,y);
    ////findHomography(src, dst, H);
    //ofLog() << "set H src point " 
      //<< ofToString(n) << " = " 
      //<< ofToString(x) << "," 
      //<< ofToString(y);
  //}

  //else if ( pressed[ key::set_H_dst ] )
  //{
    //dst[n] = ofPoint(x,y);
    ////findHomography(src, dst, H);
    //ofLog() << "set H dst point " 
      //<< ofToString(n) << " = " 
      //<< ofToString(x) << "," 
      //<< ofToString(y);
  //}

  //tex_t_startdrag.set( tex_t );
  //tex_s_startdrag = tex_s;
  //m_startdrag.set( ofVec2f(x,y) );
}


void testApp::mouseReleased(int x, int y, int button)
{
}


void testApp::windowResized(int w, int h){

}


void testApp::gotMessage(ofMessage msg){

}


void testApp::dragEvent(ofDragInfo dragInfo)
{ 

}

//void testApp::reset_tex_ts()
//{
  //tex_t.set(width/2, height/2);
  //tex_s = 1;
//}

void testApp::load_ktex()
{

  ktex_cfg.loadFile("keystone.xml");
  ktex_cfg.pushTag("keystone");

  //int npts = ktex_cfg.getNumTags("keystone:pt");

  ofLog() << "load ktex";

  for ( int i = 0; i < npts; i++ )
  {

    int x = ktex_cfg
      .getValue("pt:x",0,i);
    int y = ktex_cfg
      .getValue("pt:y",0,i);

    ofLog() << "\t " << i
      << ": " << x << ", " << y;

    ktex.setPoint( i, x, y );
  }

}

void testApp::save_ktex()
{
  ofLog() << "save ktex";

  for ( int i = 0; i < npts; i++ )
  {
    ofPoint p = ktex.getPoint( i );

    ofLog() << "\t " << i
      << ": " << p.x << ", " << p.y;

    ktex_cfg
      .setValue("pt:x",p.x,i);
    ktex_cfg
      .setValue("pt:y",p.y,i);
  }

  ktex_cfg.saveFile("keystone.xml");
}

void testApp::reset_ktex()
{
  int o = 0;
  int w = width -o;
  int h = height -o;

  ktex.setPoint(0, o, o);
  ktex.setPoint(1, w, o);
  ktex.setPoint(2, w, h);
  ktex.setPoint(3, o, h);

  //int o = 0;
  //ktex.setPoint(0, o, o);
  //ktex.setPoint(1, w, o);
  //ktex.setPoint(2, w, h);
  //ktex.setPoint(3, o, h);

  //ktex.setPoint(0, -w, -h);
  //ktex.setPoint(1, w, -h);
  //ktex.setPoint(2, w, h);
  //ktex.setPoint(3, -w, h);
}

//void testApp::center_ktex()
//{
  //int w = tex_w/2;
  //int h = tex_h/2;

  //ofPoint c;

  //c = ktex.getPoint(0);
  //ktex.setPoint(0, c.x+w, c.y+h);

  //c = ktex.getPoint(1);
  //ktex.setPoint(1, c.x+w, c.y+h);

  //c = ktex.getPoint(2);
  //ktex.setPoint(2, c.x+w, c.y+h);

  //c = ktex.getPoint(3);
  //ktex.setPoint(3, c.x+w, c.y+h);
//}

void testApp::moveLeft()
{
  int _step = pressed[OF_KEY_LEFT_SHIFT] ? step2 : step1;
  ofPoint c = ktex.getPoint(curpt);
  ktex.setPoint(curpt, c.x-_step, c.y);
}

void testApp::moveRight()
{
  int _step = pressed[OF_KEY_LEFT_SHIFT] ? step2 : step1;
  ofPoint c = ktex.getPoint(curpt);
  ktex.setPoint(curpt, c.x+_step, c.y);
}

void testApp::moveUp()
{
  int _step = pressed[OF_KEY_LEFT_SHIFT] ? step2 : step1;
  ofPoint c = ktex.getPoint(curpt);
  ktex.setPoint(curpt, c.x, c.y-_step);
}

void testApp::moveDown()
{
  int _step = pressed[OF_KEY_LEFT_SHIFT] ? step2 : step1;
  ofPoint c = ktex.getPoint(curpt);
  ktex.setPoint(curpt, c.x, c.y+_step);
}

