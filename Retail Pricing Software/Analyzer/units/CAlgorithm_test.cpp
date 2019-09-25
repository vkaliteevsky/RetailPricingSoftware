/*
 * CAlgorithm_test.cpp
 *
 *  Created on: Jul 15, 2019
 *      Author: kraychik
 */

#include "gtest/gtest.h"
#include "algs/CAlgorithms.h"
#include <functional>
#include <time.h>

using namespace std;

TEST(CAlgorithm, OptimizeFunctionDerivative) {
	function<double(const vector<double> & )> f = [](const vector<double> & x) {
		return (x[0]-3)*(x[0]-3) + (x[1]+1) * (x[1]+1) + 3.0;
	};
	vector<double> a = {-20000.0, -20000.0};
	vector<double> b = {20000.0, 20000.0};
	vector<double> startPoint = {-100.0, 1000.5};
	vector<double> x;
	CAlgorithms::OptimizeFunctionDerivative(f, a, b, startPoint, x, CAlgorithms::TargetObjective::Max);
	EXPECT_DOUBLE_EQ(x[0], -20000.0);
	EXPECT_DOUBLE_EQ(x[1], 20000.0);
	clock_t t1 = clock();
	for (int i = 0; i < 1000; i++)
	CAlgorithms::OptimizeFunctionDerivative(f, a, b, startPoint, x, CAlgorithms::TargetObjective::Min);
	cout << "Done in time: " << (double)(clock() - t1)/CLOCKS_PER_SEC << endl;
	bool isXEq = abs(x[0] - 3.0) <= 0.01;
	bool isYEq = abs(x[1] + 1) <= 0.01;
	EXPECT_EQ(isXEq, true);
	EXPECT_EQ(isYEq, true);
}

TEST(CAlgorithm, LeastSquaresDerivative) {
	function<double(const vector<double> &, const vector<double> & )> f = [](const vector<double> & x, const vector<double> & theta) {
		return theta[0] * x[0] * x[0] + theta[1] * x[0] + theta[2];
	};
	dlib::matrix<double> xs(1, 3);
	xs = 1, 2, 0;
	vector<double> ys = {-3, 6, -8};
	vector<double> a = {-100, -100, -100};
	vector<double> b = {100, 100, 100};
	vector<double> theta;

	CAlgorithms::LeastSquaresDerivative(xs, ys, f, a, b, theta, 100);
	EXPECT_EQ(abs(theta[0]-2) <= 0.01, true);
	EXPECT_EQ(abs(theta[1]-3) <= 0.01, true);
	EXPECT_EQ(abs(theta[2]+8) <= 0.01, true);

	//xs.push_back(0);
	//ys.push_back(8);
	//CAlgorithms::LeastSquaresDerivative(xs, ys, f, a, b, theta);
	//cout << "Work is Done!" << endl;
	//cout << "[" << theta[0] << ", " << theta[1] << ", " << theta[2] << "]" << endl;
}
/*
TEST(CAlgorithm, SolveQP) {
	dlib::matrix<double> Q;
	int n = 3;
	Q.set_size(n, n);
	Q = -11, 6, -6, 6, -6, 3, -6, 3, -6;
	cout << "Q: " << Q << endl;
	dlib::matrix<double, 0, 1> alpha(n, 1), lower(n, 1), upper(n, 1), b(n, 1);
	lower = -1000, -1000, -1000;
	upper = 1000, 1000, 1000;
	alpha = 0, 0, 0;
	b = 1, 2, 5;
	cout << "Lower: " << lower << endl;
	cout << "Upper: " << upper << endl;
	const auto iters = dlib::solve_qp_box_constrained(Q, b, alpha, lower, upper, 1e-10, 5000);
	cout << "Solution found: " << alpha << " in " << iters << " iterations." << endl;
	dlib::matrix<double> mtx(3, 2);
	mtx = 1, 2, 3, 4, 5, 6;
	cout << "Row 2: " << mtx(1, 1) << endl;
}*/
TEST(CAlgorithm, SolveQP) {
	int n = 2;
	dlib::matrix<double> Q(n, n);
	Q = 5, 0, 0, 2;
	vector<double> aVect = {-100, -100};
	vector<double> bVect = {100, 100};
	vector<double> b = {0, 0};
	vector<double> solution;
	CAlgorithms::SolveQPConstrained(Q, b, aVect, bVect, solution, 10000);
	cout << "Solution: (" << solution[0] << ", " << solution[1] << ")" << endl;
	EXPECT_EQ(abs(solution[0]) <= 0.001, true);
	EXPECT_EQ(abs(solution[1]) <= 0.001, true);
}
