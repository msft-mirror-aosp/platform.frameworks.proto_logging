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
#include <statslog_test_typesafe.h>

#ifdef __ANDROID__

namespace {

using namespace testing;

TEST(TypesafeCodeGenTest, AtomEnumFieldsValues) {
    using namespace android::stats::typesafe;

    EXPECT_EQ(static_cast<int>(BleScanStateChanged::State::OFF), 0);
    EXPECT_EQ(static_cast<int>(BleScanStateChanged::State::ON), 1);
    EXPECT_EQ(static_cast<int>(BleScanStateChanged::State::RESET), 2);

    EXPECT_EQ(static_cast<int>(SensorStateChanged::State::OFF), 0);
    EXPECT_EQ(static_cast<int>(SensorStateChanged::State::ON), 1);

    EXPECT_EQ(static_cast<int>(SyncStateChanged::State::OFF), 0);
    EXPECT_EQ(static_cast<int>(SyncStateChanged::State::ON), 1);

    EXPECT_EQ(static_cast<int>(ScheduledJobStateChanged::State::FINISHED), 0);
    EXPECT_EQ(static_cast<int>(ScheduledJobStateChanged::State::STARTED), 1);
    EXPECT_EQ(static_cast<int>(ScheduledJobStateChanged::State::SCHEDULED), 2);
    EXPECT_EQ(static_cast<int>(ScheduledJobStateChanged::State::CANCELLED), 3);
}

TEST(TypesafeCodeGenTest, AtomFieldNames) {
    using namespace android::stats::typesafe;

    ASSERT_EQ(offsetof(BleScanStateChanged, attribution_node), 0U);
    ASSERT_GT(offsetof(BleScanStateChanged, state), 0U);
    ASSERT_GT(offsetof(BleScanStateChanged, is_filtered), 0U);
    ASSERT_GT(offsetof(BleScanStateChanged, is_first_match), 0U);
    ASSERT_GT(offsetof(BleScanStateChanged, is_opportunistic), 0U);
}

TEST(TypesafeCodeGenTest, AtomFieldsDefaultValues) {
    using namespace android::stats::typesafe;

    ScreenBrightnessChanged screenBrightnessChanged;
    EXPECT_EQ(screenBrightnessChanged.level, 0);

    // TODO: float field without default value
    // TODO: float field with default value

    // TODO: bool field without default value
    // TODO: bool field with default value

    // TODO: int32 field without default value
    // TODO: int32 field with default value

    // TODO: int64 field without default value
    // TODO: int64 field with default value

    // TODO: string field without default value
    // TODO: string field with default value

    // TODO: enum field without default value
    // TODO: enum field with default value
}

TEST(TypesafeCodeGenTest, PushedAtomWriteFunctions) {
    using namespace android::stats::typesafe;

    typedef int (*WriteBleScanStateChangedFunc)(const BleScanStateChanged&);
    WriteBleScanStateChangedFunc writeBleScanStateChangedFunc = &stats_write;
    EXPECT_NE(writeBleScanStateChangedFunc, nullptr);

    typedef int (*WriteBleScanResultReceivedFunc)(const BleScanResultReceived&);
    WriteBleScanResultReceivedFunc writeBleScanResultReceivedFunc = &stats_write;
    EXPECT_NE(writeBleScanResultReceivedFunc, nullptr);

    typedef int (*WriteSensorStateChangedFunc)(const SensorStateChanged&);
    WriteSensorStateChangedFunc writeSensorStateChangedFunc = &stats_write;
    EXPECT_NE(writeSensorStateChangedFunc, nullptr);

    typedef int (*WriteSyncStateChangedFunc)(const SyncStateChanged&);
    WriteSyncStateChangedFunc writeSyncStateChangedFunc = &stats_write;
    EXPECT_NE(writeSyncStateChangedFunc, nullptr);

    typedef int (*WriteScheduledJobStateChangedFunc)(const ScheduledJobStateChanged&);
    WriteScheduledJobStateChangedFunc writeScheduledJobStateChangedFunc = &stats_write;
    EXPECT_NE(writeScheduledJobStateChangedFunc, nullptr);
}

TEST(TypesafeCodeGenTest, PulledAtomWriteFunctions) {
    using namespace android::stats::typesafe;

    typedef void (*WriteSubsystemSleepStateFunc)(AStatsEventList* pulled_data,
                                                 const SubsystemSleepState&);
    WriteSubsystemSleepStateFunc writeSubsystemSleepStateFunc = &addAStatsEvent;
    EXPECT_NE(writeSubsystemSleepStateFunc, nullptr);

    typedef void (*WriteCpuTimePerUidFunc)(AStatsEventList* pulled_data, const CpuTimePerUid&);
    WriteCpuTimePerUidFunc writeCpuTimePerUidFunc = &addAStatsEvent;
    EXPECT_NE(writeCpuTimePerUidFunc, nullptr);
}

}  // namespace

#else
GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
