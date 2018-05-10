#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>


using namespace std;
using namespace cv; 

class detectAndDraw
{
 public:
	detectAndDraw();
	detectAndDraw(Mat& img1, CascadeClassifier& cascade1,
                    CascadeClassifier& nestedCascade1,
                    double scale1, bool tryflip1);

	/*void init( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip );*/

};


