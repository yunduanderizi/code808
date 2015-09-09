#include "map_laneinfo.h"

#include <qpainter.h>
#include "math.h"
#include "display/di3Dengi.h"
#include "engineinterface/engineinterface.h"
#include "engineinterface/interface_guide.h"

static const double cDirectionAngles[9] = {
    PI,         // ROAD_LANE_DIR_STRAIGHT     -> 180 deg
    0.75 * PI,  // ROAD_LANE_DIR_SLIGHT_RIGHT -> 135 deg
    0.5 * PI,   // ROAD_LANE_DIR_RIGHT        -> 90 deg
    0.25 * PI,  // ROAD_LANE_DIR_SHARP_RIGHT  -> 45 deg
    0,          // ROAD_LANE_DIR_LEFT_UTURN   -> 0 deg
    1.75 * PI,  // ROAD_LANE_DIR_SHARP_LEFT   -> 315 deg
    1.5 * PI,   // ROAD_LANE_DIR_LEFT         -> 270 deg
    1.25 * PI,  // ROAD_LANE_DIR_SLIGHT_LEFT  -> 225 deg
    0           // ROAD_LANE_DIR_RIGHT_UTURN  -> 0 deg
};

map_laneinfo::map_laneinfo(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}

map_laneinfo::~map_laneinfo()
{
}

void map_laneinfo::paintEvent(QPaintEvent *event)
{
    navcoreJourney::TIntersectionDescr intersectiondetal= guideGetTintersectionInDetail();
    QPainterPath path;
    QPainterPath tmppath;
    QPainter painter(this);

    static const int cPictorialMargin = 8;
    static const int cGapSize = 2;
    static const int cHighlightThickness = 4;
    static const int cLowlightThickness = 2;


    QPen highlightPen(Qt::white,cHighlightThickness);
    QPen lowlightPen(Qt::gray,cLowlightThickness);
    //      // safety check
    if ( !intersectiondetal.lanePicValid || !intersectiondetal.lanePic.laneCount ) {
        LOG_ERROR( 10, ("Couldn't draw lane pictorial as it's not valid") );
        return;
    }

    //      // create new reduced canvas, creating a amrgin to the actual drawing area
    QRect myDrawArea(10+cPictorialMargin,
                     0+cPictorialMargin,
                     170-cPictorialMargin,
                     80-cPictorialMargin);
    //      CRect myDrawArea( bounds.TopLeft().x + cPictorialMargin,
    //                        bounds.TopLeft().y + cPictorialMargin,
    //                        bounds.BottomRight().x - cPictorialMargin,
    //                        bounds.BottomRight().y - cPictorialMargin );
    painter.fillRect(this->rect(),QBrush(Qt::blue));
    //      // calculate width and height for each lane symbol
    int laneWidth =(this->width()-30) / intersectiondetal.lanePic.laneCount;
    int halfLaneWidth = laneWidth >> 1;
    int laneHeight = this->height()-16;
    if ( laneHeight > laneWidth ) {
        laneHeight = laneWidth;
    }
    int halfLaneHeight = laneHeight >> 1;

    //      // create new pens and store the old one
    //      CPen highlightPen;
    //      highlightPen.CreatePen(PS_SOLID, cHighlightThickness, c_pictorialHighlightColor);
    //      CPen lowlightPen;
    //      lowlightPen.CreatePen(PS_SOLID, cLowlightThickness, c_pictorialLowlightColor);
    //      CPen *oldPen = dc.SelectObject(&highlightPen);

    //      // render each lane:
    for ( UINT8 lane=0; lane<intersectiondetal.lanePic.laneCount; ++lane ) {

        // pre-calculate center of lane to draw
        QPoint laneCenter(myDrawArea.topLeft().x() + lane*laneWidth + halfLaneWidth,
                          myDrawArea.center().y());

        // draw upper part for all directions not on the path
        // dc.SelectObject(&lowlightPen);
        for ( UINT8 direction=0; direction<9; ++direction ) {
            UINT16 directionFlag = 1 << direction;
            if ( ( intersectiondetal.lanePic.reqLaneDir[lane] & directionFlag ) ||
                 !( intersectiondetal.lanePic.laneDir[lane] & directionFlag ) )
            {
                continue;
            }
            path.moveTo(laneCenter);
            path.lineTo(laneCenter.x() + (int)((double)(halfLaneHeight-cGapSize) * sin(cDirectionAngles[direction])),
                        laneCenter.y() + (int)((double)(halfLaneHeight-cGapSize) * cos(cDirectionAngles[direction])));
        }

        // draw upper part for all directions on the path
        //   dc.SelectObject(&highlightPen);
        for ( UINT8 direction=0; direction<9; ++direction ) {
            UINT16 directionFlag = 1 << direction;
            if ( !( intersectiondetal.lanePic.reqLaneDir[lane] & directionFlag ) ) {
                continue;
            }
            path.moveTo(laneCenter);
            QPoint tip(laneCenter.x() + (int)((double)(halfLaneHeight-cGapSize+3) * sin(cDirectionAngles[direction])),
                       laneCenter.y() + (int)((double)(halfLaneHeight-cGapSize+3) * cos(cDirectionAngles[direction])));
            path.lineTo(tip);
            path.moveTo(tip);
            path.lineTo(tip.x() + (int)((double)halfLaneHeight * 0.3 * sin(cDirectionAngles[direction] + 0.85 * PI)),
                        tip.y() + (int)((double)halfLaneHeight * 0.3 * cos(cDirectionAngles[direction] + 0.85 * PI)));
            path.moveTo(tip);
            path.lineTo(tip.x() + (int)((double)halfLaneHeight * 0.3 * sin(cDirectionAngles[direction] - 0.85 * PI)),
                        tip.y() + (int)((double)halfLaneHeight * 0.3 * cos(cDirectionAngles[direction] - 0.85 * PI)));
        }
        // draw lower part (where we are coming from)
        path.moveTo(laneCenter);
        path.lineTo(laneCenter.x(), laneCenter.y() + halfLaneHeight + 2);
        if (intersectiondetal.lanePic.reqLaneDir[lane]) {
            painter.setPen(highlightPen);
            painter.drawPath(path);
        } else {
            painter.setPen(lowlightPen);
            painter.drawPath(path);
        }
        path = tmppath;
        // painter.setPen(Qt::green,);
    }
}

void map_laneinfo::repaint()
{
    update();
}

void map_laneinfo::refresh()
{ 
    repaint();
}

void map_laneinfo::drawEntryAndExits()
{
    QPainter painter(this);
    navcoreJourney::TIntersectionDescr intersectiondetal= guideGetTintersectionInDetail();
    QPoint centre(50,50);
    const int radius = 40;
    QPainterPath path,path1;

    QPen highlightPen(Qt::green,4);
    QPen lowlightPen(Qt::black,2);

  // Save the old pen first to restore later.
  //oldPen = dc.SelectObject(&highlightPen);

  // Render the entry to the intersection first.
//  painter.pen(highlightPen);

  // Render exit points including exit from the intersection.
  // Displacements for rays emanating from centre point are based on the
  // following calculations:
  //
  // x = -(r sin(theta))
  // y = -(r cos(theta))
  //
  // Where theta is in radians (degrees * pi/180).
  //
  // Draw rays that aren't the guidance entry and exit rays last...
  float x;
  float y;
  unsigned int i;
  for (i = 0; i < intersectiondetal.roadCount; i++) {
    x = (-((float)radius * sin(intersectiondetal.exitHeadings[i] * ((float)PI / 180))));
    y = (-((float)radius * cos(intersectiondetal.exitHeadings[i] * ((float)PI / 180))));

    path.moveTo(centre);
    if (i != intersectiondetal.exitIndex) {
      //temp = dc.SelectObject(&lowlightPen);
      path.lineTo(centre.x()+ (int)x, centre.y()- (int)y);
    }
  }

  painter.setPen(lowlightPen);
  painter.drawPath(path);
  path = path1;
  // Draw the guidance entry and exit rays last...
  for (i = 0; i < intersectiondetal.roadCount; i++) {
    x = (-((float)radius * sin(intersectiondetal.exitHeadings[i] * ((float)PI / 180))));
    y = (-((float)radius * cos(intersectiondetal.exitHeadings[i] * ((float)PI / 180))));

    path.moveTo(centre);
    if (i == intersectiondetal.exitIndex) {
      //temp = dc.SelectObject(&highlightPen);
      path.lineTo(centre.x() + (int)x, centre.y()- (int)y);
    }
  }
  path.moveTo(centre);
  path.lineTo(centre.x(), centre.y() + 40);
  painter.setPen(highlightPen);
  painter.drawPath(path);
}


void map_laneinfo::drawRoundabout()
{
    navcoreJourney::TIntersectionDescr intersectiondetal= guideGetTintersectionInDetail();
    QPoint centre(50,50);
    const int radius = 25;
    QPainterPath path,path1;
    QPainter patinter(this);

    QPen highlightPen(Qt::green,4);
    QPen lowlightPen(Qt::black,2);

    //highlightPen.CreatePen(PS_SOLID, 7, c_pictorialHighlightColor);
    //lowlightPen.CreatePen(PS_SOLID, 3, c_pictorialLowlightColor);

    // Save old pen and brush.
  //  oldPen = dc.SelectObject(&lowlightPen);
    //oldBrush = dc.SelectObject(&backgroundBrush);

    // Draw roundabout using grey colour pen with blue floodfill...
    int arcBBminX = centre.x()- radius;
    int arcBBminY = centre.y()- radius;
    int arcBBmaxX = centre.x()+ radius;
    int arcBBmaxY = centre.y()+ radius;
    QRectF bound(arcBBminX,arcBBminY,2*radius,2*radius);
    path.addEllipse(bound);
    patinter.setPen(lowlightPen);
    patinter.drawPath(path);
    path = path1;
 //   dc.Ellipse(arcBBminX, arcBBminY, arcBBmaxX, arcBBmaxY);

    // Draw the roundabout:
    // Need to consider handedness of the road network.
    // Also need to calculate points on the arc through which the roundabout
    // will go...
    float exitX = (-((float)radius * sin(intersectiondetal.exitHeadings[intersectiondetal.exitIndex] * ((float)PI / 180))));
    float exitY = (-((float)radius * cos(intersectiondetal.exitHeadings[intersectiondetal.exitIndex] * ((float)PI / 180))));
    bool arcRC;

    patinter.setPen(highlightPen);

    if (intersectiondetal.isLeftHandDrive) {
        // Go clockwise...
       // dc.SetArcDirection(AD_CLOCKWISE);
    } else {
        // Go anticlockwise...
       // dc.SetArcDirection(AD_COUNTERCLOCKWISE);
    }
    path.moveTo(centre.x(), centre.y() + radius);
    path.arcTo(bound,-90,360-intersectiondetal.exitHeadings[intersectiondetal.exitIndex]);

    patinter.drawPath(path);
//     Restore old pen...
//    tempPen = dc.SelectObject(oldPen);
//    tempBrush = dc.SelectObject(oldBrush);

    // Safe to delete pens now...
//    highlightPen.DeleteObject();
//    lowlightPen.DeleteObject();
//    backgroundBrush.DeleteObject();
}
