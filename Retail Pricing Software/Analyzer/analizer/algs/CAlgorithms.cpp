/*
 * CAlgorithms.cpp
 *
 *  Created on: Jul 11, 2019
 *      Author: kraychik
 */

#include "CAlgorithms.h"
#include "../../supply/CSmartException.h"
#include "../structures/CSupplementMatrix.h"
#include <algorithm>
#include <numeric>
#include <set>
#include <cmath>
#include <chrono>
#include <iostream>
#include <ctime>

CAlgorithms::CAlgorithms() {
	// TODO Auto-generated constructor stub

}

CAlgorithms::~CAlgorithms() {
	// TODO Auto-generated destructor stub
}

void CAlgorithms::OptimizeFunctionManual(function<double(const vector<double> & )> f, const vector<double> & aVect, const vector<double>& bVect, vector<double>& x) {
	const double acc = 0.1;
	const int n = (int)aVect.size();
	if ((int)bVect.size() != n) {
		throw CSmartException("CAlgorithms::OptimizeFunction: bVect size not equal to n");
	}
	//if ((int)x.size() != n) {
	//	throw CSmartException("CAlgorithms::OptimizeFunction: x size not equal to n");
	//}
	for (int i = 0; i < n; ++i) {
		if (aVect[i] > bVect[i]) {
			throw CSmartException("CAlgorithms::OptimizeFunction: aVect is more than bVect");
		}
	}
	vector<shared_ptr<vector<double>>> pts;	// множество рассматриваемых точек
	vector<double> ls(n, 0.0);	// полу-длины отрезков
	auto initialPt = make_shared<vector<double>>(n, 0);
	for (int i = 0; i < n; ++i) {
		initialPt->at(i) = (aVect[i] + bVect[i]) / 2;
		ls[i] = (bVect[i] - aVect[i]) / 2;
	}
	pts.push_back(initialPt);

	const vector<int> maxPts = {1000, 100, 100, 100};
	const vector<double> nTops = {10, 10, 10, 10};
	const int numberOfIters = (int)maxPts.size();
	vector<shared_ptr<vector<double>>> newPts;
	for (int j = 0; j < numberOfIters; ++j) {
		//cout << "Iterating over j = " << j << endl;
		newPts.clear();
		const int maxIter = maxPts[j];	// не более maxIter точек в данной итерации
		const int nTop = nTops[j];
		newPts.reserve(maxIter*nTop);
		//const double topPer = percents[j];	// после генерации отбираем topPer лучших точек
		//const int amtIters = floor(pow(maxIter, 1.0/n));
		//const int nTop = (int)maxIter * topPer;		// количество наилучших точек


		// далее генерируем точки в окрестности точки pts[i]
		//cout << "Generating points for j = " << j << "; size = " << pts.size() << endl;
		map<double, shared_ptr<vector<double>>> mapPointValue;
		for (int i = 0; i < (int)pts.size(); ++i) {
			vector<double> a(n, 0), b(n, 0);
			const auto curPoint = pts[i];
			for (int k = 0; k < n; k++) {
				a[k] = max(curPoint->at(k) - ls[i], aVect[k]);
				b[k] = min(curPoint->at(k) + ls[i], bVect[k]);
			}
			vector<shared_ptr<vector<double>>> newPoints;
			GenRandomGridPoints(a, b, maxIter, newPoints);
			for (int k = 0; k < (int)newPoints.size(); k++) {
				//const auto randPoint = RandomDoubleVector(a, b);
				const auto randPoint = newPoints[k];
				const double localValue = f(*randPoint);
				//cout << "(" << randPoint->at(0) << ", " << randPoint->at(1) << ", " << localValue << ") ";
				mapPointValue.insert(make_pair(localValue, randPoint));
			}
			//cout << std::chrono::system_clock::now() << endl;
		}
		//cout << endl;
		// далее обираем в массив newPts nTop лучших точек
		int k = 0;
		for (auto it = mapPointValue.rbegin(); it != mapPointValue.rend(); it++) {
			if (k++ >= nTop) break;
			const auto curPoint = it->second;
			//const double value = it->first;
			//cout << value << " ";
			newPts.push_back(curPoint);
		}
		//cout << endl;
		//cout << "Finishing for j = " << j << endl;
		pts.clear();
		pts.insert(pts.begin(), newPts.begin(), newPts.end());
		const double devisor = pow(maxIter, 1.0/n) / 4;
		//const double devisor = 2.0;
		for (int i = 0; i < n; ++i) {
			ls[i] /= devisor;
		}
	}
	// поиск наилучшего решения среди pts
	const int m = (int)pts.size();
	vector<double> vals(m, 0.0);
	for (int i = 0; i < m; ++i) {
		vals[i] = f(*pts[i]);
	}
	const auto it = max_element(vals.begin(), vals.end());
	const int index = distance(vals.begin(), it);
	x.clear();
	for(int i = 0; i < n; i++) {
		x.push_back(pts[index]->at(i));
	}
}
int CAlgorithms::Random(int from, int to) {
	return (rand() % (to-from+1)) + from;
}

void CAlgorithms::GenRandomVector(vector<int>& arr) {
	iota(arr.begin(), arr.end(), 0);
	const int n = arr.size();
	for (int k = 0; k < n; ++k) {
		int i = Random(0, n-1);
		int j = Random(0, n-1);
		int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
	}
}

double CAlgorithms::RandomDouble(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void CAlgorithms::FillRandVector(const vector<double>& a, const vector<double>& b, vector<double>& x) {
	const int n = (int) a.size();
	x.clear();
	x.reserve(n);
	for (int i = 0; i < n; ++i) {
		x[i] = RandomDouble(a[i], b[i]);
	}
}

shared_ptr<vector<double> > CAlgorithms::RandomDoubleVector(const vector<double>& a, const vector<double>& b) {
	const int n = (int) a.size();
	auto x = make_shared<vector<double>>(n, 0);
	for (int i = 0; i < n; ++i) {
		x->at(i) = RandomDouble(a[i], b[i]);
	}
	return x;
}

void CAlgorithms::LeastSquares(const map<double, double>& dataSample, const std::function<double(double, const vector<double> &)> Model, const vector<double> & aVect, const vector<double> & bVect, vector<double>& theta) {
	std::function<double(const vector<double> &)> targetFunc = [dataSample, Model](const vector<double> & pars) {
		double s = 0.0;
		for (const auto & pair : dataSample) {
			const double x = pair.first;
			const double exValue = Model(x, pars);
			const double y = pair.second;
			s += (y - exValue) * (y - exValue);
		}
		return -s;
	};
	OptimizeFunctionManual(targetFunc, aVect, bVect, theta);
}

double CAlgorithms::GenRandValue(const vector<double>& values, const vector<double>& probs) {
	const int maxInt = 10000;
	const int n = probs.size();
	vector<double> cumProbs(n, 0);
	double cumS = 0.0;
	for (int i = 0; i < n; ++i) {
		cumS += probs[i];
		cumProbs[i] = cumS;
	}
	int randNumber = rand() % maxInt;
	if (randNumber < cumProbs[0] * maxInt) return values[0];
	for (int i = 1; i < n; ++i) {
		if (randNumber < cumProbs[i] * maxInt) return values[i];
	}
	// следующие строчки никогда не должны исполниться
	return values[n-1];
}

void CAlgorithms::GenRandomGridPoints(const vector<double>& a, const vector<double>& b, int maxIters, vector<shared_ptr<vector<double> > >& newPts) {
	const int n = (int)a.size();
	newPts.clear();
	vector<double> aTemp = a, bTemp = b;
	FillRandomPoints(aTemp, bTemp, maxIters, 0, n, newPts);
}

void CAlgorithms::OptimizeFunctionDerivative(function<double(const vector<double> &)> f, const vector<double>& aVect, const vector<double>& bVect, const vector<double>& startPoint, vector<double>& x, TargetObjective minMax) {
	x.clear();
	const int n = (int)aVect.size();
	x.resize(n);
	ColumnVector startingPoint(n, 1);
	for (int i = 0; i < n; ++i) {
		startingPoint(i, 0) = startPoint[i];
	}
	const int minMaxNum = (minMax == TargetObjective::Min) ? 1 : (-1);
	function<double(ColumnVector)> g = [f, minMaxNum](const ColumnVector p) {
		const int n = p.nr();
		vector<double> x(n, 0.0);
		for (int i = 0; i < n; ++i) {
			x[i] = p(i, 0);
		}
		return minMaxNum * f(x);
	};
	ColumnVector a(n, 1), b(n, 1);
	for (int i = 0; i < n; ++i) {
		a(i, 0) = aVect[i];
		b(i, 0) = bVect[i];
	}
	dlib::find_min_box_constrained(dlib::lbfgs_search_strategy(10),
			dlib::objective_delta_stop_strategy(1e-7), g, dlib::derivative(g), startingPoint, a, b);
	for (int i = 0; i < n; ++i) {
		x[i] = startingPoint(i, 0);
	}
	//cout << "Optim solution:\n" << startingPoint << endl;
}

void CAlgorithms::VectorToColumnVector(const vector<double>& vect, ColumnVector& col) {
	const int n = (int)vect.size();
	col.set_size(n, 1);
	for (int i = 0; i < n; ++i) {
		col(i, 0) = vect[i];
	}
}

void CAlgorithms::ColumnVectorToVector(const ColumnVector& col, vector<double>& vect) {
	const int n = col.nr();
	vect.clear();
	vect.resize(n);
	for (int i = 0; i < n; ++i) {
		vect[i] = col(i, 0);
	}
}

void CAlgorithms::OptimizeFunctionGlobal(function<double(const vector<double> &)> f, const vector<double>& aVect, const vector<double>& bVect, vector<double>& x, TargetObjective minMax, const int iters) {
	const int n = (int)aVect.size();
	if ((int)bVect.size() != n) {
		throw CSmartException("CAlgorithms::OptimizeFunctionGlobal: aVect should be equal to bVect");
	}
	double bestVal;
	vector<double> bestX;
	for (int i = 0; i < iters; ++i) {
		vector<double> curX;
		vector<double> startPoint = *RandomDoubleVector(aVect, bVect);
		OptimizeFunctionDerivative(f, aVect, bVect, startPoint, curX, minMax);
		const double curValue = f(curX);
		if (i == 0) {
			bestVal = curValue; bestX = curX;
		} else {
			if (minMax == TargetObjective::Max) {
				if (curValue > bestVal) {
					bestVal = curValue; bestX = curX;
				}
			} else if (minMax == TargetObjective::Min) {
				if (curValue < bestVal) {
					bestVal = curValue; bestX = curX;
				}
			} else {
				throw CSmartException("CAlgorithms::OptimizeFunctionGlobal: no other choice for TargetObjective");
			}
		}
	}
	x = bestX;
}

void CAlgorithms::SolveQPConstrained(const dlib::matrix<double>& Q,
		const vector<double>& b, const vector<double>& aVect,
		const vector<double>& bVect, vector<double>& solution,
		const int maxIters) {
	const int n = (int)b.size();
	if ((Q.nr() != n) || (Q.nc() != n)) {
		throw CSmartException("CAlgorithms::SolveQPConstrained: Q dims not equal to n!");
	}
	if ((int)aVect.size() != n) {
		throw CSmartException("CAlgorithms::SolveQPConstrained: aVect dim not equal to n!");
	}
	if ((int)bVect.size() != n) {
		throw CSmartException("CAlgorithms::SolveQPConstrained: bVect dim not equal to n!");
	}
	ColumnVector lower(n, 1), upper(n, 1), bCol(n, 1), solutionVector(n, 1);
	VectorToColumnVector(aVect, lower);
	VectorToColumnVector(bVect, upper);
	VectorToColumnVector(b, bCol);
	solution = *RandomDoubleVector(aVect, bVect);
	VectorToColumnVector(solution, solutionVector);
	//cout << bCol << lower << upper << endl;
	//cout << Q << endl;
	dlib::solve_qp_box_constrained(Q, bCol, solutionVector, lower, upper, 10e-7, maxIters);
	//cout << "Solution found: " << solutionVector << " in " << iters << " iterations" << endl;
	ColumnVectorToVector(solutionVector, solution);
}

void CAlgorithms::MakeMatrix(shared_ptr<const CSupplementMatrix> inputMatrix, const vector<int> & assetIds, dlib::matrix<double>& outputMatrix) {
	const int n = (int)assetIds.size();
	outputMatrix.set_size(n, n);
	for (int i = 0; i < n; ++i) {
		const int iAssetId = assetIds[i];
		for (int j = 0; j < n; ++j) {
			const int jAssetId = assetIds[j];
			outputMatrix(i, j) = inputMatrix->operator ()(iAssetId, jAssetId);
		}
	}
}

void CAlgorithms::FillRandomPoints(vector<double> a, vector<double> b, int maxIters, int i, int n, vector<shared_ptr<vector<double> > >& newPts) {
	if (i >= n) {
		// в a и b лежат текущие границы интервала
		// в них надо выбрать случайный вектор
		auto randVect = RandomDoubleVector(a, b);
		newPts.push_back(randVect);
		//cout << "[(" << a[0] << ", " << b[0] << "),(" << a[1] << ", " << b[1] << ")]," << endl;
		return;
	}
	const int amtOfPieces = floor(pow(maxIters, 1.0/n));
	const double delta = (b[i] - a[i]) / amtOfPieces;
	const double leftBorder = a[i];
	for (int k = 1; k <= amtOfPieces; k++) {
		b[i] = leftBorder + k * delta;
		FillRandomPoints(a, b, maxIters, i+1, n, newPts);
		a[i] = b[i];
	}
}

void CAlgorithms::LeastSquaresDerivative(const dlib::matrix<double> & xs, const vector<double> & ys,
		function<double(const vector<double> &, const vector<double> &)> ModelFunc,
		const vector<double>& aVect, const vector<double>& bVect,
		vector<double>& theta, int iters) {
	const int n = (int)ys.size();
	if (xs.nc() != n) {
		throw CSmartException("CAlgorithms::LeastSquaresDerivative: number of columns in xs must be equal to amount of elements in ys");
	}
	function<double(const vector<double> &)> f = [ModelFunc, & xs, & ys](const vector<double> &thPars) {
		double s = 0.0;
		const int n = (int)xs.size();
		const int m = (int)xs.nr();
		for (int i = 0; i < n; ++i) {
			vector<double> x(m);
			for (int j = 0; j < m; ++j) {
				x[j] = xs(j, i);
			}
			const double y = ys[i];
			const double tmp = (ModelFunc(x, thPars) - y);
			s += tmp*tmp;
		}
		return s;
	};
	//const vector<double> startPoint = *RandomDoubleVector(aVect, bVect);
	//OptimizeFunctionDerivative(f, aVect, bVect, startPoint, theta, TargetObjective::Min);
	OptimizeFunctionGlobal(f, aVect, bVect, theta, TargetObjective::Min, iters);
}
