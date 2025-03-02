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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <limits>

#include "statslog_cts.h"
#include "statslog_test.h"

#ifdef __ANDROID__

namespace {

using namespace testing;
using std::unique_ptr;

TEST(AtomCodeGenTest, AtomConstants) {
    ASSERT_EQ(android::stats::BLE_SCAN_STATE_CHANGED, 2);
    ASSERT_EQ(android::stats::cts::TEST_ATOM_REPORTED, 205);
}

TEST(AtomCodeGenTest, CreateLinearHistogram) {
    unique_ptr<android::stats::StatsHistogram> hist =
            android::stats::create_test_extension_atom_reported__linear_histogram_histogram();

    EXPECT_THAT(hist->getBins(), ElementsAre(android::stats::UNDERFLOW_BIN, 0, 10, 20, 30, 40, 50,
                                             60, 70, 80, 90, 100));
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
}

TEST(AtomCodeGenTest, CreateExponentialHistogram) {
    unique_ptr<android::stats::StatsHistogram> hist =
            android::stats::create_test_extension_atom_reported__exponential_histogram_histogram();

    EXPECT_THAT(hist->getBins(),
                ElementsAre(android::stats::UNDERFLOW_BIN, 5, 10, 20, 40, 80, 160));
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 0, 0, 0, 0, 0, 0));
}

TEST(AtomCodeGenTest, CreateExplicitHistogram) {
    unique_ptr<android::stats::StatsHistogram> hist =
            android::stats::create_test_extension_atom_reported__explicit_histogram_histogram();

    EXPECT_THAT(hist->getBins(), ElementsAre(android::stats::UNDERFLOW_BIN, -10, -7, 0, 19, 100));
    EXPECT_THAT(hist->getBinCounts(), ElementsAre(0, 0, 0, 0, 0, 0));
}

}  // namespace

#else
GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
