//
// k-DOP (discrete oriented polytope)
//

#ifndef K_DOP_H_
#define K_DOP_H_

#include <math/thomath.h>

#include <render/Mesh.h>

#include <glad/glad.h>

#include <vector>

// normals for 22-DOP
static const std::vector<Vector3> k22normals 
{
	// y
	{ 0.0f, 1.0f, 0.0f }, 
	// x
	{ 1.0f, 0.0f, 0.0f },
	// z
	{ 0.0f, 0.0f, 1.0f },
	// x y
	{ 0.7071067811865475f, 0.7071067811865475f, 0.0f }, 
	// -x y
	{ -0.7071067811865475f, 0.7071067811865475f, 0.0f },
	// x z
	{ 0.7071067811865475f, 0.0f, 0.7071067811865475f },
	// -x z
	{ -0.7071067811865475f, 0.0f, 0.7071067811865475f },
	// x y z
	{ 0.5773502691896258f, 0.5773502691896258f, 0.5773502691896258f },
	// x -y z
	{ 0.5773502691896258f, -0.5773502691896258f, 0.5773502691896258f },
	// -x y z
	{ -0.5773502691896258f, 0.5773502691896258f, 0.5773502691896258f },
	// -x -y z
  { -0.5773502691896258f, -0.5773502691896258f, 0.5773502691896258f }
};

struct KDOP
{
	struct Slab
	{
		Vector3 normal;
		F32 dmin, dmax;
	};

	UI32 vao, vbo;

  std::vector<Slab> slabs; // union of slabs defifne a volume

	inline static KDOP getKDOP(const Mesh& mesh, const std::vector<Vector3>& normals)
	{
		KDOP kdop{};
		kdop.slabs.resize(normals.size(), {}); // initialise with empty slabs
		for (UI32 i = 0; i < kdop.slabs.size(); ++i)
		{
			kdop.slabs[i].normal = normals[i]; 
			for (auto& position : mesh.positions)
			{
				// project position onto normal and test if 
				F32 project = position.dot(normals[i]);
				if (project > kdop.slabs[i].dmax)
					kdop.slabs[i].dmax = project;
				if (project < kdop.slabs[i].dmin)
					kdop.slabs[i].dmin = project;
			}
		}
		return kdop;
	}
};

#endif // !K_DOP_H_
