#include "Identify.h"
#include "Decompose.h"

vector<int> identifyContours(const vector<Contour>& A, const vector<Contour>& B)
{
	vector<int> pairsA_B(A.size(), NIL_VAL); // corespondenta pozitie in A <-> pozitie in B

	// Create a vector pair with ((a, b), delta(a, b)) and sort by delta
	vector<Pair<Pair<int, int>, double>> deltas;

	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < B.size(); j++)
		{
			Pair<int, int> p(i, j);
			Pair<Pair<int, int>, double> P(p, Utils::delta(A[i], B[j]));
			deltas.push_back(P);
		}
	}

	sort(deltas.begin(), deltas.end(), Utils::compareDeltaPairs);

	size_t size = (A.size() < B.size()) ? A.size() : B.size();
	unordered_set<int> takenA, takenB;

	int i = 0;
	int count = 0;
	while (count < size)
	{
		if (takenA.find(deltas[i].first.first) != takenA.end()
			|| takenB.find(deltas[i].first.second) != takenB.end())
		{
			i++;
			continue;
		}
		pairsA_B[deltas[i].first.first] = deltas[i].first.second;
		takenA.insert(deltas[i].first.first);
		takenB.insert(deltas[i].first.second);
		i++;
		count++;
	}

	return pairsA_B;
}

vector<int> identify(const Mat& img, const vector<Area>& A, const vector<Area>& B, const int K, const bool largeImage)
{
    vector<int> pairsA_B(A.size(), NIL_VAL); // corespondenta pozitie in A <-> pozitie in B
    vector<Pair<Area, Area>> pairs;
    if (A.empty() || B.empty()) {
        return pairsA_B;
    }

    // Create a vector pair with ((a, b), delta(a, b)) and sort by delta
   // vector<Pair<Pair<int, int>, double>> deltas;
    vector< Pair<Pair<int, int>, double>> deltas;
    size_t sizeA = 0;
    size_t sizeB = 0;
    if (K == 0) {
        for (int i = 0; i < A.size(); i++)
        {
            Rect rA = Utils::rectanglefy(A[i].contour);
            if (rA.width <= 5 && rA.height <= 5 && largeImage) {
                continue;
            }
            size_t s = 0;
            for (int j = 0; j < B.size(); j++)
            {
                Rect rB = Utils::rectanglefy(B[i].contour);
                if (rB.width <= 5 && rB.height <= 5 && largeImage) {
                    continue;
                }
                Pair<int, int> p(i, j);
                Pair<Pair<int, int>, double> P(p, Utils::delta(A[i], B[j]));
                deltas.push_back(P);
                s++;
            }
            sizeA++;
            sizeB = s;
        }
    }
    else {
        vector<vector<Area>> catA = organize(img, A, K);
        vector<vector<Area>> catB = organize(img, B, K);
        Pair<Area, Area> p(A[0], A[0]);
        Pair<Pair<Area, Area>, double> P(p, 0);
        for (int i = 0; i < catA.size(); i++) {
            for (auto& areaA : catA[i]) {
                for (auto& areaB : catB[i]) {
                    p.first = areaA;
                    p.second = areaB;
                    P.first = p;
                    P.second = Utils::delta(areaA, areaB);
                    //sortedPairs.insert(P);
                }
            }
        }
        return pairsA_B;
    }

   /* for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < B.size() / 100; j++) {
            Pair<int, int> p(i, j);
            Pair<Pair<int, int>, double> P(p, Utils::delta(A[i], B[j]));
            deltas.insert(P);
        }
    } */

    sort(deltas.begin(), deltas.end(), Utils::compareDeltaPairs);

    size_t size = (sizeA < sizeB) ? sizeA : sizeB;
    unordered_set<int> takenA, takenB;

    int i = 0;
    int count = 0;
    while (count < size)
    {
        if (takenA.find(deltas[i].first.first) != takenA.end()
            || takenB.find(deltas[i].first.second) != takenB.end())
        {
            i++;
            continue;
        }
        pairsA_B[deltas[i].first.first] = deltas[i].first.second;
        takenA.insert(deltas[i].first.first);
        takenB.insert(deltas[i].first.second);
        i++;
        count++;
    }

    return pairsA_B;
}

