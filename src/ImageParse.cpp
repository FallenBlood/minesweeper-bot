/**
 * Russell Wiley & Drew Banin
 * Functions taking from CS283 Summer 2013 HW3
 */

#include "hw3.h"

//Only load blocks once since it will take a long time to do each time
CvScalar *bcolor;

/**
 * Computes the distance between two colors (stored as CvScalars).
 *
 * Given a CvScalar c, you can access the blue, green, and red (BGR) elements
 * via c.val[0], c.val[1], and c.val[2], respectively.
 *
 * This function computes the distance between two colors as the euclidean
 * distance between the two BGR vectors.
 *
 * @see http://en.wikipedia.org/wiki/Euclidean_distance
 *
 * @param c1    The first color
 * @param c2    The second color
 * @returns     The euclidean distance between the two 3-d vectors
 */
double colorDistance(CvScalar c1, CvScalar c2) {
    double d = 0; // the result
    int i; // an iterator

    for(i = 0; i < 3; i++)
    {
        d += pow((c1.val[i] - c2.val[i]), 2.0);
    }
    // If d is zero, just return 0.
    if(d == 0.0)
        return 0;
    else
        return sqrt(d);
}

IplImage** loadImages(int numImages, char ** fileNames) {
	IplImage** rv; // the return result
	int i; // used for looping
    rv = (IplImage** ) malloc(numImages*sizeof(IplImage*));

    for(i = 0; i < numImages; i++)
    {
        rv[i] = cvLoadImage(fileNames[i],CV_LOAD_IMAGE_UNCHANGED);
        if(rv[i] == NULL)
        {
            printf("Error loading %s",fileNames[i]);
            return NULL;
        }
	}

    return rv;
}

/**
 * Finds the CvScalar in colors closest to t using the colorDistance function.
 * @param t         The color to look for
 * @param scolors   The colors to look through
 * @param numColors The length of scolors
 * @returns         The index of scolors that t is closest to
 *                  (i.e., colorDistance( t, scolors[result]) <=
 *                  colorDistance( t, scolors[i]) for all i != result)
 */
int findClosest(CvScalar t, CvScalar* scolors, int numColors) {
    int rv = 0, // return value
         i; // used to iterate
    double d, // stores the result of distance
           m = colorDistance(t, scolors[0]); // the current minmum distance
    for(i = 1; i < numColors; i++)
    {
        d = colorDistance(t,scolors[i]);
        if(d < m)
        {
            rv = i;
            m = d;
        }
    }
    return rv;
}

/**
 * For each image provided, computes the average color vector
 * (represented as a CvScalar object).
 *
 * @param images    The images
 * @param numImages The length of images
 * @returns         An numImages length array of CvScalars were rv[i] is the average color in images[i]
 */
CvScalar* getAvgColors(IplImage** images, int numImages) {
    CvScalar* rv;
    int i;
    rv = (CvScalar*) malloc(numImages * sizeof(CvScalar));
    for(i = 0; i < numImages; i++)
    {
        rv[i] = cvAvg(images[i],NULL);
    }

    return rv;
}

void startUp(int size, char* args[])
{
    IplImage **blocks = loadImages(size, args);
}

int main(int argc, char* argv[])
{
    int numImages = argc-2;
    startUp(numImages, argv+2);
    //Get average color of passed image
//    CvScalar rv;
 //   rv = cvAvg(cvLoadImage(argv[1],CV_LOAD_IMAGE_UNCHANGED),NULL);
   // int ci = findClosest(rv, bcolor, 10);
    printf("Closest is %d\n",1);
}
