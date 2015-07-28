#include <opencv2/highgui/highgui.hpp>

#include "WebcamCapture.h"
#include "ProcessingThread.h"
#include "TSDataHandler.h"

int main(int argc, char* argv[])
{
  // инициализация очереди на обработку изображения
  TSDataHandler<Mat>* cap2proc = new TSDataHandler<Mat>();
  // инициализация очереди для вывода 
  TSDataHandler<Point2f>* proc2out = new TSDataHandler<Point2f>();
  TSDataHandler<Mat>* dbg_outputImage = new TSDataHandler<Mat>();
  cv::Mat img;

  // инициализация и старт потоков считывания и обработки данных
  WebcamCapture capThread(cap2proc, 0);
  ProcessingThread procThread(cap2proc, proc2out, dbg_outputImage);
  capThread.start();
  procThread.start();

  // цикл вывода обработанных изображений
  forever
  {
    Point2f out;
    if (proc2out->Read(out) && dbg_outputImage->Read(img))
    {
      cout << out << endl;
      imshow("OUTPUT", img);
      waitKey(1);
    }
  }

    // закрытие потоков
  capThread.exit();
  procThread.exit();
  return 0;
}