#include "ImageFlowProcessing.h"


ImageFlowProcessing::ImageFlowProcessing(void)
{
}

void ImageFlowProcessing::Start(){

}

void ImageFlowProcessing::CountDisplacement(IplImage *Img1, IplImage *Img2, DisplacementImages *Displacement)
{
	if(Img1 == 0 || Img2 == 0){
		return;
	}
	  
	CvSize img_sz;
	img_sz.height = Img1->height;
	img_sz.width = Img1->width;
	 
	 IplImage * eig_image = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );
	IplImage * tmp_image = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );
	IplImage * imgA = cvCreateImage( img_sz, IPL_DEPTH_8U, 1);  
	IplImage * imgB = cvCreateImage( img_sz, IPL_DEPTH_8U, 1);      
	
	cvCvtColor(Img1, imgA, CV_BGR2GRAY);  
	cvCvtColor(Img2, imgB, CV_BGR2GRAY);     
	
	int corner_count = MAX_COUNT;  
	CvPoint2D32f * cornersA = new CvPoint2D32f[ MAX_COUNT ];  
	CvPoint2D32f * cornersB = new CvPoint2D32f[ MAX_COUNT ];

	int win_size=20;  
	
	char features_found[ MAX_COUNT ];  
	float feature_errors[ MAX_COUNT ];  
	
	cvGoodFeaturesToTrack(imgA, eig_image, tmp_image, cornersA, &corner_count, 0.01, 5.0, 0, 3, 0, 0.04);  
	cvFindCornerSubPix(imgA, cornersA, corner_count, cvSize(win_size, win_size), cvSize(-1, -1), cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03));     
	CvSize pyr_sz = cvSize( imgA->width+8, imgB->height/3 );  
	
	IplImage * pyrA = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1);  
	IplImage * pyrB = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1);  
	
	cvCalcOpticalFlowPyrLK( imgA, imgB, pyrA, pyrB, cornersA, cornersB, corner_count, cvSize(win_size, win_size), 5, features_found, feature_errors, cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3), 0);  
	
	int numVectors = 0;
	for(int i=0; i<corner_count; i++)
	{
		if (features_found[i] == 0) 
			continue;
		Displacement->Vectors[numVectors].BeginningX = cornersA[i].x;
		Displacement->Vectors[numVectors].BeginningY = cornersA[i].y;
		Displacement->Vectors[numVectors].EndX = cornersB[i].x;
		Displacement->Vectors[numVectors].EndY = cornersB[i].y;
		Displacement->Vectors[numVectors].Length = Displacement->Dist(cornersA[i], cornersB[i]);
		numVectors++;
	}
	Displacement->NumVectors = numVectors;

	cvReleaseImage(&imgA);  
	cvReleaseImage(&imgB);   
	delete cornersA;  
	delete cornersB;   
	cvReleaseImage(&pyrA);  
	cvReleaseImage(&pyrB); 
}


//ShowOpticalFlow  (this metod is not necessary, but we could used it to debug)
void ImageFlowProcessing::ShowOpticalFlow(CvCapture *Capture)
{
	if(Capture == NULL){
		printf("Capture doesn't exist =(\n");
		return;
	} 
  IplImage* image = 0;
	IplImage* current_Img = 0;  
	IplImage* Old_Img = 0;  

	IplImage * imgA=0;  
	IplImage * imgB=0;  
     
	CvSize img_sz;
	img_sz.height = (int) cvGetCaptureProperty( Capture, CV_CAP_PROP_FRAME_HEIGHT );
	img_sz.width = (int) cvGetCaptureProperty( Capture, CV_CAP_PROP_FRAME_WIDTH );

	IplImage *eig_image = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );
	IplImage *tmp_image = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );

	int corner_count = MAX_COUNT;  
	CvPoint2D32f *cornersA = new CvPoint2D32f[ MAX_COUNT ];  
	CvPoint2D32f *cornersB = new CvPoint2D32f[ MAX_COUNT ];  

	int win_size=20;  
	IplImage* pyrA=0;  
	IplImage* pyrB=0;  
  
	char features_found[ MAX_COUNT ];  
	float feature_errors[ MAX_COUNT ]; 

  char Filename[64];
  /*CvFont myFont;
  cvInitFont(&myFont, CV_FONT_HERSHEY_COMPLEX_SMALL, 1, 0.5, 0, 1, 8);*/

	int current_frame = 0;
	
	while(1)
	{
		if(cvGrabFrame(Capture) == 0)  break; 
		if(Old_Img == 0)
		{
			image = cvQueryFrame(Capture);  
			if(image == NULL)  break;
			current_Img = cvCreateImage(cvSize(image->width, image->height), image->depth, image->nChannels);     
			Old_Img  = cvCreateImage(cvSize(image->width, image->height), image->depth, image->nChannels);
		}
		
    memcpy(Old_Img->imageData, current_Img->imageData, sizeof(char)*image->imageSize );  
		image = cvQueryFrame(Capture);
    if(image == NULL)  break;
		memcpy(current_Img->imageData, image->imageData, sizeof(char)*image->imageSize );  
		
    if(imgA == 0)  
		{  
			imgA = cvCreateImage( cvSize(image->width, image->height), IPL_DEPTH_8U, 1);  
			imgB = cvCreateImage( cvSize(image->width, image->height), IPL_DEPTH_8U, 1);      
		}
		  
		cvCvtColor(current_Img, imgA, CV_BGR2GRAY);  
		cvCvtColor(Old_Img, imgB, CV_BGR2GRAY);     
		
    cvGoodFeaturesToTrack(imgA, eig_image, tmp_image, cornersA, &corner_count, 0.01, 5.0, 0, 3, 0, 0.04);  
    cvFindCornerSubPix(imgA, cornersA, corner_count, cvSize(win_size, win_size), cvSize(-1, -1), cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03));     
    CvSize pyr_sz = cvSize( imgA->width+8, imgB->height/3 );

		if( pyrA == 0)  
		{   
			pyrA = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1);  
			pyrB = cvCreateImage( pyr_sz, IPL_DEPTH_32F, 1);  
		}  
		cvCalcOpticalFlowPyrLK( imgA, imgB, pyrA, pyrB, cornersA, cornersB, corner_count, cvSize(win_size, win_size), 5, features_found, feature_errors, cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3), 0);  

		float MaxLength = 0, MaxAngle = 0;
		for(int i=0; i<corner_count; i++){
			if (features_found[i] == 0) continue;
			
			float Length = sqrt((float)((cornersA[i].x-cornersB[i].x)*(cornersA[i].x-cornersB[i].x)+(cornersA[i].y-cornersB[i].y)*(cornersA[i].y-cornersB[i].y)));  
			float Angle  = fabs( atan2((float)(cornersB[i].y-cornersA[i].y), (float)(cornersB[i].x-cornersA[i].x)) * 180/PI );
			
			cvLine( image, cvPoint(cornersA[i].x, cornersA[i].y), cvPoint(cornersB[i].x, cornersB[i].y), CV_RGB(0,255,0),1);
			
			if(MaxLength < Length)  MaxLength = Length;
			if(MaxAngle < Angle)  MaxAngle = Angle;
		}
		/*sprintf(Filename,"Frame: %d",current_frame);
		cvPutText(image, Filename, cvPoint(20, 180), &myFont, CV_RGB(128,0,128));
    sprintf(Filename,"MaxAngle: %.2lf",MaxAngle);  
		cvPutText(image, Filename, cvPoint(20, 200), &myFont, CV_RGB(128,0,128));
    sprintf(Filename,"MaxLength: %.2lf",MaxLength);  
		cvPutText(image, Filename, cvPoint(20, 220), &myFont, CV_RGB(128,0,128));*/
    sprintf(Filename,"Image%d.jpg", current_frame);
    cvSaveImage(Filename,image);
    current_frame++;
	}
	cvReleaseCapture(&Capture);   
	cvReleaseImage(&Old_Img);
	cvReleaseImage(&imgA);
	cvReleaseImage(&imgB);
  delete cornersA;
  delete cornersB;
	cvReleaseImage(&pyrA);
  cvReleaseImage(&pyrB);
}


ImageFlowProcessing::~ImageFlowProcessing(void)
{
}

