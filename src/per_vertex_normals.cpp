#include "per_vertex_normals.h"
#include "triangle_area_normal.h"

// Compute per vertex normals for a triangle mesh by computing the area-weighted
// average of normals at incident faces.
//
// Inputs:
//   V  #V by 3 list of vertex positions
//   F  #F by 3 list of mesh triangle indices into V
// Outputs:
//   N  #V by 3 list of per-face unit normal vectors

void per_vertex_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(V.rows(),3);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  // Loop through the Faces
  for (int i = 0; i < F.rows(); i++) {
	  //Calculate the Area Normal for each Face
	  Eigen::Vector3d normal = triangle_area_normal(V.row(F(i, 0)), V.row(F(i, 1)), V.row(F(i, 2)));
	  //Add it to the sum for each vertex on the face
	  N.row(F(i, 0)) += normal;
	  N.row(F(i, 1)) += normal;
	  N.row(F(i, 2)) += normal;
  }
  for (int j = 0; j < N.rows(); j++) {
	  N.row(j).normalize();
  }

  ////////////////////////////////////////////////////////////////////////////
}
