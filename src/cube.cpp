#include "cube.h"

void cube(
	Eigen::MatrixXd& V,
	Eigen::MatrixXi& F,
	Eigen::MatrixXd& UV,
	Eigen::MatrixXi& UF,
	Eigen::MatrixXd& NV,
	Eigen::MatrixXi& NF)
{
	////////////////////////////////////////////////////////////////////////////
	// Add your code here:
	////////////////////////////////////////////////////////////////////////////

	//Hint:
	V.resize(8, 3);
	F.resize(6, 4);
	UV.resize(14, 2);
	UF.resize(6, 4);
	NV.resize(6, 3);
	NF.resize(6, 4);

	//Add Vertices
	//Trying to follow coordinate system in README
	V.row(0) = Eigen::Vector3d(0, 0, 0);
	V.row(1) = Eigen::Vector3d(1, 0, 0);
	V.row(2) = Eigen::Vector3d(1, 1, 0);
	V.row(3) = Eigen::Vector3d(0, 1, 0);
	V.row(4) = Eigen::Vector3d(0, 0, -1);
	V.row(5) = Eigen::Vector3d(1, 0, -1);
	V.row(6) = Eigen::Vector3d(1, 1, -1);
	V.row(7) = Eigen::Vector3d(0, 1, -1);

	//Add Faces
	//    [4]
	// [0][1][2][3]
	//    [5]
	F.row(0) = Eigen::Vector4i(4, 0, 3, 7);
	F.row(1) = Eigen::Vector4i(0, 1, 2, 3);
	F.row(2) = Eigen::Vector4i(1, 5, 6, 2);
	F.row(3) = Eigen::Vector4i(5, 4, 7, 6);
	F.row(4) = Eigen::Vector4i(3, 2, 6, 7);
	F.row(5) = Eigen::Vector4i(4, 5, 1, 0);

	//Populate Texture Map Coordinates
	UV.row(0) = Eigen::Vector2d(0, 0.25);
	UV.row(1) = Eigen::Vector2d(0, 0.5);
	UV.row(2) = Eigen::Vector2d(0.25, 0.5);
	UV.row(3) = Eigen::Vector2d(0.25, 0.75);
	UV.row(4) = Eigen::Vector2d(0.5, 0.75);
	UV.row(5) = Eigen::Vector2d(0.5, 0.5);
	UV.row(6) = Eigen::Vector2d(0.75, 0.5);
	UV.row(7) = Eigen::Vector2d(1, 0.5);
	UV.row(8) = Eigen::Vector2d(1, 0.25);
	UV.row(9) = Eigen::Vector2d(0.75, 0.25);
	UV.row(10) = Eigen::Vector2d(0.5, 0.25);
	UV.row(11) = Eigen::Vector2d(0.5, 0);
	UV.row(12) = Eigen::Vector2d(0.25, 0);
	UV.row(13) = Eigen::Vector2d(0.25, 0.25);

	//Populate Faces with Texture Mappings
	UF.row(0) = Eigen::Vector4i(0, 13, 2, 1);
	UF.row(1) = Eigen::Vector4i(13, 10, 5, 2);
	UF.row(2) = Eigen::Vector4i(10, 9, 6, 5);
	UF.row(3) = Eigen::Vector4i(9, 8, 7, 6);
	UF.row(4) = Eigen::Vector4i(2, 5, 4, 3);
	UF.row(5) = Eigen::Vector4i(12, 11, 10, 13);

	//Populate Normal Vectors:
	NV.row(0) = Eigen::Vector3d(1, 0, 0);
	NV.row(1) = Eigen::Vector3d(-1, 0, 0);
	NV.row(2) = Eigen::Vector3d(0, 1, 0);
	NV.row(3) = Eigen::Vector3d(0, -1, 0);
	NV.row(4) = Eigen::Vector3d(0, 0, 1);
	NV.row(5) = Eigen::Vector3d(0, 0, -1);

	//Populate Faces with PerVertex Normals
	NF.row(0) = Eigen::Vector4i(1, 1, 1, 1);
	NF.row(1) = Eigen::Vector4i(4, 4, 4, 4);
	NF.row(2) = Eigen::Vector4i(0, 0, 0, 0);
	NF.row(3) = Eigen::Vector4i(5, 5, 5, 5);
	NF.row(4) = Eigen::Vector4i(2, 2, 2, 2);
	NF.row(5) = Eigen::Vector4i(3, 3, 3, 3);

}
