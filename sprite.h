#ifndef SPRITE__H
#define SPRITE__H

#include <string>
#include "drawable.h"

class Sprite : public Drawable {
public:
  // In this version, frame is a parameter:
  Sprite(const std::string&, const Frame* const);  
  Sprite(const Sprite& s); // canonical form
  virtual ~Sprite() { }  // Don't delete frame; manager will reuse it 
  Sprite& operator=(const Sprite&); // canonical form

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { return frame; }

  int getCenterX()  const { return centerX; }
  int getCenterY() const { return centerY; }

private:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int centerX;
  int centerY;
  int worldWidth;
  int worldHeight;
  int getDistance(const Sprite*) const;
};
#endif
