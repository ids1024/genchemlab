#ifndef POINT3D_H
#define POINT3D_H

class Point3D {
 public:
  void setXYZ(double x1, double y1, double z1) {
    xi = x1; yi = y1; zi = z1;
  }
  void setRGB(double r1, double g1, double b1) {
    ri = r1; gi = g1; bi = b1;
  }
  double x() { return xi; }
  double y() { return yi; }
  double z() { return zi; }
  double red() { return ri; }
  double green() { return gi; }
  double blue() { return bi; }

 private:
  double ri,gi,bi,xi,yi,zi;
};

#endif
