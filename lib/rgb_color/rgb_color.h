#ifndef rgb_color_h
#define rgb_color_h

class rgb_color {

  private:
    int my_r;
    int my_g;
    int my_b;

  public:
    rgb_color(int red, int green, int blue);
    int r() const;
    int b() const;
    int g() const;

};

#endif
