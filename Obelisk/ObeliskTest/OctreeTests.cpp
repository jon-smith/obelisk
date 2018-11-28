#include "stdafx.h"
#include "CppUnitTest.h"
#include "Octree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
	TEST_CLASS(OctreeTest)
	{

		static Octree<Vector3f, 256> make(const Box &extents, size_t count)

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

	public:

		TEST_METHOD(SmokeTest)
		{
			constexpr size_t countToInsert = 256 * 256;
			const Box extents({ 0,0,0 }, { 1,1,1 });
			const auto tree = make(extents, countToInsert);

			const auto &find = tree.findNode({ 0,0,0 });
			const auto findNearest = tree.findNearestNeighbour({ 0,0,0 }, 0.5f);

			size_t count = 0;
			tree.forEachItem([&count](const Vector3f &) {
				++count;
			});
			Assert::AreEqual(countToInsert, count, L"counted total");

			tree.forEachItemInSphere({ 0,0,0 }, 1.f, [](const Vector3f &) {
				return;
			});

			tree.forEachItemInBox(extents, [](const Vector3f &) {
				return;
			});

			const auto depth = tree.getMaxDepth();
			const auto total = tree.getTotalItemCount();
			Assert::AreEqual(countToInsert, total, L"cache total");

		}
	};
}