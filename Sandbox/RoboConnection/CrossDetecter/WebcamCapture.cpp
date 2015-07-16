#include "WebcamCapture.h"

void WebcamCapture::run()
{
	Mat orig;

	// макрос для создания таймера
	TimerCreate();
	while (isRunning())
	{
		TimerUpdate();
		vector<unsigned char> m_status;
		vector<float>         m_error;
		vector<Point2f> trackedPts;

		// если камера инициализирована и можно считать
		// изображение, то добавляем в очередь на обработку
		if (mVideoCapture->isOpened())
		{
			if (mVideoCapture->read(orig))
				mDataHandler_out->Write(orig);
		}
		else
		{
			this->exit(-1);
			
		}
		
		TimerElapsed();
	}
	// освобождение ресурсов камеры
	mVideoCapture->release();
	this->exit(0);
}

WebcamCapture::WebcamCapture(TSDataHandler<Mat> *dh_out, int cameraNum)
{
	// инициализация камеры стандартной камеры 
	// с размером выходного изображения 320x240
	this->mVideoCapture = new VideoCapture(cameraNum);
	if (!mVideoCapture->isOpened())
		exit(-1);
	//bool success = mVideoCapture->open(cameraNum);
	mVideoCapture->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	mVideoCapture->set(CV_CAP_PROP_FRAME_WIDTH, 320);
	this->mDataHandler_out = dh_out;
}

WebcamCapture::~WebcamCapture()
{
	mVideoCapture->release();
}
