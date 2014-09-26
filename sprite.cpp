#include <iostream>
#include <cmath>
#include "sprite.h"
#include "gamedata.h"
//#define PI = 3.1415926
Sprite::Sprite(const std::string& name, const Frame* const frame) :
  Drawable(name,
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"/startLoc/x") + rand()%100*2, 
                    Gamedata::getInstance()->getXmlInt(name+"/startLoc/y") + rand()%100*2), 

           Vector2f((Gamedata::getInstance()->getXmlInt(name+"/speedX")), 
                    (Gamedata::getInstance()->getXmlInt(name+"/speedY")))

           ),
  frame( frame ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),

  centerX(Gamedata::getInstance()->getXmlInt(name+"/startcenter/x")),
  centerY(Gamedata::getInstance()->getXmlInt(name+"/startcenter/y")),


  worldWidth(Gamedata::getInstance()->getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance()->getXmlInt("world/height"))
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s),
  frame(s.frame),  // shallow copy; Manager may reuse it
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),

  centerX(s.getCenterX()),
  centerY(s.getCenterY()),

  worldWidth(Gamedata::getInstance()->getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance()->getXmlInt("world/height"))
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=(rhs);
  frame = rhs.frame;  // shallow copy; Manager may reuse it
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  centerX = rhs.centerX;
  centerY = rhs.centerY;
  return *this;
}

void Sprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

void Sprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  double  theta;

  if ( Y() > centerY && X() > centerX) {//4 area
    theta = atan((Y()-centerY)/(X()-centerX));
    velocityX( 1*sin(theta)*200 );
    velocityY( -1*cos(theta)*200 );
  }
  if ( Y() > centerY && X() < centerX) {//3 area
    theta = atan((Y() - centerY)/(centerX - X()));
    velocityX( 1*sin(theta)*200 );
    velocityY( 1*cos(theta)*200 );
  }
  if ( Y() < centerY && X() < centerX) {//2 area
    theta = atan((centerY - Y())/(centerX - X()));
    velocityX( -1*sin(theta)*200 );
    velocityY( 1*cos(theta)*200 );
  }
  if ( Y() < centerY && X() > centerX) {//1 area
    theta = atan((centerY - Y())/(X() - centerX));
    velocityX( -1*sin(theta)*200 );
    velocityY( -1*cos(theta)*200 );
  }
}
