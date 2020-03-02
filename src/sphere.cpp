#define _USE_MATH_DEFINES
#include "sphere.h"
#include <math.h>
#include <iostream>

void sphere(
	const int num_faces_u,
	const int num_faces_v,
	Eigen::MatrixXd& V,
	Eigen::MatrixXi& F,
	Eigen::MatrixXd& UV,
	Eigen::MatrixXi& UF,
	Eigen::MatrixXd& NV,
	Eigen::MatrixXi& NF)
{
	////////////////////////////////////////////////////////////////////////////
	// Add your code here:
	// Appropriately size matrices
	V.resize((num_faces_u + 1) * (num_faces_v + 1), 3);
	F.resize((num_faces_u * num_faces_v), 4);
	UV.resize(V.rows(), 2);
	UF.resize(F.rows(), 4);
	NV.resize(V.rows(), 3);
	NF.resize(F.rows(), 4);

	//Loop through and populate U,V texture coordinates and calculate x,y,z accordingly
	int vertexCount = 0;
	int faceCount = 0;
	for (int i = 0; i <= num_faces_v; i++) {
		for (int j = 0; j <= num_faces_u; j++) {
			//Calculate the u and v by stepping down and across the mercator lines using the number of faces
			double v = i * (1 / (double)num_faces_v);
			double u = j * (1 / (double)num_faces_u);
			UV.row(vertexCount) = Eigen::Vector2d(u, v);
			//Caculate the angles of the faces
			double phi = (v * M_PI) - (M_PI / 2);
			double theta = (u * 2 * M_PI) - M_PI;
			//Use said angles to calculate x,y,z
			double x = -1.0*(cos(phi) * cos(theta));
			double z = cos(phi) * sin(theta);
			double y = sin(phi);
			V.row(vertexCount) = Eigen::Vector3d(x, y, z);
			//Surface Normal vectors of a sphere are the x,y,z coordinates
			NV.row(vertexCount) = Eigen::Vector3d(x, y, z);
			vertexCount++;
			if (i < num_faces_v && j < num_faces_u) {
				//Use index math like we did for rasterization and ppm files to put together the appropriate indices for the faces
				int corner0 = i * (num_faces_u + 1) + j;
				int corner1 = i * (num_faces_u + 1) + (j + 1);
				int corner2 = (i + 1) * (num_faces_u + 1) + (j + 1);
				int corner3 = (i + 1) * (num_faces_u + 1) + j;
				F.row(faceCount) = Eigen::RowVector4i(corner0, corner1, corner2, corner3);
				UF.row(faceCount) = Eigen::RowVector4i(corner0, corner1, corner2, corner3);
				NF.row(faceCount) = Eigen::RowVector4i(corner0, corner1, corner2, corner3);
				faceCount++;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
}
