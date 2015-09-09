#include "mathmatics.h"
#include "math.h"
#include "stdio.h"

// 计算叉乘 |P0P1| × |P0P2|
double Multiply(Location p1, Location p2, Location p0)
{
    return ( (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y) );
}
// 判断线段是否包含点point
bool IsOnline(Location point, LineSegment line)
{
    return( ( fabs(Multiply(line.pt1, line.pt2, point)) < ESP ) &&
            ( ( point.x - line.pt1.x ) * ( point.x - line.pt2.x ) <= 0 ) &&
            ( ( point.y - line.pt1.y ) * ( point.y - line.pt2.y ) <= 0 ) );
}
// 判断线段相交
bool Intersect(LineSegment L1, LineSegment L2)
{
    return( (max(L1.pt1.x, L1.pt2.x) >= min(L2.pt1.x, L2.pt2.x)) &&
            (max(L2.pt1.x, L2.pt2.x) >= min(L1.pt1.x, L1.pt2.x)) &&
            (max(L1.pt1.y, L1.pt2.y) >= min(L2.pt1.y, L2.pt2.y)) &&
            (max(L2.pt1.y, L2.pt2.y) >= min(L1.pt1.y, L1.pt2.y)) &&
            (Multiply(L2.pt1, L1.pt2, L1.pt1) * Multiply(L1.pt2, L2.pt2, L1.pt1) >= 0) &&
            (Multiply(L1.pt1, L2.pt2, L2.pt1) * Multiply(L2.pt2, L1.pt2, L2.pt1) >= 0)
            );
}

// 判断点在多边形内
int InPolygon(const Polyline& polygon, Location point)
{
    int n = polygon.size();
    int count = 0;
    LineSegment line;
    line.pt1 = point;
    line.pt2.y = point.y;
    line.pt2.x = - INFINITY;

    for( int i = 0; i < n; i++ ) {
        // 得到多边形的一条边
        LineSegment side;
        side.pt1 = polygon[i];
        side.pt2 = polygon[(i + 1) % n];

        if( IsOnline(point, side) ) {
            return 1;
        }

        // 如果side平行x轴则不作考虑
        if( fabs(side.pt1.y - side.pt2.y) < ESP ) {
            continue;
        }

        if( IsOnline(side.pt1, line) ) {
            if( side.pt1.y > side.pt2.y ) count++;
        } else if( IsOnline(side.pt2, line) ) {
            if( side.pt2.y > side.pt1.y ) count++;
        } else if( Intersect(line, side) ) {
            count++;
        }
    }

    if ( count % 2 == 1 ) {return 0;}
    else { return 2;}
}

//double A;

//double B;

//double C;

///**
// * 求直线方程的一般式
// *
// * @param point1
// * @param point2
// *            直线l经过的两个点
// */
//void lineExp(Location point1, Location point2) {
//    /**
//     * 由起始点和终止点构成的直线方程一般式的系数A
//     */
//    A = (point1.y - point2.y)
//            / sqrt(pow((point1.y - point2.y), 2)
//                    + pow((point1.x - point2.x), 2));

//    /**
//     * 由起始点和终止点构成的直线方程一般式的系数
//     */
//    B = (point2.x - point1.x)
//            / sqrt(pow((point1.y - point2.y), 2)
//                    + pow((point1.x - point2.x), 2));

//    /**
//     * 由起始点和终止点构成的直线方程一般式的系数
//     */
//    C = (point1.x * point2.y - point2.x * point1.y)
//            / sqrt(pow((point1.y - point2.y), 2)
//                    + pow((point1.x - point2.x), 2));

//}

//double alLine(double x, double y) {
//        double d = fabs(A * (x) + B * (y) + C)/sqrt(pow(A,2)+pow(B,2));
//        return d;
//    }

double lineDis(double x1, double y1, double x2, double y2) {
        double lineLength = 0;
        lineLength = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
        return lineLength;
    }

double alSegmentDis(Location point1, Location point2, Location point) {
    double dis = 0;
    double a, b, c;
    double x = point.x;
    double y = point.y;

    a = lineDis(point1.x, point1.y, point2.x, point2.y);// 线段的长度
    b = lineDis(point1.x, point1.y, x, y); // point1到点的距离
    c = lineDis(point2.x, point2.y, x, y);//point2到点的距离
    if(c < 0.0001) return 1;
    if(a+b == c) return -1;
    if(a+c == b) return -1;

    if (c * c > a * a + b * b) { // 组成直角三角形或钝角三角形，投影在point1延长线上，
        //dis = b;
        return -2;
    }
    if (b * b > a * a + c * c) {// 组成直角三角形或钝角三角形，投影在point2延长线上，
        //dis = c;
        return -2;
    }

    if (c + b == a) {// 点在线段上
        dis = 0;
        return dis;
    }

    // 组成锐角三角形，则求三角形的高
    double p = (a + b + c) / 2;// 半周长
    double s = sqrt(p * (p - a) * (p - b) * (p - c));// 海伦公式求面积
    dis = 2 * s / a;// 返回点到线的距离（利用三角形面积公式求高）
    return dis;
}

double InLine(Polyline polygon,Location point)
{
    double x0 = point.x;
    double y0 = point.y;
    double x1 = polygon[0].x;
    double y1 = polygon[0].y;
    double x2 = polygon[1].x;
    double y2 = polygon[1].y;

    double d = (fabs((y2 - y1) * x0 +(x1 - x2) * y0 + ((x2 * y1) -(x1 * y2)))) / (sqrt(pow(y2 - y1, 2) + pow(x1 - x2, 2)));
}
