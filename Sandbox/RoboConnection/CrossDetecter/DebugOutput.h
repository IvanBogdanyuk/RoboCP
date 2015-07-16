#pragma once

// отладочный вывод изображений
#ifdef DEBUG_IMAGE_OUTPUT
#define OUTPUT_IMG_VAR __output
#define DBG_InitOutputImage() Mat OUTPUT_IMG_VAR
#define DBG_CreateOutputFromImage(img) OUTPUT_IMG_VAR = img.clone()
#define DBG_DrawOutputCircle(pt) circle(OUTPUT_IMG_VAR, pt, 5, Scalar(255))
#define DBG_DrawOutputLine(pt_begin, pt_end) line(OUTPUT_IMG_VAR, pt_begin, pt_end, Scalar(255))
#define DBG_DrawOutputCircles(pts) for(int i=0; i<pts.size(); i++) DBG_DrawOutputCircle(pts[i])
#define DBG_WriteFrame(dh) dh->Write(OUTPUT_IMG_VAR)
#define DBG_DrawOutputText(text, coords) putText(OUTPUT_IMG_VAR, text, coords, FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0), 1.0)
#else
#define DBG_InitOutputImage()
#define DBG_CreateOutputFromImage(img)
#define DBG_DrawOutputCircle(pt)
#define DBG_DrawOutputCircles(pts)
#define DBG_WriteFrame(dh)
#define DBG_DrawOutputLine(pt_begin, pt_end)
#define DBG_DrawOutputText(text, coords)
#endif