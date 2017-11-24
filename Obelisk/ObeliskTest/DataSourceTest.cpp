#include "stdafx.h"
#include "CppUnitTest.h"
#include "DataSourceWithPolling.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
  namespace Impl
  {
    class TestDataGetter_DoubleDouble
    {
    public:

      TestDataGetter_DoubleDouble() = delete;

      TestDataGetter_DoubleDouble(std::function<double(size_t)> firstValFunc, std::function<double(size_t)> secondValFunc) : 
      firstValFunc(firstValFunc),
      secondValFunc(secondValFunc)
      {
        
      }

      void updateData(std::vector<std::tuple<double,double>> &d) const
      {
        d.push_back({ firstValFunc(d.size()) , secondValFunc(d.size()) });
      }

    private:
      std::function<double(size_t)> firstValFunc;
      std::function<double(size_t)> secondValFunc;
    };

    template <size_t PollTimeMs>
    using PolledDataSource_DoubleDouble = DataSourceWithPolling<TestDataGetter_DoubleDouble, PollTimeMs, double, double>;
  }

  TEST_CLASS(DataSourceTests)
  {
  public:
    TEST_METHOD(DoubleDoublePollingTest)
    {
      auto const firstValFunc = [](size_t i)
      {
        return static_cast<double>(i)*2.5;
      };

      auto const secondValFunc = [](size_t i)
      {
        return static_cast<double>(i)*5.0;
      };

      constexpr size_t pollingTimeMs = 10;
      constexpr size_t expectedPolls = 10;
      constexpr size_t minExpectedPolls = expectedPolls / 2;
      constexpr size_t maxExpectedPolls = expectedPolls * 3 / 2;
      Impl::PolledDataSource_DoubleDouble<pollingTimeMs> dataSource10ms(firstValFunc, secondValFunc);
      std::this_thread::sleep_for(std::chrono::milliseconds(pollingTimeMs*expectedPolls));

      auto const allData = dataSource10ms.getAllData();
      auto const firstDouble = dataSource10ms.getSelectedVariable<0>();
      auto const secondDouble = dataSource10ms.getSelectedVariable<1>();
      // Should not compile:
      //auto const thirdDouble = dataSource10ms.getSelectedVariable<2>();

      Assert::IsTrue(allData.size() > minExpectedPolls && allData.size() < maxExpectedPolls, L"Size outside reasonable range of values based on time");
      Assert::IsTrue(firstDouble.size() > minExpectedPolls && allData.size() < maxExpectedPolls, L"Size outside reasonable range of values based on time");
      Assert::IsTrue(secondDouble.size() > minExpectedPolls && allData.size() < maxExpectedPolls, L"Size outside reasonable range of values based on time");

      for (size_t i = 0; i < allData.size(); ++i)
        Assert::IsTrue(allData[i] == std::make_tuple(firstValFunc(i), secondValFunc(i)), L"Tuple value is not as expected");

      for (size_t i = 0; i < firstDouble.size(); ++i)
        Assert::IsTrue(firstDouble[i] == firstValFunc(i), L"First variable is not as expected");

      for (size_t i = 0; i < secondDouble.size(); ++i)
        Assert::IsTrue(secondDouble[i] == secondValFunc(i), L"Second variable is not as expected");

      // Create as a polymorphic pointer
      auto const dataSourceBase = std::unique_ptr<IDataSource<double, double>>(std::make_unique<Impl::PolledDataSource_DoubleDouble<pollingTimeMs>>(firstValFunc, secondValFunc));
      auto const allData2 = dataSourceBase->getAllData();
    }
  };
}