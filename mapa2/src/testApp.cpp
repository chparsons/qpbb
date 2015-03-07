#include "testApp.h"

enum vmode_t {
  vmode_circulos = 0, 
  vmode_props, 
  vmode_lineas, 
  vmode_length
};

//enum imode_t {
  //imode_lin = 0, 
  //imode_exp, 
  //imode_map, 
  //imode_length
//};

int npts = 4;
int curpt = 0;
int step1 = 1;
int step2 = 10;

//ofVec2f m_startdrag;
//ofVec2f tex_t;
//ofVec2f tex_t_startdrag;
//float tex_s;
//float tex_s_startdrag;

string mapfile = "qpbb_1024x768_BB-BLANCO-NEGRO-1";
//string mapfile = "qpbb_1024x768_-62.3088,-38.7948,-62.2414,-38.7554";
////string mapfile = "qpbb_1024x768_-62.3368,-38.8013,-62.2199,-38.7329";
////string mapfile = "qpbb_2000x1500_-62.3291,-38.8037,-62.2233,-38.7419";

int tex_w, tex_h; 
float O,S,E,N; 

int width, height;


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
  draw_charts = false;
  draw_map = true;
  calib_ktex = false;

  vector<string> res = ofSplitString( ofSplitString( mapfile, "_")[1], "x" );
  tex_w = ofToInt( res[0] ); 
  tex_h = ofToInt( res[1] ); 

  //vector<string> bounds = ofSplitString( ofSplitString( mapfile, "_")[2], "," );
  //O = ofToFloat( bounds[0] );
  //S = ofToFloat( bounds[1] );
  //E = ofToFloat( bounds[2] );
  //N = ofToFloat( bounds[3] );

  //width = tex_w;
  //height = tex_h;
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

  reset_ktex();
  warper.setup();
  load_ktex();
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

  //coords = new CoordsMercator();
  //((CoordsMercator*)coords)->init(width,height,O,S,E,N);
  coords = new CoordsHardcoded();

  //log_sustancias();
  //log_plantas_coords();

  //vector<Data::Sustancia>& sustancias =
    //data.sustancias();

  //for ( int i = 0; 
      //i < sustancias.size(); 
      //i++ )
  //{
    //ofxDate f;
    //utils::to_date(
      //sustancias[i].fecha, f );
  //}

  chart_timeline.init( 300, ofGetHeight()-20, 225, 90 );
  chart_parallelcoords.init( 10, ofGetHeight()-20, 225, 90 );

  mesh_lineas.setMode( OF_PRIMITIVE_LINES );
  mesh_circulos.setMode( OF_PRIMITIVE_LINES );

  params.setup( data, false );
  int m = 10;
  params.gui_info.setPosition( m, params.gui.getHeight() + m*4 );
  params.gui_sustancias.setPosition( ofGetWidth() - params.gui_sustancias.getWidth() - m, m );
  params.gui_plantas.setPosition( params.gui_sustancias.getPosition().x - params.gui_sustancias.getWidth() - m, m );


  // init colores
  vector<string> _cols = colorbrewer.get( "Spectral", 11 ); 
  _cols.erase( _cols.begin() + 4, _cols.begin() + 6 );
  map<string,Data::SustanciaStats> stats = data.stats_x_sustancia();
  int i = 0;
  int len = _cols.size();
  for ( map<string,Data::SustanciaStats>::iterator it = stats.begin(); it != stats.end(); ++it )
  {
    ofFloatColor c = ofFloatColor::fromHex( ofHexToInt( _cols[ (i%len) ] ) );
    //float off = floor((float(i)/len));
    //c.setHue(c.getHue() + off * 0.02);
    //ofLog() << i << ":" << (i%len) << ", " << c.getHue() << ", " << off;
    sustancias_colores[ it->first ] = c;
    i++;
  }

  //for ( map<string,ofFloatColor>::iterator it = sustancias_colores.begin(); it != sustancias_colores.end(); ++it )
  //{
    //ofLog() 
      //<< it->first 
      //<< ": " 
      //<< it->second;
  //}

}


void testApp::update()
{
  ofSetWindowTitle( ofToString( ofGetFrameRate(), 2 ) );

  params.update( data );

}


void testApp::draw()
{ 

  ofBackground(0); 

  ofEnableSmoothing();
  ofSetCircleResolution( 40 );

  ofSetColor( 255 ); 
  ofFill();


  if ( render_calib() )
    return;


  //render to texture

  fbo.bind();
  {

    ofBackground(255);
    if ( draw_map ) 
      mapimg.draw( 
          0, 0, width, height );

    render(); 

  }
  fbo.unbind();


  //render to screen

  H = warper.getMatrix();

  ofSetColor( 255 ); 
  ofPushMatrix();
  {
    ofMultMatrix( H.getPtr() );
    //ofTranslate( 0, height );
    //ofScale(1,-1,1);
    //fbo.draw(0,0,width,height);
    fbo.draw(0, height, width, -height);
  }
  ofPopMatrix(); 

  if ( calib_ktex )
  {
    ofSetColor( ofColor::magenta );
    warper.draw();
  }

  if ( draw_params )
  {
    params.draw();
  }

  if ( draw_charts )
  {
    chart_timeline.render(params, data); 
    chart_parallelcoords.render( params, data ); 
  }

}

bool testApp::filter_sustancia(
    Data::Sustancia& s )
{

  float t = fns::date_to_t( s, data );
  bool in_range = fns::in_range( t, params );

  // no está visible por gui 
  // en lista de sustancias visibles
  return !params.sustancias_visibles
    [ s.analito ] 
  // no está visible por gui 
  // en lista de plantas visibles
    || !params.plantas_visibles
    [ s.id_planta ]
  // no tiene indice
    || s.indice == 0
  // no esta dentro del timeline
    || !in_range;
}

void testApp::render()
{  

  // render: clear
  switch ( params.vmode )
  {
    case vmode_circulos:
      mesh_circulos.clear();
      break;
    case vmode_lineas:
      mesh_lineas.clear();
      break;
  }

  // render: batch

  vector<Data::Planta>& plantas = 
    data.plantas();

  for ( int i = 0; 
      i < plantas.size(); 
      i++ )
  { 

    Data::Planta& planta = plantas[ i ];

    ofPoint p = coords->
      get_screen_loc( planta );

    vector<Data::Sustancia*> sust_vis;
    int slen = planta.sustancias.size(); 

    for ( int j = 0; j < slen; j++ )
    {
      Data::Sustancia& s = 
        planta.sustancias[ j ];

      if ( filter_sustancia(s) ) 
        continue;

      sust_vis.push_back( &s );
    }

    render_planta_props( p, sust_vis );
    if ( params.vmode == vmode_props )
    {
      //ofLog() << planta.nombre;
      render_planta_props( p, sust_vis );
    }
    else
    {
      render_sustancias( p, sust_vis );
    }

    //ref legal
    //ofNoFill();
    //ofSetLineWidth( 0.7 );
    //ofSetColorf( 1, 1, 0, 1 );
    //ofCircle( p.x, p.y, rref );

  }

  // render: send to gpu
  switch ( params.vmode )
  {
    case vmode_circulos:
      mesh_circulos.draw();
      break;
    case vmode_lineas:
      mesh_lineas.draw();
      break;
  }

}

void testApp::render_sustancias(
    const ofPoint& p,
    vector<Data::Sustancia*>& sust_vis )
{

  int slen_vis = sust_vis.size(); 

  float alfa = ofMap( 1.0/slen_vis,
      0.0, 1.0, params.alfa_min, 1.0 );
  //float _alfa_pos=ofMap(1.0/slen_vis,
  //0.0, 1.0,params.alfa_min_pos, 1.0);

  for ( int j = 0; j < slen_vis; j++ )
  {
    Data::Sustancia s = *(sust_vis[j]);

    ofFloatColor color;
    float hue, stroke;

    float r = params.rmin + fns::indice( s.indice, data, params ); 

    // render: add vertices
    switch ( params.vmode )
    {
      case vmode_circulos:

        hue = 0.; 
        //float hue = ofMap(s.indice, 
          //0., data.max_indice(), 
          ////naranja -> rojo
          //0.02, 0.0 );
        color = ofFloatColor::fromHsb( hue, 1, 1, alfa );
        //color = sustancias_colores
          //[ s.analito ];
        //color[3] = alfa;

        //stroke = 0.001; 
        stroke = ofMap(s.indice, 
            0., data.max_indice(), 
            0.001, 2.2 );

        ofSetLineWidth( stroke );

        render_circulo(
            j, slen_vis, r, p, color );

        break;

      case vmode_lineas:

        color = sustancias_colores
          [ s.analito ];

        ofSetLineWidth( 4 );

        render_linea( 
            j, slen_vis, r, p, color );

        break;
    }

  }
}

// calc proporciones de sust visibles

void testApp::render_planta_props(
    const ofPoint& p,
    vector<Data::Sustancia*>& sust_vis )
{

  map<string,float> indices_x_sust;
  float indices_sum_planta = 0;

  int slen_vis = sust_vis.size(); 

  for ( int j = 0; j < slen_vis; j++ )
    indices_x_sust
      [ sust_vis[j]->analito ] = 0;

  for ( int j = 0; j < slen_vis; j++ )
  {
    Data::Sustancia s = *(sust_vis[j]);
    float _indice = s.indice;
    //float _indice = fns::indice( s.indice, data, params );
    indices_sum_planta += _indice;
    indices_x_sust[s.analito] += _indice;
  }

  float theta0 = 0;
  float theta1 = 0;
  int i = 0;
  int sust_x_planta_len = indices_x_sust.size();
  for ( map<string,float>::iterator it = indices_x_sust.begin(); it != indices_x_sust.end(); ++it )
  {
    i++;

    string analito = it->first;
    float indices_sum_sust = it->second;

    float prop = indices_sum_sust/indices_sum_planta;

    //radianes
    //theta1 += TWO_PI * prop;
    //grados
    theta1 += 360. * prop;

    //float hue = ((float)i)/sust_x_planta_len;
    //ofFloatColor color = ofFloatColor::fromHsb( hue, .7, 8., 1. );

    ofFloatColor color = sustancias_colores[ analito ];

    //ofLog()
      //<< analito
      //<< ", "
      //<< color
      //<< ", theta0: "
      //<< theta0
      //<< ", theta1: "
      //<< theta1
      //<< ", indices_sum_planta: "
      //<< indices_sum_planta
      //<< ", indices_sum_sust: "
      //<< indices_sum_sust
      //<< ", prop: "
      //<< prop
      //<< ", hue: "
      //<< hue
      //<< "";

    float stroke = 3;

    ofSetLineWidth( stroke );
    ofSetColor( color );

    //float r = 20;
    float r = params.rmin;
    float cres = 60;
    ofPolyline pline;
    pline.arc( p, r, r, 
        theta0, theta1, true, cres );
    pline.draw();

    theta0 = theta1;
  }

}

void testApp::render_circulo(
    int j, int slen, float r, 
    const ofPoint& p, 
    const ofFloatColor& color ) 
{ 

  //no renderear circ muy grandes
  if ( r > ofGetWidth()/1.6 )
    return;

  //ofSetColor( color );
  //ofNoFill();
  //ofCircle( p.x, p.y, r );

  //int LOD = ofMap( r, 
      //50, 1000, 30, 80, true );
  int LOD = 30;

  float theta = TWO_PI;
  float res = TWO_PI / LOD;
  float x0, y0, x1, y1;
  ofVec3f src, dst;

  x0 = r * cos(theta);
  y0 = r * sin(theta);
  while ( theta > res )
  {
    theta -= res;

    x1 = r * cos(theta);
    y1 = r * sin(theta);

    src.set( p.x+x0, p.y+y0 );
    dst.set( p.x+x1, p.y+y1 );

    mesh_circulos.addVertex( src );
    mesh_circulos.addColor( color );
    mesh_circulos.addVertex( dst );
    mesh_circulos.addColor( color );

    x0 = x1;
    y0 = y1;
  }

}

void testApp::render_linea(
    int j, int slen, float r, 
    const ofPoint& p, 
    const ofFloatColor& color ) 
{

  float theta = (float)j / slen * TWO_PI;
  float x = r * cos(theta);
  float y = r * sin(theta);
  float xo = params.rmin * cos(theta);
  float yo = params.rmin * sin(theta);

  ofVec3f src( p.x+xo, p.y+yo );
  ofVec3f dst( p.x+x, p.y+y );

  mesh_lineas.addVertex( src );
  mesh_lineas.addColor( color );
  mesh_lineas.addVertex( dst );
  mesh_lineas.addColor( color ); 

}

float testApp::radio_map_x_sustancia(
    Data::Sustancia& s )
{

  Data::SustanciaStats& st = data.stats_x_sustancia()[ s.analito ];

  float r;
  //float _rref;

  if ( ! utils::equals(st.min,st.max) )
  {
    //_rref = ofMap( 1, 
      //st.min, st.max, 
      //params.rmin, params.rmax );
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

    Data::SustanciaStats& st = stats
      [ s.analito ];

    ofLog() 
      << s.analito 
      << " [" << st.min 
      << "," << st.max << "]"
      << ", sum: " << st.sum
      << ", indice: " << s.indice
      << ", date: "<< s.date.getIsoDate()
      << ", fecha: " << s.fecha
      << "";

  }

}

void testApp::log_plantas_coords()
{

  vector<Data::Planta>& plantas = 
    data.plantas();

  for ( int i = 0; 
      i < plantas.size(); 
      i++ )
  { 

    Data::Planta& planta = plantas[ i ];

    //ofPoint p = coords->get_screen_loc( planta );

    ofLog() 
      << planta.id
      << "\t"
      << ofToString(i+1)
      << "\t"
      << planta.lat
      << "\t"
      << planta.lon
      //<< p
      << "";
  }

};

bool testApp::render_calib()
{

  if ( pressed[ key::calib_ktex_src ] )
  { 
    mapimg.draw( 0,0,ofGetWidth(),ofGetHeight() );
    ofDrawBitmapStringHighlight("calib src", 10, 10, ofColor::yellow, ofColor::magenta);
    return true;
  }

  else if (pressed[key::calib_ktex_dst])
  {
    ofBackground(255); 
    //mapimg.draw( 0,0,ofGetWidth(),ofGetHeight() );
    ofSetColor( ofColor::magenta );
    ofDrawBitmapStringHighlight("calib dst", 10, 10, ofColor::yellow, ofColor::magenta);
    return true;
  }

  return false;
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

    case key::draw_charts:
      draw_charts = !draw_charts;
      break;

    case key::calib_ktex:
      calib_ktex = !calib_ktex;
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

    case ' ':
      params.log();
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

  int n;
  if ( pressed[ '1' ] ) n = 0;
  if ( pressed[ '2' ] ) n = 1;
  if ( pressed[ '3' ] ) n = 2;
  if ( pressed[ '4' ] ) n = 3;

  if ( pressed[ key::calib_ktex_src ] )
  { 
    ofLog() << "set H src point " 
      << ofToString(n) << " = " 
      << ofToString(x) << "," 
      << ofToString(y);
    src[n] = ofPoint(x,y);
    //findHomography(src, dst, H);
    warper.setSourcePoints(src);
    //H = warper.getMatrix();
  }

  else if (pressed[key::calib_ktex_dst])
  { 
    ofLog() << "set H dst point " 
      << ofToString(n) << " = " 
      << ofToString(x) << "," 
      << ofToString(y);
    dst[n] = ofPoint(x,y);
    //findHomography(src, dst, H);
    warper.setTargetPoints(dst);
    //H = warper.getMatrix();
  }

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

  ofLog() << "--src";
  ktex_cfg.pushTag("src");
  for ( int i = 0; i < npts; i++ )
  {

    int x = ktex_cfg
      .getValue("pt:x",0,i);
    int y = ktex_cfg
      .getValue("pt:y",0,i);

    ofLog() << "\t " << i
      << ": " << x << ", " << y;

    src[i] = ofPoint(x,y);
    warper.setSourcePoints(src);
  }
  ktex_cfg.popTag();


  ofLog() << "--dst";
  ktex_cfg.pushTag("dst");
  for ( int i = 0; i < npts; i++ )
  {

    int x = ktex_cfg
      .getValue("pt:x",0,i);
    int y = ktex_cfg
      .getValue("pt:y",0,i);

    ofLog() << "\t " << i
      << ": " << x << ", " << y;

    dst[i] = ofPoint(x,y);
    warper.setTargetPoints(dst);
  }
  ktex_cfg.popTag();

}

void testApp::save_ktex()
{
  ofLog() << "save ktex";


  ofLog() << "--src";
  ktex_cfg.pushTag("src");
  ofPoint* _src=warper.getSourcePoints();
  for ( int i = 0; i < npts; i++ )
  {
    ofPoint p = _src[ i ];

    ofLog() << "\t " << i
      << ": " << p.x << ", " << p.y;

    ktex_cfg
      .setValue("pt:x",p.x,i);
    ktex_cfg
      .setValue("pt:y",p.y,i);
  }
  ktex_cfg.popTag();


  ofLog() << "--dst";
  ktex_cfg.pushTag("dst");
  ofPoint* _dst=warper.getTargetPoints();
  for ( int i = 0; i < npts; i++ )
  {
    ofPoint p = _dst[ i ];

    ofLog() << "\t " << i
      << ": " << p.x << ", " << p.y;

    ktex_cfg
      .setValue("pt:x",p.x,i);
    ktex_cfg
      .setValue("pt:y",p.y,i);
  }
  ktex_cfg.popTag();


  ktex_cfg.saveFile("keystone.xml");
}

void testApp::reset_ktex()
{
  int x = 100;
  int y = 100;
  int w = width -x*2;
  int h = height -y*2;
  src.assign( 4, ofPoint() );
  dst.assign( 4, ofPoint() );
  //warper.setSourcePoints(src);
  //warper.setTargetPoints(src);
  warper.setSourceRect( ofRectangle( 0, 0, width, height ) );
  warper.setTargetRect( ofRectangle( x, y, w, h ) ); 
  //H = warper.getMatrix();
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
  //int _step = pressed[OF_KEY_LEFT_SHIFT] ? step2 : step1;
  //ofPoint c = ktex.getPoint(curpt);
  //ktex.setPoint(curpt, c.x-_step, c.y);
}

void testApp::moveRight()
{
  //int _step = pressed[OF_KEY_LEFT_SHIFT] ? step2 : step1;
  //ofPoint c = ktex.getPoint(curpt);
  //ktex.setPoint(curpt, c.x+_step, c.y);
}

void testApp::moveUp()
{
  //int _step = pressed[OF_KEY_LEFT_SHIFT] ? step2 : step1;
  //ofPoint c = ktex.getPoint(curpt);
  //ktex.setPoint(curpt, c.x, c.y-_step);
}

void testApp::moveDown()
{
  //int _step = pressed[OF_KEY_LEFT_SHIFT] ? step2 : step1;
  //ofPoint c = ktex.getPoint(curpt);
  //ktex.setPoint(curpt, c.x, c.y+_step);
}

