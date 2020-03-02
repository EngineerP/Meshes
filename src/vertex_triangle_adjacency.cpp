#include "vertex_triangle_adjacency.h"

void vertex_triangle_adjacency(
	const Eigen::MatrixXi& F,
	const int num_vertices,
	std::vector<std::vector<int> >& VF)
{
	VF.resize(num_vertices);
	////////////////////////////////////////////////////////////////////////////
	// Add your code here:
	// Loop through faces
	for (int i = 0; i < F.rows(); i++) {
		// Add Faces to the adjacency list of the vertices that make up the face
		for (int j = 0; j < F.cols(); j++) {
			VF[F(i, j)].emplace_back(i);
		}
	}

	////////////////////////////////////////////////////////////////////////////
}

