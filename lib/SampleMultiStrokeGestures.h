#ifndef SAMPLEMULTISTROKEGESTURES_H
#define SAMPLEMULTISTROKEGESTURES_H

#include "GeometricRecognizer.h"
#include "GeometricRecognizerTypes.h"
#include "SampleGestures.h"
#include "utils.h"
// TODO: read from a file. This approach is just ugly
namespace DollarRecognizer
{
        //--- Make it a struct so that everything defaults to public
        struct SampleMultiStrokeGestures
        {
            utils util;

                MultiStrokeGesture getGestureT()
                {
                        MultiStrokeGesture Multistrokes;
                        Multistrokes.push_back(util.addPointsToMakePath(Point2D(30,7),Point2D(103,7)));
                        Multistrokes.push_back(util.addPointsToMakePath(Point2D(66,7),Point2D(66,87)));
                        return Multistrokes;
                }

                MultiStrokeGesture getGestureN()
                {
                    MultiStrokeGesture MultiStrokes;
                    MultiStrokes.push_back(util.addPointsToMakePath(Point2D(177,92),Point2D(177,2)));
                    MultiStrokes.push_back(util.addPointsToMakePath(Point2D(182,1),Point2D(246,95)));
                    MultiStrokes.push_back(util.addPointsToMakePath(Point2D(247,87),Point2D(247,1)));
                    return MultiStrokes;
                }
                MultiStrokeGesture getGestureD()
                {
                    MultiStrokeGesture MultiStrokes;
                    MultiStrokes.push_back( util.addPointsToMakePath(Point2D(345,9),Point2D(345,87)));
                    MultiStrokes.push_back(util.addPointsToMakePath(Point2D(351,8),Point2D(363,8),Point2D(372,9),Point2D(380,11),Point2D(386,14),Point2D(391,17),Point2D(394,22),Point2D(397,28),Point2D(399,34),Point2D(400,42),Point2D(400,50),Point2D(400,56),Point2D(399,61),Point2D(397,66),Point2D(394,70),Point2D(391,74),Point2D(386,78),Point2D(382,81),Point2D(377,83),Point2D(372,85),Point2D(367,87),Point2D(360,87),Point2D(355,88),Point2D(349,87)));
                    return MultiStrokes;
                }
                MultiStrokeGesture getGestureP()
                {
                    MultiStrokeGesture MultiStrokes;
                    MultiStrokes.push_back(util.addPointsToMakePath(Point2D(507,8),Point2D(507,87)));
                    MultiStrokes.push_back(util.addPointsToMakePath(Point2D(513,7),Point2D(528,7),Point2D(537,8),Point2D(544,10),Point2D(550,12),Point2D(555,15),Point2D(558,18),Point2D(560,22),Point2D(561,27),Point2D(562,33),Point2D(561,37),Point2D(559,42),Point2D(556,45),Point2D(550,48),Point2D(544,51),Point2D(538,53),Point2D(532,54),Point2D(525,55),Point2D(519,55),Point2D(513,55),Point2D(510,55)));
                    return MultiStrokes;
                }

                MultiStrokeGesture getGestureX()
                {
                    MultiStrokeGesture MultiStrokes;
                    MultiStrokes.push_back(util.addPointsToMakePath(Point2D(30,146),Point2D(106,222)));
                    MultiStrokes.push_back(util.addPointsToMakePath(Point2D(30,225),Point2D(106,146)));
                    return MultiStrokes;
                }

                MultiStrokeGesture getGestureI()
                {
                    MultiStrokeGesture MultiStrokes;
                    MultiStrokes.push_back(util.addPointsToMakePath(Point2D(371,149),Point2D(371,221)));
                    MultiStrokes.push_back(util.addPointsToMakePath(Point2D(371,149),Point2D(371,221)));
                     MultiStrokes.push_back(util.addPointsToMakePath(Point2D(341,221),Point2D(401,221)));
                    return MultiStrokes;
                }

        };
}


#endif // SAMPLEMULTISTROKEGESTURES_H
