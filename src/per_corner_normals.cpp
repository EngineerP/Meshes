#define _USE_MATH_DEFINES
#include "per_corner_normals.h"
#include "triangle_area_normal.h"
// Hint:
#include "vertex_triangle_adjacency.h"
#include <iostream>
#include <math.h>

void per_corner_normals(
	const Eigen::MatrixXd& V,
	const Eigen::MatrixXi& F,
	const double corner_threshold,
	Eigen::MatrixXd& N)
{
	N = Eigen::MatrixXd::Zero(F.rows() * 3, 3);
	////////////////////////////////////////////////////////////////////////////
	// Add your code here:
	//Get all the adjacent faces to a vector
	std::vector<std::vector<int>> VF;
	vertex_triangle_adjacency(F, V.rows(), VF);

	//Loop Through Faces
	for (int i = 0; i < F.rows(); i++){
		//Calculate base normal for the face
		Eigen::Vector3d faceNormal = triangle_area_normal(V.row(F(i, 0)), V.row(F(i, 1)), V.row(F(i, 2))).normalized();
		//Go through the corners of this face
		for (int j = 0; j < 3; j++) {
			std::vector<int> adjFaces = VF[F(i, j)];
			Eigen::Vector3d sumNormal(0, 0, 0);
			//Go through the adjFaces and comapre normals
			for (int k = 0; k < adjFaces.size(); k++) {
				Eigen::Vector3d adjFaceNormal = triangle_area_normal(V.row(F(adjFaces[k], 0)), V.row(F(adjFaces[k], 1)), V.row(F(adjFaces[k], 2)));
				if (faceNormal.dot(adjFaceNormal.normalized()) > cos(corner_threshold * (M_PI / 180.0))) {
					sumNormal += adjFaceNormal;
				}
			}
			N.row((i * 3) + j) = sumNormal.normalized();
		}
	}
	////////////////////////////////////////////////////////////////////////////
}
