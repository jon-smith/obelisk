#include "catch.hpp"

#include "obelisk/Octree.hpp"

using namespace obelisk;

namespace
{

Octree<Vector3f, 256> make(const Box &extents, size_t count)
{
	Octree<Vector3f, 256> tree(extents);

	for (size_t i = 0; i < count; ++i)
	{
		const auto pos = static_cast<float>(i) / static_cast<float>(count);
		Vector3f vect(pos, pos, pos);
		tree.insert(vect);
	}

	return tree;
}
} // namespace

TEST_CASE("OctreeSmokeTest")
{
	constexpr size_t countToInsert = 256 * 256;
	const Box extents({0, 0, 0}, {1, 1, 1});
	const auto tree = make(extents, countToInsert);

	const auto &find = tree.findNode({0, 0, 0});
	const auto findNearest = tree.findNearestNeighbour({0, 0, 0}, 0.5f);

	SECTION("ForEachCount")
	{
		size_t count = 0;
		tree.forEachItem([&count](const Vector3f &) { ++count; });
		REQUIRE(countToInsert == count);
	}

	tree.forEachItemInSphere({0, 0, 0}, 1.f, [](const Vector3f &) { return; });

	tree.forEachItemInBox(extents, [](const Vector3f &) { return; });

	const auto depth = tree.getMaxDepth();
	const auto total = tree.getTotalItemCount();
	REQUIRE(countToInsert == total);
}
