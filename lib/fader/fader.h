#ifndef fader_h
#define fader_h

#include "../rgb_color/rgb_color.h"

class fader {

  private:
    int r_pin;
    int g_pin;
    int b_pin;

  public:

    fader(int red_pin, int green_pin, int blue_pin);


    /*fade from rgb_in to rgb_out*/
    void fade( const rgb_color& in,
               const rgb_color& out,
               unsigned n_steps = 128,
               unsigned time    = 20);

};

#endif
