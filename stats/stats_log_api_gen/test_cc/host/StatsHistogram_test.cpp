/*
 * Copyright (C) 2025 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <StatsHistogram.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

namespace android {
namespace util {
namespace statslogapigen {
namespace {

using namespace testing;
using std::string;
using std::unique_ptr;
using std::vector;

struct TestCase {
    string name;
    unique_ptr<StatsHistogram> (*histCreator)();
};

void PrintTo(const TestCase& testCase, std::ostream* os) {
    *os << testCase.name;
}

class StatsHistogramTestParameterized : public TestWithParam<TestCase> {
protected:
    unique_ptr<StatsHistogram> hist;

    void SetUp() override {
        hist = GetParam().histCreator();
    }
};

const vector<TestCase> testCases = {
        {"linear", []() { return StatsHistogram::createLinearBins(0, 50, 5); }},
        {"exponential", []() { return StatsHistogram::createExponentialBins(5, 160, 5); }},
        {"explicit", []() { return StatsHistogram::createExplicitBins({-10, -7, 0, 19, 100}); }}};

INSTANTIATE_TEST_SUITE_P(StatsHistogramTestParameterized, StatsHistogramTestParameterized,
                         ValuesIn(testCases));

TEST(StatsHistogramTest, InitialStateLinear) {
    unique_ptr<StatsHistogram> hist = StatsHistogram::createLinearBins(0, 50, 5);
    EXPECT_THAT(hist->getBins(), ElementsAre(UNDERFLOW_BIN, 0, 10, 20, 30, 40, 50));
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 0, 0, 0, 0, 0, 0));
}

TEST(StatsHistogramTest, InitialStateExponential) {
    unique_ptr<StatsHistogram> hist = StatsHistogram::createExponentialBins(5, 160, 5);
    EXPECT_THAT(hist->getBins(), ElementsAre(UNDERFLOW_BIN, 5, 10, 20, 40, 80, 160));
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 0, 0, 0, 0, 0, 0));
}

TEST(StatsHistogramTest, InitialStateExplicit) {
    unique_ptr<StatsHistogram> hist = StatsHistogram::createExplicitBins({-10, -7, 0, 19, 100});
    EXPECT_THAT(hist->getBins(), ElementsAre(UNDERFLOW_BIN, -10, -7, 0, 19, 100));
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 0, 0, 0, 0, 0));
}

TEST(StatsHistogramTest, SingleEntryLinear) {
    unique_ptr<StatsHistogram> hist = StatsHistogram::createLinearBins(0, 50, 5);
    hist->addValue(18);
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 0, 1, 0, 0, 0, 0));
}

TEST(StatsHistogramTest, SingleEntryExponential) {
    unique_ptr<StatsHistogram> hist = StatsHistogram::createExponentialBins(5, 160, 5);
    hist->addValue(101);
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 0, 0, 0, 0, 1, 0));
}

TEST(StatsHistogramTest, SingleEntryExplicit) {
    unique_ptr<StatsHistogram> hist = StatsHistogram::createExplicitBins({-10, -7, 0, 19, 100});
    hist->addValue(0);
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 0, 0, 1, 0, 0));
}

TEST_P(StatsHistogramTestParameterized, Underflow) {
    hist->addValue(-100);

    const vector<int> binCounts = hist->getBinCounts();
    EXPECT_EQ(binCounts[0], 1);

    const vector<int> remainingBins(binCounts.begin() + 1, binCounts.end());
    EXPECT_THAT(remainingBins, Each(Eq(0)));
}

TEST_P(StatsHistogramTestParameterized, Overflow) {
    hist->addValue(200);

    const vector<int> binCounts = hist->getBinCounts();
    EXPECT_EQ(binCounts.back(), 1);

    const vector<int> remainingBins(binCounts.begin(), binCounts.end() - 1);
    EXPECT_THAT(remainingBins, Each(Eq(0)));
}

TEST_P(StatsHistogramTestParameterized, UnderflowMarker) {
    hist->addValue(UNDERFLOW_BIN);

    const vector<int> binCounts = hist->getBinCounts();
    EXPECT_EQ(binCounts[0], 1);

    const vector<int> remainingBins(binCounts.begin() + 1, binCounts.end());
    EXPECT_THAT(remainingBins, Each(Eq(0)));
}

TEST(StatsHistogramTest, MultipleEntriesLinear) {
    unique_ptr<StatsHistogram> hist = StatsHistogram::createLinearBins(0, 50, 5);
    hist->addValue(18);
    hist->addValue(40);
    hist->addValue(45);
    hist->addValue(19.99999);
    hist->addValue(27);
    hist->addValue(0.0000001);
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 1, 2, 1, 0, 2, 0));
}

TEST(StatsHistogramTest, MultipleEntriesExponential) {
    unique_ptr<StatsHistogram> hist = StatsHistogram::createExponentialBins(5, 160, 5);
    hist->addValue(101);
    hist->addValue(40);
    hist->addValue(45);
    hist->addValue(159.99999);
    hist->addValue(160.000001);
    hist->addValue(80);
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 0, 0, 0, 2, 3, 1));
}

TEST(StatsHistogramTest, MultipleEntriesExplicit) {
    unique_ptr<StatsHistogram> hist = StatsHistogram::createExplicitBins({-10, -7, 0, 19, 100});
    hist->addValue(0);
    hist->addValue(-10);
    hist->addValue(1);
    hist->addValue(25);
    hist->addValue(49);
    hist->addValue(-2);
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 1, 1, 2, 2, 0));
}

TEST_P(StatsHistogramTestParameterized, Clear) {
    for (float v = -20; v <= 200; v += 3) {
        hist->addValue(v);
    }
    hist->clear();

    // Check that all elements are 0.
    const vector<int> binCounts = hist->getBinCounts();
    EXPECT_THAT(binCounts, Each(Eq(0)));
}

}  // namespace
}  // namespace statslogapigen
}  // namespace util
}  // namespace android
