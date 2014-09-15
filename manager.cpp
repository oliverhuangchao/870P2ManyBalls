#include <iostream>
#include <string>
#include <iomanip>
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"

Manager::~Manager() { 
  // Manager made it, so Manager needs to delete it
  SDL_FreeSurface(orbSurface);
  delete orbFrame;
  delete IOManager::getInstance();
  delete Gamedata::getInstance();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  //--------------- get two singlton here ----------------
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io->getScreen() ),
  

  /*----------------- set the background coloor ----------  */
  backRed( Gamedata::getInstance()->getXmlInt("back/red") ),
  backGreen( Gamedata::getInstance()->getXmlInt("back/green") ),
  backBlue( Gamedata::getInstance()->getXmlInt("back/blue") ),

  
  // --------------- get other material in xml file----------
  orbSurface( io->loadAndSet(
    Gamedata::getInstance()->getXmlStr("greenorb/file"), 
    Gamedata::getInstance()->getXmlBool("greenorb/transparency")) ),
  orbFrame( new Frame("greenorb", orbSurface) ),

  /* --------- try to init one ball ---------- */
  //orb("greenorb", orbFrame),
  /* --------- try to init more balls ---------- */
  ballCount( Gamedata::getInstance()->getXmlInt("ballCount")),


  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance()->getXmlStr("username") ),
  frameMax( Gamedata::getInstance()->getXmlInt("frameMax") ),
  TITLE( Gamedata::getInstance()->getXmlStr("screenTitle") )
{

  /* ----------- init more balls ------ */
  //orb.resize(2);// here will call the construction method of Sprite
  for (int i = 0; i<ballCount; i++)
  {
    Sprite tmp("greenorb",orbFrame);
    orb.push_back(tmp);
  }


  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);
}

void Manager::drawBackground() const {
  SDL_FillRect( screen, NULL, 
    SDL_MapRGB(screen->format, backRed, backGreen, backBlue) );
  SDL_Rect dest = {0, 0, 0, 0};
  SDL_BlitSurface( screen, NULL, screen, &dest );
}

void Manager::draw() const {
  drawBackground();

  /*   ----- draw one ball here------- */
  //orb.draw();
  /*   ----- draw one ball here------- */
  for (int i = 0;i < ballCount;i ++)
      orb[i].draw();

  io->printMessageCenteredAt(TITLE, 10);
  io->printMessageValueAt("fps: ", clock.getFps(), 10, 10);
  SDL_Flip(screen);
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();
if( ( ticks < (unsigned) (1000 / 60) ) ) { 
    // delay remaining frame time
    SDL_Delay( ( 1000 / 60 ) - ticks );
} 

  /* --------- one ball need update ------------- */
  //orb.update(ticks);
  /* --------- many balls need update ------------- */
  for (int i = 0;i < ballCount;i++)
  {
    orb[i].update(ticks);
  }



  if ( makeVideo && frameCount < frameMax ) {
    std::stringstream strm;
    strm << "frames/" << username<< '.' 
         << std::setfill('0') << std::setw(4) 
         << frameCount++ << ".bmp";
    std::string filename( strm.str() );
    std::cout << "Making frame: " << filename << std::endl;
    SDL_SaveBMP(screen, filename.c_str());
  }
}

void Manager::play() {
  SDL_Event event;

  bool done = false;
  bool keyCatch = false;
  while ( not done ) {

    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
    }
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
        done = true;
        break;
      }

      if (keystate[SDLK_F4] && !makeVideo) {
        std::cout << "Making video frames" << std::endl;
        makeVideo = true;
      }
      if (keystate[SDLK_p] && !keyCatch) {
        keyCatch = true;
        if ( clock.isPaused() ) clock.unpause();
        else clock.pause();
      }
    }

    draw();
    update();
  }
}
