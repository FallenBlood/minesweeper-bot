/*
 * Russell Wiley & Drew Banin
 * Functions taking from CS283 Summer 2013 HW3
 */

#include "hw3.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include "Board.h"
#include "MouseClick.h"

//Only load blocks once since it will take a long time to do each time
CvScalar *bcolor;
int horiz,vert;
int width,height;
Board* board = new Board(16,31);
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
 * Splits up an image into numColumns by numRows sub-images and returns the results. 
 * 
 * @param src   The source image to split 
 * @param numColumns    The number of columns to split into 
 * @param numRows       The number of rows to split into 
 * @returns             A numColumns x numRows array of IplImages 
 */ 
IplImage ** getSubImages(IplImage* src, int numColumns, int numRows) { 
    int cellWidth, cellHeight, y, x, i; 
    IplImage ** rv; 
    CvSize s = cvGetSize(src); 

    // Compute the cell width and the cell height 
    cellWidth = s.width / numColumns; 
    cellHeight = s.height / numRows; 
    // Allocate an array of IplImage* s 
    rv = (IplImage**) malloc(sizeof(IplImage*) * numColumns * numRows); 
    if (rv == NULL) { 
        fprintf(stderr, "Could not allocate rv array\n"); 
    } 

    // Iterate over the cells 
    i = 0; 
    for (y = 0; y < s.height; y += cellHeight) 
        for (x = 0; x < s.width; x += cellWidth) { 
            // Create a new image of size cellWidth x cellHeight and 
            // store it in rv[i]. The depth and number of channels should 
            // be the same as src. 
            rv[i] = cvCreateImage(cvSize(cellWidth, cellHeight), src->depth, 
                    src->nChannels); 
            if (rv[i] == NULL) { 
                fprintf(stderr, "Could not allocate image %d\n", i); 
            } 

            // set the ROI of the src image 
            cvSetImageROI(src, cvRect(x, y, cellWidth, cellHeight)); 

            // copy src to rv[i] using cvCopy, which obeys ROI 
            cvCopy(src, rv[i], NULL); 

            // reset the src image roi 
            cvResetImageROI(src); 

            // increment i 
            i++; 
        } 
    // return the result 
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
    bcolor = getAvgColors(blocks,size);
}

/**
 ** This is the function to demonstrate the conversion XImage to IplImage
 ** @param _xImage the X image object
 ** @param _xDisplay the X display, for init see main() below
 ** @param _xScreen the X screen, for init see main() below
 ** @return
 **/
IplImage* XImage2OpenCVImage(XImage& _xImage, Display& _xDisplay, Screen& _xScreen) {
    XColor color;
    IplImage* ocvImage = cvCreateImage(cvSize(_xImage.width, _xImage.height), IPL_DEPTH_8U, 3);

    if (_xScreen.depths->depth == 24) {
        // Some of the following code is borrowed from http://www.roard.com/docs/cookbook/cbsu19.html
        // ("Screen grab with X11" - by Marko Riedel, with an idea by Alexander Malmberg)
        unsigned long rmask = _xScreen.root_visual->red_mask,
                      gmask = _xScreen.root_visual->green_mask,
                      bmask = _xScreen.root_visual->blue_mask;
        unsigned long rshift, rbits, gshift, gbits, bshift, bbits;
        unsigned char colorChannel[3];

        rshift = 0;
        rbits = 0;
        while (!(rmask & 1)) {
            rshift++;
            rmask >>= 1;
        }
        while (rmask & 1) {
            rbits++;
            rmask >>= 1;
        }
        if (rbits > 8) {
            rshift += rbits - 8;
            rbits = 8;
        }

        gshift = 0;
        gbits = 0;
        while (!(gmask & 1)) {
            gshift++;
            gmask >>= 1;
        }
        while (gmask & 1) {
            gbits++;
            gmask >>= 1;
        }
        if (gbits > 8) {
            gshift += gbits - 8;
            gbits = 8;
        }

        bshift = 0;
        bbits = 0;
        while (!(bmask & 1)) {
            bshift++;
            bmask >>= 1;
        }
        while (bmask & 1) {
            bbits++;
            bmask >>= 1;
        }
        if (bbits > 8) {
            bshift += bbits - 8;
            bbits = 8;
        }

        for (unsigned int x = 0; x < _xImage.width; x++) {
            for (unsigned int y = 0; y < _xImage.height; y++) {
                color.pixel = XGetPixel(&_xImage, x, y);
                colorChannel[0] = ((color.pixel >> bshift) & ((1 << bbits) - 1)) << (8 - bbits);
                colorChannel[1] = ((color.pixel >> gshift) & ((1 << gbits) - 1)) << (8 - gbits);
                colorChannel[2] = ((color.pixel >> rshift) & ((1 << rbits) - 1)) << (8 - rbits);
                CV_IMAGE_ELEM(ocvImage, uchar, y, x * ocvImage->nChannels) = colorChannel[0];
                CV_IMAGE_ELEM(ocvImage, uchar, y, x * ocvImage->nChannels + 1) = colorChannel[1];
                CV_IMAGE_ELEM(ocvImage, uchar, y, x * ocvImage->nChannels + 2) = colorChannel[2];
            }
        }
    } else { // Extremly slow alternative for non 24bit-depth
        Colormap colmap = DefaultColormap(&_xDisplay, DefaultScreen(&_xDisplay));
        for (unsigned int x = 0; x < _xImage.width; x++) {
            for (unsigned int y = 0; y < _xImage.height; y++) {
                color.pixel = XGetPixel(&_xImage, x, y);
                XQueryColor(&_xDisplay, colmap, &color);
                CV_IMAGE_ELEM(ocvImage, uchar, y, x * ocvImage->nChannels) = color.blue;
                CV_IMAGE_ELEM(ocvImage, uchar, y, x * ocvImage->nChannels + 1) = color.green;
                CV_IMAGE_ELEM(ocvImage, uchar, y, x * ocvImage->nChannels + 2) = color.red;
            }
        }
    }
    return ocvImage;
}

void setWindowSize() {
    Display* display = NULL;
    Screen* screen = NULL;
    XImage* xImageShot = NULL;
    IplImage* screenShot;
    XColor col;
    display = XOpenDisplay(NULL); // Open first (-best) display
    screen = DefaultScreenOfDisplay(display);
    int startX = 40, startY = 200; // The starting pixels / offset (x,y) to take the screenshot from
    unsigned int widthX = 1000 , heightY = 520; // The size of the area (width,height) to take a screenshot of
    xImageShot = XGetImage(display, DefaultRootWindow(display), startX, startY, widthX, heightY, AllPlanes, ZPixmap);
    // Check for bad null pointers
    if (xImageShot == NULL || display == NULL || screen == NULL) {
        printf("Error: Screen is null?\n");
        return;
    }
    screenShot = XImage2OpenCVImage(*xImageShot, *display, *screen);
    CvMat *mat = cvCreateMat(screenShot->height,screenShot->width,CV_32FC3);
    cvConvert(screenShot,mat);
    IplImage* whitePix = cvLoadImage("../tiles/zblank.png",CV_LOAD_IMAGE_UNCHANGED);
    CvMat *mat2 = cvCreateMat(whitePix->height,whitePix->width,CV_32FC3);
    cvConvert(whitePix,mat2);

    //Find first white pixel, indicating top left corner or first actual mine field
    int x,y;
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            CvScalar scal = cvGet2D(mat,i,j);
            CvScalar scal2 = cvGet2D(mat2,0,0);
            if(scal.val[0] == scal2.val[0] && scal.val[1] == scal2.val[1] && scal.val[2] == scal2.val[2])
            {
                x = i-4;
                y = j+1;
                i = 10;
                j = 10;
                break;
            }
        }
    }

    horiz = 40 + x;
    vert = 200 + y;
    width = 992;
    height = 512;
}
// X resources
IplImage* getState()
{

    Display* display = NULL;
    Screen* screen = NULL;
    XImage* xImageShot = NULL;
    IplImage* screenShot;
    XColor col;
    display = XOpenDisplay(NULL); // Open first (-best) display
    screen = DefaultScreenOfDisplay(display);
    xImageShot = XGetImage(display, DefaultRootWindow(display), 0, 0, 1200, 800, AllPlanes, ZPixmap);
    // Check for bad null pointers
    if (xImageShot != NULL && display != NULL && screen != NULL) {
        screenShot = XImage2OpenCVImage(*xImageShot, *display, *screen);
        cvSetImageROI(screenShot, cvRect(horiz,vert,width,height));
        IplImage *tmp = cvCreateImage(cvGetSize(screenShot),screenShot->depth,screenShot->nChannels);
        cvCopy(screenShot,tmp,NULL);
        cvResetImageROI(screenShot);
        screenShot = cvCloneImage(tmp);
        // Always clean up your mess
        XDestroyImage(xImageShot);
        XCloseDisplay(display);
    } else {
        printf ("Error taking screenshot!\n");
    }
    return screenShot;
}

void updateBoard(int numColors)
{
    IplImage* state = getState();
    IplImage** subImages = getSubImages(state,31,16);
    CvScalar* avgs = getAvgColors(subImages,31*16);
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 31; j++)
        {
            int index = findClosest(avgs[(31*i)+j],bcolor,numColors);
            board->setCell(i,j,index);
        }
    }
}


int main(int argc, char* argv[])
{
    int numImages = argc-1;
    startUp(numImages, argv+1);
    setWindowSize();
    mouseMove(horiz+(width/2),vert+(height/2),true);
    updateBoard(numImages);
    bool canDoSafe = true;
    while(canDoSafe)
    {
        for(int i = 0; i < 16; i++)
        {
            for(int j = 0; j < 31; j++)
            {
                if(board->getCell(i,j)->getVal() < 8)
                {
                    std::vector<Cell*> surround = board->getAdjacent(board->getCell(i,j));
                    int count = 0;
                    int count2 = 0;
                    for(std::vector<Cell*>::iterator it = surround.begin(); it!=surround.end(); ++it)
                    {
                        if((*it)->getVal() == 8)
                        {
                            count++;
                        }
                        else if((*it)->getVal() == 10)
                        {
                            count++;
                            count2++;
                        }
                    }
                    if(count == board->getCell(i,j)->getVal()+1)
                    {
                        for(std::vector<Cell*>::iterator it = surround.begin(); it!=surround.end(); ++it)
                        {
                            if((*it)->getVal() == 8)
                            {
                                mouseMove(horiz+((*it)->getCol()*32)+32,vert+((*it)->getRow()*32)+32,false);
                                (*it)->setVal(10);
                            }
                        }
                        updateBoard(numImages);
                    }
                    else if(count2 == board->getCell(i,j)->getVal()+1)
                    {
                        mouseMove(horiz+(j*32)+32,vert+(i*32)+32,true);
                    }
                }
            }
        }

        updateBoard(numImages);

        for(int i = 15; i >=0; i--)
        {
            for(int j = 30; j >= 0; j--)
            {
                if(board->getCell(i,j)->getVal() < 8)
                {
                    std::vector<Cell*> surround = board->getAdjacent(board->getCell(i,j));
                    int count = 0;
                    int count2 = 0;
                    for(std::vector<Cell*>::iterator it = surround.begin(); it!=surround.end(); ++it)
                    {
                        if((*it)->getVal() == 8)
                        {
                            count++;
                        }
                        else if((*it)->getVal() == 10)
                        {
                            count++;
                            count2++;
                        }
                    }
                    if(count == board->getCell(i,j)->getVal()+1)
                    {
                        for(std::vector<Cell*>::iterator it = surround.begin(); it!=surround.end(); ++it)
                        {
                            if((*it)->getVal() == 8)
                            {
                                mouseMove(horiz+((*it)->getCol()*32)+32,vert+((*it)->getRow()*32)+32,false);
                                (*it)->setVal(10);
                            }
                        }
                        updateBoard(numImages);
                    }
                    else if(count2 == board->getCell(i,j)->getVal()+1)
                    {
                        mouseMove(horiz+(j*32)+32,vert+(i*32)+32,true);
                    }
                }
            }
        }

        updateBoard(numImages);
    }
}            


