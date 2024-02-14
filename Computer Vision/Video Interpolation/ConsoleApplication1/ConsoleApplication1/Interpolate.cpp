#include "Interpolate.h"

void interpolateSimpleContour(const Contour& A, const Contour& B, Contour& R, const float t)
{

}

void interpolateContours(const Area& A, const Area& B, Area& IC, const float t)
{
	/*vector<int> pairsA_B = identifyContours(A.innerBounds, B.innerBounds);

	int i = 0;
	for (auto& innerA : A.innerBounds)
	{
		if (pairsA_B[i] != NIL_VAL)
		{
			Contour ICH;
			interpolateSimpleContour(innerA, B.innerBounds[pairsA_B[i]], ICH, t);
			IC.innerBounds.push_back(ICH);
			i++;
		}
	} */

	Contour ICC;
	interpolateSimpleContour(A.contour, B.contour, ICC, t);
	IC.contour = ICC;
}

void interpolateAreas(const Area& A, const Area& B, Mat& frame, const float t)
{

}