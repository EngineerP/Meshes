#include "catmull_clark.h"
#include <unordered_map>
#include <map>
#include <utility>
#include <functional>
#include <iostream>
#include <vector>

void catmull_clark(
	const Eigen::MatrixXd& V,
	const Eigen::MatrixXi& F,
	const int num_iters,
	Eigen::MatrixXd& SV,
	Eigen::MatrixXi& SF)
{
	// Using the recusrive evaluation of Catmull Clark as described on its wikipedia page
	// base case, end of recursion
	if (num_iters == 0) {
		return;
	}
	//Loop through all the faces and generate their facepoints
	Eigen::MatrixXd facePoint(F.rows(), V.cols());
	for (int i = 0; i < F.rows(); i++) {
		//Add all four vertices and avg
		//Add it to matrix of facepoints
		facePoint.row(i) = (V.row(F(i, 0)) + V.row(F(i, 1)) + V.row(F(i, 2)) + V.row(F(i, 3))) / 4.0;
	}

	//Get all vertices and their adjacent faces
	std::vector<std::vector<int>> VAdjF;
	for (int i = 0; i < F.rows(); i++) {
		// Add Faces to the adjacency list of the vertices that make up the face
		VAdjF.resize(V.rows());
		for (int j = 0; j < F.cols(); j++) {
			VAdjF[F(i, j)].emplace_back(i);
		}
	}

	//Determine the Edges and their adjacent faces
	//Make a Vector(Endpoint1)<Vector(Endpoint2)<Vector(Faces)>>>
	std::map<int, std::map<int, std::vector<int>>> EAdjF;
	std::map<int, std::vector<int>> VAdjV;
	//Loop Through faces and add edges
	for (int i = 0; i < F.rows(); i++) {
		for (int j = 0; j < F.cols(); j++) {
			EAdjF[F(i, j)][F(i, (j + 1) % F.cols())].emplace_back(i);
			EAdjF[F(i, (j + 1) % F.cols())][F(i, j)].emplace_back(i);
			VAdjV[F(i, j)].emplace_back(F(i, (j + 1) % F.cols()));
			//VAdjV[F(i, (j + 1) % F.cols())].emplace_back(F(i, j));
		}
	}

	//Calculate the New EdgePoints
	std::map<int, std::map<int, Eigen::RowVector3d>> EdgeToEdgePoint;
	for (int i = 0; i < F.rows(); i++) {
		for (int j = 0; j < F.cols(); j++) {
			Eigen::RowVector3d edgePoint(0, 0, 0);
			edgePoint += V.row(F(i, j)) + V.row(F(i, (j + 1) % F.cols()));
			edgePoint += facePoint.row(EAdjF[F(i, j)][F(i, (j + 1) % F.cols())][0]) + facePoint.row(EAdjF[F(i, j)][F(i, (j + 1) % F.cols())][1]);
			edgePoint /= 4.0;
			EdgeToEdgePoint[F(i, j)][F(i, (j + 1) % F.cols())] = edgePoint;
			EdgeToEdgePoint[F(i, (j + 1) % F.cols())][F(i, j)] = edgePoint;
		}
	}

	// Loop VF and calculate the Fval for the Catmull Vertext reposition formula
	std::vector<Eigen::RowVector3d> VtoF(V.rows());
	for (int i = 0; i < VAdjF.size(); i++) {
		VtoF[i] = { 0, 0, 0 };
		//Add all the facepoints for the adjacent faces and average them
		for (int j = 0; j < VAdjF[i].size(); j++) {
			VtoF[i] += facePoint.row(VAdjF[i][j]);
		}
		VtoF[i] /= (double)VAdjF[i].size();
	}

	//Loop through VadjV, and calculate R for the Catmull Vertext reposition formula
	std::vector<Eigen::RowVector3d> VtoR(V.rows());
	for (int i = 0; i < VAdjV.size(); i++) {
		VtoR[i] = { 0, 0, 0 };
		//Add all the facepoints for the adjacent faces and average them
		for (int j = 0; j < VAdjV[i].size(); j++) {
			VtoR[i] += (V.row(VAdjV[i][j]) + V.row(i)) / 2.0;
		}
		VtoR[i] /= (double)VAdjV[i].size();
	}

	//Loop through V and recalulate the orginal points location using R and F
	SV.resize(V.rows(), V.cols());
	for (int i = 0; i < V.rows(); i++) {
		Eigen::RowVector3d newPoint(0, 0, 0);
		if (VAdjF[i].size() != VAdjV[i].size()) std::cout << "adj faces != adj vertices WTF for V(" << i << ")\n";
		newPoint = (VtoF[i] + (2.0 * VtoR[i]) + (VAdjV[i].size() - 3) * (V.row(i))) / VAdjV[i].size();
		SV.row(i) = newPoint;
	}

	//Loop through facepoints and add them to SV
	SV.conservativeResize(SV.rows() + facePoint.rows(), SV.cols());
	for (int i = 0; i < facePoint.rows(); i++) {
		SV.row(V.rows() + i) = facePoint.row(i);
	}

	//Loop through edgepoints and add them to SV
	//Create a vector<vector<int>> to hold indices into SV for the edgepoints
	std::map<int, std::map<int, int>> EtoSV;
	for (int i = 0; i < F.rows(); i++) {
		for (int j = 0; j < F.cols(); j++) {
			if (EtoSV[F(i, j)][F(i, (j + 1) % F.cols())] == NULL && EtoSV[F(i, (j + 1) % F.cols())][(i, j)] == NULL) {
				//If no index has been noted, its not in SV, so add it
				SV.conservativeResize(SV.rows() + 1, SV.cols());
				SV.row(SV.rows() - 1) = EdgeToEdgePoint[F(i, j)][F(i, (j + 1) % F.cols())];
				//Store index
				EtoSV[F(i, j)][F(i, (j + 1) % F.cols())] = SV.rows() - 1;
				EtoSV[F(i, (j + 1) % F.cols())][F(i, j)] = SV.rows() - 1;
			}
		}
	}

	//Now that all the points are in SV, and we can track the indices
	//Original indices of Points in V, match SV
	//Facepoints are V.rows() + Face index into F
	//Edgepoints indices into SV are stored in EtoSV using Endpoint indexs into V
	//Loop through faces to build faces
	SF.resize(0, F.cols());
	for (int i = 0; i < F.rows(); i++) {
		for (int j = 0; j < F.cols(); j++) {
			int vertexIndex, edgePoint1Index, facePointIndex, edgePoint2Index = -1;
			vertexIndex = F(i, j);
			edgePoint1Index = EtoSV[F(i, j)][F(i, (j + 1) % F.cols())];
			facePointIndex = V.rows() + i;
			edgePoint2Index = EtoSV[F(i, j)][F(i, (j - 1 + F.cols()) % F.cols())];
			SF.conservativeResize(SF.rows() + 1, F.cols());
			SF(SF.rows() - 1, 0) = vertexIndex;
			SF(SF.rows() - 1, 1) = edgePoint1Index;
			SF(SF.rows() - 1, 2) = facePointIndex;
			SF(SF.rows() - 1, 3) = edgePoint2Index;
		}
	}
	catmull_clark(Eigen::MatrixXd(SV), Eigen::MatrixXi(SF), num_iters - 1, SV, SF);
}
