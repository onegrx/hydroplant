#include "rgb_color.h"

rgb_color::rgb_color(int red, int green, int blue) {
  // my_r = 255 - red;
  // my_g = 255 - green;
  // my_b = 255 - blue;

  my_r = red;
  my_g = green;
  my_b = blue;
}

int rgb_color::r() const {return my_r;}
int rgb_color::b() const {return my_b;}
int rgb_color::g() const {return my_g;}
