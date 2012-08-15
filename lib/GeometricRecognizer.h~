//This code taken (and modified) from :

//1. $N Multistroke Recognizer (JavaScript version) by Jacob O. Wobbrock, Ph.D. and Lisa Anthony, Ph.D. .
//2. C++ $1 Unistroke Recognizer by Baylor Wetzel.

#ifndef _GeometricRecognizerIncluded_
#define _GeometricRecognizerIncluded_

#include <limits>
#include <iostream>
#include "GeometricRecognizerTypes.h"
#include "GestureTemplate.h"
#include "SampleGestures.h"
#include "MultipleStrokeGestureTemplate.h"
#include "SampleMultiStrokeGestures.h"
#include <string>
using namespace std;

namespace DollarRecognizer
{


	class GeometricRecognizer
	{
        protected:
                //--- These are doubleiables because C++ doesn't (easily) allow
		//---  constants to be floating point numbers
                double halfDiagonal;
		double angleRange;
		double anglePrecision;
		double goldenRatio;
                bool ndollar;
		//--- How many points we use to define a shape
		int numPointsInGesture;
		//---- Square we resize the shapes to
		int squareSize;
		
		bool shouldIgnoreRotation;
                //
                //
                // NDollarRecognizer class constants
                //
                double NumMultistrokes;
                double NumPoints ;
                double SquareSize;
                double OneDThreshold; // customize to desired gesture set (usually 0.20 - 0.35)
                Point2D Origin;
                double Diagonal;
                double HalfDiagonal;
                double AngleRange;
                double AnglePrecision;
                double Phi; // Golden Ratio
                double StartAngleIndex; // eighth of gesture length
                double AngleSimilarityThreshold;


                vector<int > order; // array of integer indices
                vector<vector<int> > orders ;// array of integer arrays
                //--- All templates saved in the library
                GestureTemplates allTemplates;
                //--- What we match the input shape against (sub part of allTemplates)
                GestureTemplates templates;
                //--- All templates saved in the library
                GestureTemplates allunistrokenormalizedtemplates;
                vector<GestureTemplates> allmultistrokenormalizedgestures;
                MultipleStrokeGestureTemplates allMtemplates;
                //--- What we match the input shape against (sub part of allTemplates)
                MultipleStrokeGestureTemplates Mtemplates;

	public:
		GeometricRecognizer();
//                Path2D addPointsToMakePath(Point2D v1,Point2D v2,Point2D v3,Point2D v4);
//                Path2D addPointsToMakePath(Point2D v1,Point2D v2,Point2D v3);
//                Path2D addPointsToMakePath(Point2D v1,Point2D v2);
                int addTemplate(string name, Path2D points);
		DollarRecognizer::Rectangle boundingBox(Path2D points);
		Point2D centroid(Path2D points);
		double getDistance(Point2D p1, Point2D p2);
               bool   getRotationInvariance() { return shouldIgnoreRotation; }
               double distanceAtAngle(
			Path2D points, GestureTemplate aTemplate, double rotation);
		double distanceAtBestAngle(Path2D points, GestureTemplate T);
		Path2D normalizePath(Path2D points);
		double pathDistance(Path2D pts1, Path2D pts2);
		double pathLength(Path2D points);
                RecognitionResult recognize(Path2D points, string method="goldenSearch");
		Path2D resample(Path2D points);
		Path2D rotateBy(Path2D points, double rotation);
		Path2D rotateToZero(Path2D points);
		Path2D scaleToSquare(Path2D points);
                void   setRotationInvariance(bool ignoreRotation);
                Path2D translateToOrigin(Path2D points);
                vector<double> vectorize(Path2D points); // for Protractor
                double optimalCosineDistance(vector<double> v1, vector<double> v2); // for Protractor
                void HeapPermute(int n);
                void Multistroke(string name,bool useBoundedRotationIndoubleiance, vector<Path2D> strokes); // constructor

                void loadTemplates();
                void activateTemplates(vector<string>);
                void activateTemplates();
                void loadMultistrokeTemplates();
                void activateMultiStrokesTemplates(vector<string>);

                RecognitionResult Multirecognize(MultiStrokeGesture paths, string method);
        private:
                bool inTemplates(string, vector<string>);
                double Deg2Rad(double d);
                double Rad2Deg(double r);

                double Round(double n,double d);
                int addMultiStrokesTemplate(string name, MultiStrokeGesture paths);
                MultiStrokeGesture MakeUnistrokes(MultiStrokeGesture strokes);
                Point2D CalcStartUnitVector(Path2D points,double index) ;// start angle from points[0] to points[index] normalized as a unit vector
                vector<double> Vectorize(Path2D points,bool useBoundedRotationInvariance); // for Protractor
                Path2D CombineStrokes(MultiStrokeGesture strokes);
                Path2D UnistrokeTemplate(Path2D points,string name);

                double AngleBetweenUnitVectors(Point2D v1,Point2D v2);

	};
}
#endif
