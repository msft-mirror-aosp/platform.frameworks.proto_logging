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

#include <statslog_bootstrap_test.h>

#ifdef __ANDROID__

namespace {

using namespace testing;

TEST(BootstrapCodeGenTest, AtomConstants) {
    ASSERT_EQ(android::stats::bs::APEX_INSTALLATION_REQUESTED, 732);
    ASSERT_EQ(android::stats::bs::APEX_INSTALLATION_STAGED, 733);
    ASSERT_EQ(android::stats::bs::APEX_INSTALLATION_ENDED, 734);
}

}  // namespace

#else
GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
