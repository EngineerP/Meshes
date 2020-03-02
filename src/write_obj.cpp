#include "write_obj.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_obj(
	const std::string& filename,
	const Eigen::MatrixXd& V,
	const Eigen::MatrixXi& F,
	const Eigen::MatrixXd& UV,
	const Eigen::MatrixXi& UF,
	const Eigen::MatrixXd& NV,
	const Eigen::MatrixXi& NF)
{
	assert((F.size() == 0 || F.cols() == 3 || F.cols() == 4) && "F must have 3 or 4 columns");
	////////////////////////////////////////////////////////////////////////////
	// Add your code here:
	std::ofstream objFile;
	objFile.open(filename);
	if (!objFile.is_open()) return false;

	//Push Vertex info to the file
	for (int i = 0; i < V.rows(); i++) {
		objFile << "v " << V(i, 0) << " " << V(i, 1) << " " << V(i, 2) << "\n";
	}
	//Push Vertex Normal info to file
	for (int i = 0; i < NV.rows(); i++) {
		objFile << "vn " << NV(i, 0) << " " << NV(i, 1) << " " << NV(i, 2) << "\n";
	}
	//Push Vertex Texture info to file
	for (int i = 0; i < UV.rows(); i++) {
		objFile << "vt " << UV(i, 0) << " " << UV(i, 1) << "\n";
	}

	//Push Face info into file, for each face:  Vertex Index/UV For Vertex on this Face Index/Normal for Vertex on this Face Index x poly
	for (int i = 0; i < F.rows(); i++) {
		objFile << "f ";
		for (int j = 0; j < F.cols(); j++) {
			objFile << F(i, j) + 1 << "/" << UF(i, j) + 1 << "/" << NF(i, j) + 1;
			//Add a space for the next vertex or a newline to start the new face
			if (j == F.cols() - 1) objFile << "\n";
			else objFile << " ";
		}
	}

	objFile.close();
	if (objFile.is_open()) return false;
	else return true;

	////////////////////////////////////////////////////////////////////////////
}
