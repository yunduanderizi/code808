#ifndef MATHMATICS_H
#define MATHMATICS_H
#include <vector>
using namespace std;
const double INFINITY = 1e10;
const double ESP = 1e-5;
const int MAX_N = 1000;

struct Location {
    double x, y;
    void init(double x1 , double y1)
    {
        x = x1;
        y = y1;
    }
};
struct LineSegment {
    Location pt1, pt2;

};

typedef vector<Location> Polyline;

int InPolygon(const Polyline& polygon, Location point);

double lineDis(double x1, double y1, double x2, double y2);
double alSegmentDis(Location point1, Location point2, Location point);
double InLine(Polyline polygon,Location point);
#endif // MATHMATICS_H
