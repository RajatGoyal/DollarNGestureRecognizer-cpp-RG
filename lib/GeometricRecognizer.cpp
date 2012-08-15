#include "GeometricRecognizer.h"
#include <math.h>
#include <algorithm>
#define MAX_DOUBLE std::numeric_limits<double>::max();

//This code taken (and modified) from :

//1. $N Multistroke Recognizer (JavaScript version) by Jacob O. Wobbrock, Ph.D. and Lisa Anthony, Ph.D. .
//2. C++ $1 Unistroke Recognizer by Baylor Wetzel.

namespace DollarRecognizer
{
	GeometricRecognizer::GeometricRecognizer()
	{
		//--- How many templates do we have to compare the user's gesture against?
		//--- Can get ~97% accuracy with just one template per symbol to recognize
		//numTemplates = 16;
		//--- How many points do we use to represent a gesture
		//--- Best results between 32-256
		numPointsInGesture = 128;
		//--- Before matching, we stretch the symbol across a square
		//--- That way we don't have to worry about the symbol the user drew
		//---  being smaller or larger than the one in the template
		squareSize = 250;
		//--- 1/2 max distance across a square, which is the maximum distance
		//---  a point can be from the center of the gesture
		halfDiagonal = 0.5 * sqrt((250.0 * 250.0) + (250.0 * 250.0));
		//--- Before matching, we rotate the symbol the user drew so that the 
		//---  start point is at degree 0 (right side of symbol). That's how 
		//---  the templates are rotated so it makes matching easier
		//--- Note: this assumes we want symbols to be rotation-invariant, 
		//---  which we might not want. Using this, we can't tell the difference
		//---  between squares and diamonds (which is just a rotated square)
		setRotationInvariance(false);
		anglePrecision = 2.0;
		//--- A magic number used in pre-processing the symbols
		goldenRatio    = 0.5 * (-1.0 + sqrt(5.0));

                NumMultistrokes = 16;
                NumPoints = 96;
                SquareSize = 250.0;
                OneDThreshold = 0.25; // customize to desired gesture set (usually 0.20 - 0.35)
                Origin = Point2D();
                Diagonal = sqrt(SquareSize * SquareSize + SquareSize * SquareSize);
                HalfDiagonal = 0.5 * Diagonal;
                AngleRange = Deg2Rad(45.0);
                AnglePrecision = Deg2Rad(2.0);
                Phi = 0.5 * (-1.0 + sqrt(5.0)); // Golden Ratio
                StartAngleIndex = (NumPoints / 8); // eighth of gesture length
                AngleSimilarityThreshold = Deg2Rad(30.0);



        }

        bool GeometricRecognizer::inTemplates(string templ, vector<string> list)
        {
            for(unsigned int i=0;i<list.size();i++)
            {
            if(list.at(i)==templ)
            {
                return true;
            }
            }
            return false;
       }

        void GeometricRecognizer::activateTemplates(vector<string> list)
        {
            for (unsigned int i=0; i<allTemplates.size() ; i++)
                if (inTemplates(allTemplates.at(i).name, list))
                    templates.push_back(allTemplates.at(i));
       }

	void GeometricRecognizer::loadTemplates()
	{
		SampleGestures samples;

		addTemplate("Arrow", samples.getGestureArrow());
		addTemplate("Caret", samples.getGestureCaret());
		addTemplate("CheckMark", samples.getGestureCheckMark());
		addTemplate("Circle", samples.getGestureCircle());
		addTemplate("Delete", samples.getGestureDelete());
		addTemplate("Diamond", samples.getGestureDiamond());
		//addTemplate("LeftCurlyBrace", samples.getGestureLeftCurlyBrace());
		addTemplate("LeftSquareBracket", samples.getGestureLeftSquareBracket());
		addTemplate("LeftToRightLine", samples.getGestureLeftToRightLine());
		addTemplate("LineDownDiagonal", samples.getGestureLineDownDiagonal());
		addTemplate("Pigtail", samples.getGesturePigtail());
		addTemplate("QuestionMark", samples.getGestureQuestionMark());
		addTemplate("Rectangle", samples.getGestureRectangle());
		//addTemplate("RightCurlyBrace", samples.getGestureRightCurlyBrace());
		addTemplate("RightSquareBracket", samples.getGestureRightSquareBracket());
		addTemplate("RightToLeftLine", samples.getGestureRightToLeftLine());
		addTemplate("RightToLeftLine2", samples.getGestureRightToLeftLine2());
		addTemplate("RightToLeftSlashDown", samples.getGestureRightToLeftSlashDown());
		addTemplate("Spiral", samples.getGestureSpiral());
		addTemplate("Star", samples.getGestureStar());
		addTemplate("Triangle", samples.getGestureTriangle());
		addTemplate("V", samples.getGestureV());
		addTemplate("X", samples.getGestureX());
	}

	int GeometricRecognizer::addTemplate(string name, Path2D points)
	{
		points = normalizePath(points);

                allTemplates.push_back(GestureTemplate(name, points));
		//--- Let them know how many examples of this template we have now
		int numInstancesOfGesture = 0;
		// You know, i don't care so i'm just going to ignore this
		//for (var i = 0; i < templates.size(); i++)
		//{
		//	if (templates[i].Name == name)
		//		numInstancesOfGesture++;
		//}
		return numInstancesOfGesture;
	}

	Rectangle GeometricRecognizer::boundingBox(Path2D points)
	{
		double minX =  MAX_DOUBLE;
		double maxX = -MAX_DOUBLE;
		double minY =  MAX_DOUBLE; 
		double maxY = -MAX_DOUBLE;

		for (Path2DIterator i = points.begin(); i != points.end(); i++)
		{
			Point2D point = *i;
			if (point.x < minX)
				minX = point.x;
			if (point.x > maxX)
				maxX = point.x;
			if (point.y < minY)
				minY = point.y;
			if (point.y > maxY)
				maxY = point.y;
		}
		Rectangle bounds(minX, minY, (maxX - minX), (maxY - minY));
		return bounds;
	}

	Point2D GeometricRecognizer::centroid(Path2D points)
	{
		double x = 0.0, y = 0.0;
		for (Path2DIterator i = points.begin(); i != points.end(); i++)
		{
			Point2D point = *i;
			x += point.x;
			y += point.y;
		}
		x /= points.size();
		y /= points.size();
		return Point2D(x, y);
	}	

	double GeometricRecognizer::getDistance(Point2D p1, Point2D p2)
	{
		double dx = p2.x - p1.x;
		double dy = p2.y - p1.y;
		double distance = sqrt((dx * dx) + (dy * dy));
		return distance;
	}

	double GeometricRecognizer::distanceAtAngle(
		Path2D points, GestureTemplate aTemplate, double rotation)
	{
                Path2D newPoints = rotateBy(points, rotation);
		return pathDistance(newPoints, aTemplate.points);
	}	

	double GeometricRecognizer::distanceAtBestAngle(
		Path2D points, GestureTemplate aTemplate)
	{
		double startRange = -angleRange;
		double endRange   =  angleRange;
		double x1 = goldenRatio * startRange + (1.0 - goldenRatio) * endRange;
		double f1 = distanceAtAngle(points, aTemplate, x1);
		double x2 = (1.0 - goldenRatio) * startRange + goldenRatio * endRange;
		double f2 = distanceAtAngle(points, aTemplate, x2);
                while (fabs(endRange - startRange) > anglePrecision)
		{
			if (f1 < f2)
			{
				endRange = x2;
				x2 = x1;
				f2 = f1;
				x1 = goldenRatio * startRange + (1.0 - goldenRatio) * endRange;
				f1 = distanceAtAngle(points, aTemplate, x1);
			}
			else
			{
				startRange = x1;
				x1 = x2;
				f1 = f2;
				x2 = (1.0 - goldenRatio) * startRange + goldenRatio * endRange;
				f2 = distanceAtAngle(points, aTemplate, x2);
			}
		}
		return min(f1, f2);
	}

	Path2D GeometricRecognizer::normalizePath(Path2D points)
	{
		/* Recognition algorithm from 
			http://faculty.washington.edu/wobbrock/pubs/uist-07.1.pdf
			Step 1: Resample the Point Path
			Step 2: Rotate Once Based on the "Indicative Angle"
			Step 3: Scale and Translate
			Step 4: Find the Optimal Angle for the Best Score
		*/
		// TODO: Switch to $N algorithm so can handle 1D shapes

                //--- Make everyone have the same number of points (anchor points)
		points = resample(points);
		//--- Pretend that all gestures began moving from right hand side
		//---  (degree 0). Makes matching two items easier if they're
		//---  rotated the same
		if (getRotationInvariance())
			points = rotateToZero(points);
		//--- Pretend all shapes are the same size. 
		//--- Note that since this is a square, our new shape probably
		//---  won't be the same aspect ratio
		points = scaleToSquare(points);
		//--- Move the shape until its center is at 0,0 so that everyone
		//---  is in the same coordinate system
		points = translateToOrigin(points);

		return points;
	}

        vector<double> GeometricRecognizer::vectorize(Path2D points) // for Protractor
        {
                double sum = 0.0;
                vector<double> vectorized;

                // Preprocessing, Move from .x.y notation to 1D vector notation
                // points[i](.x,.y) => vectorized(i, i+1, ...)
                for (unsigned int i = 0; i < points.size(); i++)
                {
                    vectorized.push_back(points[i].x);
                    vectorized.push_back(points[i].y);
                    sum += points[i].x * points[i].x + points[i].y * points[i].y;
                }
                // normalize values : dividing by magnitude
                // magnitude = sqrt ( sum_i(x²) + sum_i(y²) )
                double magnitude = sqrt(sum);
                for (unsigned int i = 0; i < vectorized.size(); i++)
                        vectorized[i] /= magnitude;
                return vectorized;
        }

        double GeometricRecognizer::optimalCosineDistance(vector<double> v1, vector<double> v2) // for Protractor
        {
                double a = 0.0;
                double b = 0.0;

                // Compute protractor a and b values
                // a = sum_i( xt_i*xg_i + yt_i*yg_i)
                // b = sum_i( xt_i*yg_i - yt_i*xg_i)
                for (unsigned int i = 0; i < v1.size(); i += 2)
                {
                        a += v1[i] * v2[i] + v1[i + 1] * v2[i + 1];
                        b += v1[i] * v2[i + 1] - v1[i + 1] * v2[i];
                }
                double angle = atan(b / a);
                return acos(a * cos(angle) + b * sin(angle));
        }

	double GeometricRecognizer::pathDistance(Path2D pts1, Path2D pts2)
	{
		// assumes pts1.size == pts2.size

		double distance = 0.0;
                for (int i = 0; i < (int)pts1.size(); i++)
                        distance += getDistance(pts1[i], pts2[i]);
                return (distance / pts1.size());
	}

	double GeometricRecognizer::pathLength(Path2D points)
	{
		double distance = 0;
		for (int i = 1; i < (int)points.size(); i++)
			distance += getDistance(points[i - 1], points[i]);
		return distance;
	}

        RecognitionResult GeometricRecognizer::recognize(Path2D points, string method)
	{
		//--- Make sure we have some templates to compare this to
		//---  or else recognition will be impossible
		if (templates.empty())
		{
			std::cout << "No templates loaded so no symbols to match." << std::endl;
                        return RecognitionResult("Unknown", 0);
		}

		points = normalizePath(points);
	
		//--- Initialize best distance to the largest possible number
		//--- That way everything will be better than that
                double bestDistance = MAX_DOUBLE;
		//--- We haven't found a good match yet
		int indexOfBestMatch = -1;
                double score = 0.0;

                    //--- Check the shape passed in against every shape in our database
                    for (int i = 0; i < (int)templates.size(); i++)
                    {
                        double distance;
                            //--- Calculate the total distance of each point in the passed in
                            //---  shape against the corresponding point in the template
                            //--- We'll rotate the shape a few degrees in each direction to
                            //---  see if that produces a better match
                        if (method=="protractor")
                            distance = optimalCosineDistance(vectorize(points), vectorize(templates[i].points));
                        else
                            distance = distanceAtBestAngle(points, templates[i]);

                            //cout << distance<< " " << bestDistance << " ";
                            //cout << " = " ;
                            if (distance < bestDistance)
                            {
                                    bestDistance     = distance;
                                    indexOfBestMatch = i;
                            }
                    }
                    //--- Turn the distance into a percentage by dividing it by
                    //---  half the maximum possible distance (across the diagonal
                    //---  of the square we scaled everything too)
                    //--- Distance = hwo different they are
                    //--- Subtract that from 1 (100%) to get the similarity
                    if (method=="protractor")
                        { score = bestDistance ? (1.0 / bestDistance) : MAX_DOUBLE ; }
                    else
                        score = 1.0 - (bestDistance / halfDiagonal);
                    //cout << score << "   " << bestDistance<< "   " ;

		//--- Make sure we actually found a good match
		//--- Sometimes we don't, like when the user doesn't draw enough points
		if (-1 == indexOfBestMatch)
		{
			cout << "Couldn't find a good match." << endl;
			return RecognitionResult("Unknown", 1);
		}
		RecognitionResult bestMatch(templates[indexOfBestMatch].name, score);
		return bestMatch;
	};

	Path2D GeometricRecognizer::resample(Path2D points)
	{
		double interval = pathLength(points) / (numPointsInGesture - 1); // interval length
		double D = 0.0;
		Path2D newPoints;

		//--- Store first point since we'll never resample it out of existence
		newPoints.push_back(points.front());
	    for(int i = 1; i < (int)points.size(); i++)
		{
			Point2D currentPoint  = points[i];
			Point2D previousPoint = points[i-1];
			double d = getDistance(previousPoint, currentPoint);
			if ((D + d) >= interval)
			{
				double qx = previousPoint.x + ((interval - D) / d) * (currentPoint.x - previousPoint.x);
				double qy = previousPoint.y + ((interval - D) / d) * (currentPoint.y - previousPoint.y);
				Point2D point(qx, qy);
				newPoints.push_back(point);
				points.insert(points.begin() + i, point);
				D = 0.0;
			}
			else D += d;
		}

		// somtimes we fall a rounding-error short of adding the last point, so add it if so
		if (newPoints.size() == (numPointsInGesture - 1))
		{
			newPoints.push_back(points.back());
		}

		return newPoints;
	}

        Path2D GeometricRecognizer::rotateBy(Path2D points, double rotation)
	{
		Point2D c     = centroid(points);
		//--- can't name cos; creates compiler error since VC++ can't
		//---  tell the difference between the variable and function
		double cosine = cos(rotation);	
		double sine   = sin(rotation);
		
		Path2D newPoints;
		for (Path2DIterator i = points.begin(); i != points.end(); i++)
		{
			Point2D point = *i;
			double qx = (point.x - c.x) * cosine - (point.y - c.y) * sine   + c.x;
			double qy = (point.x - c.x) * sine   + (point.y - c.y) * cosine + c.y;
			newPoints.push_back(Point2D(qx, qy));
		}
		return newPoints;
	}

	Path2D GeometricRecognizer::rotateToZero(Path2D points)
	{
		Point2D c = centroid(points);
		double rotation = atan2(c.y - points[0].y, c.x - points[0].x);
		return rotateBy(points, -rotation);
	}

	Path2D GeometricRecognizer::scaleToSquare(Path2D points)
	{
		//--- Figure out the smallest box that can contain the path
		DollarRecognizer::Rectangle box = boundingBox(points);
		Path2D newPoints;
		for (Path2DIterator i = points.begin(); i != points.end(); i++)
		{
			Point2D point = *i;
			//--- Scale the points to fit the main box
			//--- So if we wanted everything 100x100 and this was 50x50,
			//---  we'd multiply every point by 2
			double scaledX = point.x * (this->squareSize / box.width);
			double scaledY = point.y * (this->squareSize / box.height);
			//--- Why are we adding them to a new list rather than 
			//---  just scaling them in-place?
			// TODO: try scaling in place (once you know this way works)
			newPoints.push_back(Point2D(scaledX, scaledY));
		}
		return newPoints;
	}

        void GeometricRecognizer::setRotationInvariance(bool ignoreRotation)
	{
		shouldIgnoreRotation = ignoreRotation;

		if (shouldIgnoreRotation)
		{
			angleRange = 45.0;
		}
		else
		{
			angleRange = 15.0;
		}
	}

	/**
	 * Shift the points so that the center is at 0,0.
	 * That way, if everyone centers at the same place, we can measure
	 *  the distance between each pair of points without worrying about
	 *  where each point was originally drawn
	 * If we didn't do this, shapes drawn at the top of the screen
	 *  would have a hard time matching shapes drawn at the bottom
	 *  of the screen
	 */
	Path2D GeometricRecognizer::translateToOrigin(Path2D points)
	{
		Point2D c = centroid(points);
		Path2D newPoints;
		for (Path2DIterator i = points.begin(); i != points.end(); i++)
		{
			Point2D point = *i;
			double qx = point.x - c.x;
			double qy = point.y - c.y;
			newPoints.push_back(Point2D(qx, qy));
		}
		return newPoints;
	}


        Path2D GeometricRecognizer::CombineStrokes(MultiStrokeGesture strokes)
        {
            Path2D points;
            for (int s = 0; s < strokes.size(); s++) {
                Path2D tmp=strokes.at(s);
                for (int p = 0; p < tmp.size(); p++) {
                    Point2D pt=tmp.at(p);
                    points.push_back(Point2D(pt.x, pt.y));
                }
            }
            return points;
        }
        //
        // Multistroke class: a container for unistroke templates
        //
        void GeometricRecognizer::loadMultistrokeTemplates() // constructor
        {
            SampleMultiStrokeGestures samplemultistrokegestures;
            addMultiStrokesTemplate("T", samplemultistrokegestures.getGestureT());
            addMultiStrokesTemplate("X", samplemultistrokegestures.getGestureX());
            addMultiStrokesTemplate("P", samplemultistrokegestures.getGestureP());
            addMultiStrokesTemplate("D", samplemultistrokegestures.getGestureD());
            addMultiStrokesTemplate("I", samplemultistrokegestures.getGestureI());
            addMultiStrokesTemplate("N", samplemultistrokegestures.getGestureN());
         }

        int GeometricRecognizer::addMultiStrokesTemplate(string name, MultiStrokeGesture paths)
        {
                allMtemplates.push_back(MultipleStrokeGestureTemplate(name, paths));
                //--- Let them know how many examples of this template we have now
                int numInstancesOfGesture = 0;
                // You know, i don't care so i'm just going to ignore this
                //for (var i = 0; i < templates.size(); i++)
                //{
                //	if (templates[i].Name == name)
                //		numInstancesOfGesture++;
                //}
                return numInstancesOfGesture;
        }

        void GeometricRecognizer::activateMultiStrokesTemplates(vector<string> list)
        {
            cout<< "No. of templates in Multi-Stroke database " <<allMtemplates.size()<<endl;
            for (unsigned int i=0; i<allMtemplates.size() ; i++)
            {

              //  cout<< "Processing Template " <<allMtemplates.at(i).name<<endl;
                if (inTemplates(allMtemplates.at(i).name, list))
                {
                  //  cout<<"Added Template :"<<allMtemplates.at(i).name<<endl;
                    Mtemplates.push_back(allMtemplates.at(i));
                    order.clear();
                    orders.clear();
                    MultipleStrokeGestureTemplate strokes = allMtemplates.at(i);
                    order.resize(strokes.paths.size());

                    for (int i = 0; i < strokes.paths.size(); i++)
                            order[i] = i; // initialize; // initialize

                    GeometricRecognizer::HeapPermute(strokes.paths.size());
                    MultiStrokeGesture unistrokes = GeometricRecognizer::MakeUnistrokes(strokes.paths); // returns array of point arrays
                    allunistrokenormalizedtemplates.clear();
                    for (int j = 0; j < unistrokes.size(); j++)
                        Path2D unistroke = GeometricRecognizer::UnistrokeTemplate(unistrokes.at(j),allMtemplates.at(i).name);
                    allmultistrokenormalizedgestures.push_back(allunistrokenormalizedtemplates);


                }


               }

        }
        //Perform permutations to make all the combination of multistroke gesture
        MultiStrokeGesture GeometricRecognizer::MakeUnistrokes(MultiStrokeGesture strokes)
        {
            MultiStrokeGesture unistrokes; // array of point arrays
            for (int r = 0; r < orders.size(); r++)
            {

                for (int b = 0; b < pow(2, orders.at(r).size()); b++) // use b's bits for directions
                {

                    Path2D unistroke; // array of points
                    for (int i = 0; i < orders.at(r).size(); i++)
                    {

                        Path2D pts;

                       if (((b >> i) & 1) == 1) {  // is b's bit at index i on?
                            // copy and reverse

                            Path2D tmp=strokes[orders[r][i]];

                            reverse(tmp.begin(),tmp.end());

                            pts = tmp;
                     } else {

                            Path2D tmp=strokes[orders[r][i]];

                            pts = tmp;

                             // copy
                        }
                        for (int p = 0; p < pts.size(); p++) {

                            unistroke.push_back(pts[p]); // append points

                        }

                    }

                    unistrokes.push_back(unistroke); // add one unistroke to set

                }
            }
            return unistrokes;
        }//


        //
        // Private helper functions from this point down
        //
        void GeometricRecognizer::HeapPermute(int n)
        {
                if (n == 1)
                {
                    orders.push_back(order);// append copy
                }
                else
                {
                        for (int i = 0; i < n; i++)
                        {
                                 GeometricRecognizer::HeapPermute(n - 1);
                                if (n % 2 == 1) // swap 0, n-1
                                {
                                        int tmp = order[0];
                                        order[0] = order[n - 1];
                                        order[n - 1] = tmp;
                                }
                                else // swap i, n-1
                                {
                                        int tmp = order[i];
                                        order[i] = order[n - 1];
                                        order[n - 1] = tmp;
                                }
                        }
                }
        }

Path2D GeometricRecognizer::UnistrokeTemplate(Path2D points,string name)
{
  points=normalizePath(points);
  Point2D startv=GeometricRecognizer::CalcStartUnitVector(points,StartAngleIndex);
  vector<double> Vector = vectorize(points);
  allunistrokenormalizedtemplates.push_back(GestureTemplate(name,points,startv,Vector));
  return points;
}

Point2D GeometricRecognizer::CalcStartUnitVector(Path2D points,double index) // start angle from points[0] to points[index] normalized as a unit vector
{
    Point2D p1 = points[index];
    Point2D p2 = points[0];
    Point2D v =Point2D(p1.x - p2.x, p1.y - p2.y);
    double len = sqrt(v.x * v.x + v.y * v.y);
    return Point2D(v.x / len, v.y / len);
}

vector<double> GeometricRecognizer::Vectorize(Path2D points,bool useBoundedRotationInvariance) // for Protractor
{
    double tcos = 1.0;
    double tsin = 0.0;
    if (useBoundedRotationInvariance)
    {
        double iAngle = atan2(points[0].y, points[0].x);
        double baseOrientation = (3.14 / 4.0) * floor((iAngle + 3.14 / 8.0) / (3.14 / 4.0));
        tcos = cos(baseOrientation - iAngle);
        tsin = sin(baseOrientation - iAngle);
    }
    double sum = 0.0;
    vector<double> Vector;
    for (int i = 0; i < points.size(); i++)
    {
        double newX = points[i].x * tcos - points[i].y * tsin;
        double newY = points[i].y * tcos + points[i].x * tsin;
        Vector.push_back(newX);
        Vector.push_back(newY);
        sum += newX * newX + newY * newY;
    }
    double magnitude = sqrt(sum);
    for (int i = 0; i < Vector.size(); i++)
        Vector.at(i) /= magnitude;
    return Vector;
}

double GeometricRecognizer::Round(double n,double d) {
    d = pow(10,d);
   return round(n*d)/d;
} // round 'n' to 'd' decimals

double GeometricRecognizer::Deg2Rad(double d)
{ return (d * 3.14 / 180.0); }

double GeometricRecognizer::Rad2Deg(double r)
{ return (r * 180.0 / 3.14); }

RecognitionResult GeometricRecognizer::Multirecognize(MultiStrokeGesture strokes,string method)
{
    bool useProtractor=false;
    if(method=="protractor"){
        cout<<"using protactor"<<endl;
         useProtractor=true;
    }


        Path2D points=CombineStrokes(strokes);
        //--- Make sure we have some templates to compare this to
        //---  or else recognition will be impossible
        if (allmultistrokenormalizedgestures.empty())
        {
                std::cout << "No templates loaded so no symbols to match." << std::endl;
                return RecognitionResult("Unknown", 0);
        }
        points=normalizePath(points);
        Point2D startv=GeometricRecognizer::CalcStartUnitVector(points,StartAngleIndex);
        vector<double> Vector = vectorize(points);

        //--- Initialize best distance to the largest possible number
        //--- That way everything will be better than that
        double bestDistance = MAX_DOUBLE;
        //--- We haven't found a good match yet
        int indexOfBestMatch = -1;
        double score = 0.0;
        bool requireSameNoOfStrokes = false;
        for (int i = 0; i < (int)allmultistrokenormalizedgestures.size(); i++) // for each multistroke
            {

                GestureTemplates Mgestures= allmultistrokenormalizedgestures.at(i);
                if (!requireSameNoOfStrokes || strokes.size() == Mgestures.size()) // optional -- only attempt match when same # of component strokes
                {
                    for (int j = 0; j < Mgestures.size(); j++) // each unistroke within this multistroke
                    {
                        GestureTemplate Mgesture=Mgestures.at(j);
                        if(AngleBetweenUnitVectors(startv,Mgesture.startv) <= AngleSimilarityThreshold) // strokes start in the same direction
                        {
                            double distance;
                            if (useProtractor) // for Protractor
                            {
                                cout<<"using protactor"<<endl;
                               distance = GeometricRecognizer::optimalCosineDistance(Vector, Mgesture.Vector);
                            }
                            else // Golden Section Search (original $N)
                            {
                                distance = GeometricRecognizer::distanceAtBestAngle(points, Mgesture);
                            }
                            if (distance < bestDistance)
                            {
                                    bestDistance     = distance;
                                    indexOfBestMatch = i;
                            }
                        }
                    }
                }
            }

        //--- Turn the distance into a percentage by dividing it by
        //---  half the maximum possible distance (across the diagonal
        //---  of the square we scaled everything too)
        //--- Distance = hwo different they are
        //--- Subtract that from 1 (100%) to get the similarity
        if (useProtractor)
            { score = bestDistance ? (1.0 / bestDistance) : MAX_DOUBLE ; }
        else
            score = 1.0 - (bestDistance / halfDiagonal);
        //cout << score << "   " << bestDistance<< "   " ;

    //--- Make sure we actually found a good match
    //--- Sometimes we don't, like when the user doesn't draw enough points
    if (-1 == indexOfBestMatch)
    {
            cout << "Couldn't find a good match." << endl;
            return RecognitionResult("Unknown", 1);
    }
    GestureTemplates tmp=allmultistrokenormalizedgestures[indexOfBestMatch];
    GestureTemplate tmp2=tmp.at(1);
    RecognitionResult bestMatch(tmp2.name, score);
    return bestMatch;

}

double GeometricRecognizer::AngleBetweenUnitVectors(Point2D v1,Point2D v2) // gives acute angle between unit vectors from (0,0) to v1, and (0,0) to v2
{

    double n = (v1.x * v2.x + v1.y * v2.y);
    if (n < -1.0 || n > +1.0){
        double d = pow(10,5);
      	n = round(n*d)/d;
      //  n = GeometricRecognizer::Round(n, 5);
    } // fix: JavaScript rounding bug that can occur so that -1 <= n <= +1
    return acos(n); // arc cosine of the vector dot product
}

}
