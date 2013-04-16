#include "ImageFlowProcessing.h"


ImageFlowProcessing::ImageFlowProcessing(void)
{
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
	Flow * corners = new Flow[ MAX_COUNT ];  

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
		corners[i].Point = cornersA[i];
		corners[i].Vector.x = cornersB[i].x-cornersA[i].x;
		corners[i].Vector.y = cornersB[i].y-cornersA[i].y;
		Displacement->Vectors[numVectors].Beginning = cornersA[i];
		Displacement->Vectors[numVectors].End = cornersB[i];
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


//ShowOpticalFlow  (this metod is not necessary, but we could yoused it to debug)
void ImageFlowProcessing::ShowOpticalFlow(CvCapture * capture)
{
	if(capture == NULL){
		printf("Video doesn't exist =(\n");
		return;
	}
	IplImage* image = 0;  
	IplImage* current_Img = 0;  
	IplImage* Old_Img = 0;  
  
	//Optical Image  
	IplImage * imgA=0;  
	IplImage * imgB=0;  
     
	//Video Load  	
	CvSize img_sz;
	img_sz.height = (int) cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT );
	img_sz.width = (int) cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH );
	
	//Window  
	cvNamedWindow( "Origin" );  
     
	//Optical Flow Variables   
	IplImage *eig_image = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );
	IplImage *tmp_image = cvCreateImage( img_sz, IPL_DEPTH_32F, 1 );

	int corner_count = MAX_COUNT;  
	CvPoint2D32f* cornersA = new CvPoint2D32f[ MAX_COUNT ];  
	CvPoint2D32f * cornersB = new CvPoint2D32f[ MAX_COUNT ];  

	int win_size=20;  
	IplImage* pyrA=0;  
	IplImage* pyrB=0;  
  
	char features_found[ MAX_COUNT ];  
	float feature_errors[ MAX_COUNT ];  

	int current_frame = 0;
	
	//Routine Start
	while(1)
	{     
		if( cvGrabFrame( capture ) == 0 )  
			break; 

		if(Old_Img == 0)
		{
			image = cvQueryFrame( capture );  
			if(image == NULL)
				break;
			current_Img = cvCreateImage(cvSize(image->width, image->height), image->depth, image->nChannels);     
			Old_Img  = cvCreateImage(cvSize(image->width, image->height), image->depth, image->nChannels);
		}

		//copy to image class  
		memcpy(Old_Img->imageData, current_Img->imageData, sizeof(char)*image->imageSize );  
		image = cvQueryFrame( capture );  
		if(image == NULL)
				break;
		memcpy(current_Img->imageData, image->imageData, sizeof(char)*image->imageSize );  
		//Create image for Optical flow  
		if(imgA == 0)  
		{  
			imgA = cvCreateImage( cvSize(image->width, image->height), IPL_DEPTH_8U, 1);  
			imgB = cvCreateImage( cvSize(image->width, image->height), IPL_DEPTH_8U, 1);      
		}
		  
		//RGB to Gray for Optical Flow  
		cvCvtColor(current_Img, imgA, CV_BGR2GRAY);  
		cvCvtColor(Old_Img, imgB, CV_BGR2GRAY);     
		//  
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
			if (features_found[i] == 0) 
				continue;
			float Length = sqrt((float)((cornersA[i].x-cornersB[i].x)*(cornersA[i].x-cornersB[i].x)+(cornersA[i].y-cornersB[i].y)*(cornersA[i].y-cornersB[i].y)));  
			float Angle  = fabs( atan2((float)(cornersB[i].y-cornersA[i].y), (float)(cornersB[i].x-cornersA[i].x)) * 180/PI );  
  
			cvLine( image, cvPoint(cornersA[i].x, cornersA[i].y), cvPoint(cornersB[i].x, cornersA[i].y), CV_RGB(255, 0, 0), 2);    
			
			if(MaxLength < Length)
				MaxLength = Length;
			if(MaxAngle < Angle)
					MaxAngle = Angle;
		}  
		printf("[%d] - MaxAngle:%lf, MaxLength:%lf\n",current_frame , MaxAngle, MaxLength);  
		cvShowImage( "Origin", image );  
	
		int key_pressed;
		key_pressed = cvWaitKey(0);
		current_frame++;
	}     
  
	cvReleaseCapture(&capture);  
	cvDestroyWindow( "Origin" );     
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

