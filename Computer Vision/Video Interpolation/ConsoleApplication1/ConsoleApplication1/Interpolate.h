#pragma once

#include "Constants.h"
#include "Utils.h"

void interpolateSimpleContour(const Contour& A, const Contour& B, Contour& R, const float t);
void interpolateContours(const Area& A, const Area& B, Area& IC, const float t);
void interpolateAreas(const Area& A, const Area& B, Mat& frame, const float t);


// THE BETTER WAY
// 1. Interpolate A and B contours, and get IC
// 2. Check all pixels inside IC and if the current pixel is undrawn(black)
//		a) if the pixel is inside area B -> draw the pixel of B on the frame
//		b) else if the pixel is inside area A -> draw the pixel of A on the frame
//		c) else draw average color



// 1. Interpolate inner contours of A and B and get all interpolated contour holes(ICH)
// 2. Interpolate A and B main contours and get interpolated contour(IC)
// 3. Check all pixels inside IC / ICHs
//		a) if the pixel is inside area B -> draw the pixel of B on the frame
//		b) else if the pixel is inside area A -> draw the pixel of A on the frame
//		c) else draw nothing