//
// k-DOP (discrete oriented polytope)
//

#ifndef K_DOP_H_
#define K_DOP_H_

#include <math/thomath.h>

#include <render/Mesh.h>

#include <glad/glad.h>

#include <vector>

struct KDOP
{
	static const std::vector<Vector3> boxAxes;
	static const std::vector<Vector3> edgesAxes;
	static const std::vector<Vector3> cornersAxes;

	// enum for constructing k-dop
	typedef enum : UI32
	{
		BOX = 0x1,
		EGDES = 0x2,
		CORNERS = 0x4
	} kAxes;

	struct Slab
	{
		Vector3 normal;
		F32 dmin, dmax;
	};

	UI32 vao, vbo;

  std::vector<Slab> slabs; // union of slabs defifne a volume
	Vector3 centre;

	inline static KDOP getKDOP(const Mesh& mesh, UI32 axesMask)
	{
		KDOP kdop{};

		// get axes based on mask
		std::vector<Vector3> axes{};
		if (axesMask & kAxes::BOX)
			axes.insert(axes.end(), boxAxes.begin(), boxAxes.end());
		if (axesMask & kAxes::EGDES)
			axes.insert(axes.end(), edgesAxes.begin(), edgesAxes.end());
		if (axesMask & kAxes::CORNERS)
			axes.insert(axes.end(), cornersAxes.begin(), cornersAxes.end());

		kdop.slabs.resize(axes.size(), {}); // initialise with empty slabs
		for (UI32 i = 0; i < kdop.slabs.size(); ++i)
			kdop.slabs[i].normal = axes[i].normalize(); // set normals

		F32 project;
		for (auto& position : mesh.positions)
			kdop.centre += position;
		kdop.centre /= mesh.positions.size();


		for (auto& position : mesh.positions)
		{
			for (UI32 i = 0; i < kdop.slabs.size(); ++i)
			{
				// project position onto slab normal and test if largest or smallest 
				project = position.dot(kdop.slabs[i].normal);
				if (project > kdop.slabs[i].dmax)
					kdop.slabs[i].dmax = project;
				if (project < kdop.slabs[i].dmin)
					kdop.slabs[i].dmin = project;
			}
		}
		return kdop;
	}
};


// normals for 6-DOP
const std::vector<Vector3> KDOP::boxAxes =
{
	// y
	{ 0.0f, 1.0f, 0.0f },
	// x
	{ 1.0f, 0.0f, 0.0f },
	// z
	{ 0.0f, 0.0f, 1.0f }
};

// normals for 18-DOP (if included with previous = aabb with beveled edges)
const std::vector<Vector3> KDOP::edgesAxes =
{
	// x y
	{ 1.0f, 1.0f, 0.0f },
	// -x y
	{ -1.0f, 1.0f, 0.0f },
	// x z
	{ 1.0f, 0.0f, 1.0f },
	// -x z
	{ -1.0f, 0.0f, 1.0f },
	// y z
	{ 0.0f, 1.0f, 1.0f },
	// y -z
	{ 0.0f, 1.0f, -1.0f }
};

// normals for 22-DOP (if included with previous = aabb with beveled edges and corners)
const std::vector<Vector3> KDOP::cornersAxes = 
{
	// x y z
	{ 1.0f, 1.0f, 1.0f },
	// x -y z
	{ 1.0f, -1.0f, 1.0f },
	// -x y z
	{ -1.0f, 1.0f, 1.0f },
	// -x -y z
	{ -1.0f, -1.0f, 1.0f }
};

#endif // !K_DOP_H_
