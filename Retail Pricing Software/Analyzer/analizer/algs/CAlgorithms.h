/*
 * CAlgorithms.h
 *
 *  Created on: Jul 11, 2019
 *      Author: kraychik
 */

#ifndef ANALIZER_ALGS_CALGORITHMS_H_
#define ANALIZER_ALGS_CALGORITHMS_H_

#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <dlib/optimization.h>

using namespace std;

class CSupplementMatrix;
class CAlgorithms {
public:
	typedef dlib::matrix<double,0,1> ColumnVector;
	enum class TargetObjective { Min, Max };
	CAlgorithms();
	virtual ~CAlgorithms();
	static void OptimizeFunctionManual(function<double(const vector<double> & )> f, const vector<double> & aVect, const vector<double> & bVect, vector<double> & x);
	static void OptimizeFunctionGlobal(function<double(const vector<double> & )> f, const vector<double> & aVect, const vector<double> & bVect, vector<double> & x, TargetObjective minMax, const int iters);
	static void SolveQPConstrained(const dlib::matrix<double> & Q, const vector<double> & b, const vector<double> & aVect, const vector<double> & bVect, vector<double> & solution, const int maxIters);
	static void LeastSquares(const map<double, double> & dataSample, function<double(double, const vector<double> &)> Model, const vector<double> & aVect, const vector<double> & bVect, vector<double> & theta);
	static void LeastSquaresDerivative(const dlib::matrix<double> & xs, const vector<double> & ys, function<double(const vector<double> &, const vector<double> &)> ModelFunc, const vector<double> & aVect, const vector<double> & bVect, vector<double> & theta, int iters);
	static void OptimizeFunctionDerivative(function<double(const vector<double> & )> f, const vector<double> & aVect, const vector<double> & bVect, const vector<double> & startPoint, vector<double> & x, TargetObjective minMax);

	static void VectorToColumnVector(const vector<double> & vect, ColumnVector & col);
	static void ColumnVectorToVector(const ColumnVector & col, vector<double> & vect);
	static void MakeMatrix(shared_ptr<const CSupplementMatrix> inputMatrix, const vector<int> & assetIds, dlib::matrix<double> & outputMatrix);

	static int Random(int from, int to);
	static double RandomDouble(double fMin, double fMax);
	static void GenRandomVector(vector<int> & arr);
	static void FillRandVector(const vector<double> & a, const vector<double> & b, vector<double> & x);
	static shared_ptr<vector<double>> RandomDoubleVector(const vector<double> & a, const vector<double> & b);
	static double GenRandValue(const vector<double> & values, const vector<double> & probs);
private:
	static void GenRandomGridPoints(const vector<double> & a, const vector<double> & b, int maxIters, vector<shared_ptr<vector<double>>> & newPts);
	static void FillRandomPoints(vector<double> a, vector<double> b, int maxIters, int i, int n, vector<shared_ptr<vector<double>>> & newPts);
};

#endif /* ANALIZER_ALGS_CALGORITHMS_H_ */
