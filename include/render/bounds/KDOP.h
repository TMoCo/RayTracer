/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// k-DOP (discrete oriented polytope)
//

#ifndef K_DOP_H
#define K_DOP_H

#include <Vector3.h>
#include <render/primitives/Mesh.h>

#include <glad/glad.h>
#include <vector>


class KDOP
{
public:
	static const std::vector<Vector3> boxAxes;
	static const std::vector<Vector3> edgesAxes;
	static const std::vector<Vector3> cornersAxes;

	// enum for constructing k-dop
	typedef enum : uint32_t
	{
		BOX = 0x1,
		EGDES = 0x2,
		CORNERS = 0x4
	} kAxes;

	struct Slab
	{
		Vector3 normal;
		float dmin, dmax;
	};

	uint32_t vao, vbo;

  std::vector<Slab> slabs; // union of slabs defifne a volume
	Vector3 centre;

	inline static KDOP getKDOP(const Mesh& mesh, uint32_t axesMask)
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
		for (uint32_t i = 0; i < kdop.slabs.size(); ++i)
			kdop.slabs[i].normal = axes[i].normalize(); // set normals

		/*
		float project;
		for (auto& position : mesh.positions)
			kdop.centre += position;
		kdop.centre /= static_cast<float>(mesh.positions.size());

		for (auto& position : mesh.positions)
		{
			for (uint32_t i = 0; i < kdop.slabs.size(); ++i)
			{
				// project position onto slab normal and test if largest or smallest 
				project = position.dot(kdop.slabs[i].normal);
				if (project > kdop.slabs[i].dmax)
					kdop.slabs[i].dmax = project;
				if (project < kdop.slabs[i].dmin)
					kdop.slabs[i].dmin = project;
			}
		}
		*/
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

#endif // !K_DOP_H
