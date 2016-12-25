#include "fader.h"
#include "Arduino.h"

fader::fader(int red_pin, int green_pin, int blue_pin) {

  r_pin = red_pin;
  g_pin = green_pin;
  b_pin = blue_pin;

}

void fader::fade(const rgb_color& in, const rgb_color& out,
  unsigned n_steps, unsigned time) {

  int red_diff   = out.r() - in.r();
  int green_diff = out.g() - in.g();
  int blue_diff  = out.b() - in.b();
  for ( unsigned i = 0; i < n_steps; ++i) {
    /* output is the color that is actually written to the pins
     * and output nicely fades from in to out.
     */
    rgb_color output ( in.r() + i * red_diff / n_steps,
                       in.g() + i * green_diff / n_steps,
                       in.b() + i * blue_diff/ n_steps);
    /*put the analog pins to the proper output.*/
    analogWrite( r_pin, 255-output.r() );
    analogWrite( g_pin, 255- output.g() );
    analogWrite( b_pin, 255-output.b() );
    delay(time);
  }

}
