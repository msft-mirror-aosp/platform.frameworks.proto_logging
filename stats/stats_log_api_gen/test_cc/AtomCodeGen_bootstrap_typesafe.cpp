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

#include <gtest/gtest.h>

#include <statslog_bootstrap_typesafe.h>

#ifdef __ANDROID__

namespace {

using namespace testing;

TEST(BootstrapBootstrapTypesafeCodeGenTest, AtomEnumFieldsValues) {
    using namespace android::stats::bs::typesafe;

    EXPECT_EQ(static_cast<int>(ApexInstallationRequested::PreinstallPartition::PARTITION_UNKNOWN),
              0);
    EXPECT_EQ(static_cast<int>(ApexInstallationRequested::PreinstallPartition::PARTITION_OTHER), 1);
    EXPECT_EQ(static_cast<int>(ApexInstallationRequested::PreinstallPartition::PARTITION_PRODUCT),
              2);
    EXPECT_EQ(static_cast<int>(ApexInstallationRequested::PreinstallPartition::PARTITION_SYSTEM),
              3);
    EXPECT_EQ(
            static_cast<int>(ApexInstallationRequested::PreinstallPartition::PARTITION_SYSTEM_EXT),
            4);
    EXPECT_EQ(static_cast<int>(ApexInstallationRequested::PreinstallPartition::PARTITION_VENDOR),
              5);
    EXPECT_EQ(static_cast<int>(ApexInstallationRequested::PreinstallPartition::PARTITION_ODM), 6);

    EXPECT_EQ(static_cast<int>(ApexInstallationRequested::InstallationType::UNKNOWN_INSTALL_TYPE),
              0);
    EXPECT_EQ(static_cast<int>(ApexInstallationRequested::InstallationType::STAGED), 1);
    EXPECT_EQ(static_cast<int>(ApexInstallationRequested::InstallationType::REBOOTLESS), 2);

    EXPECT_EQ(static_cast<int>(ApexInstallationEnded::InstallResultType::UNKNOWN_INSTALL_RESULT),
              0);
    EXPECT_EQ(static_cast<int>(ApexInstallationEnded::InstallResultType::INSTALL_SUCCESSFUL), 1);
    EXPECT_EQ(static_cast<int>(
                      ApexInstallationEnded::InstallResultType::INSTALL_FAILURE_APEX_READING),
              2);
    EXPECT_EQ(static_cast<int>(
                      ApexInstallationEnded::InstallResultType::INSTALL_FAILURE_APEX_VALIDATION),
              3);
    EXPECT_EQ(static_cast<int>(
                      ApexInstallationEnded::InstallResultType::INSTALL_FAILURE_APEX_PREPARATION),
              4);
    EXPECT_EQ(static_cast<int>(
                      ApexInstallationEnded::InstallResultType::INSTALL_FAILURE_APEX_STAGING),
              5);
    EXPECT_EQ(static_cast<int>(
                      ApexInstallationEnded::InstallResultType::INSTALL_FAILURE_APEX_INSTALLATION),
              6);
    EXPECT_EQ(static_cast<int>(
                      ApexInstallationEnded::InstallResultType::INSTALL_FAILURE_APEX_ACTIVATION),
              7);
}

TEST(BootstrapTypesafeCodeGenTest, AtomFieldNames) {
    using namespace android::stats::bs::typesafe;

    ASSERT_EQ(offsetof(ApexInstallationRequested, module_name), 0U);
    ASSERT_GT(offsetof(ApexInstallationRequested, version_code), 0U);
    ASSERT_GT(offsetof(ApexInstallationRequested, package_size_bytes), 0U);
    ASSERT_GT(offsetof(ApexInstallationRequested, file_hash), 0U);
    ASSERT_GT(offsetof(ApexInstallationRequested, apex_preinstall_partition), 0U);
    ASSERT_GT(offsetof(ApexInstallationRequested, installation_type), 0U);
    ASSERT_GT(offsetof(ApexInstallationRequested, request_is_rollback), 0U);
    ASSERT_GT(offsetof(ApexInstallationRequested, provides_shared_apex_libs), 0U);
    ASSERT_GT(offsetof(ApexInstallationRequested, provided_hals), 0U);

    ASSERT_GE(offsetof(ApexInstallationEnded, file_hash), 0U);
    ASSERT_GT(offsetof(ApexInstallationEnded, installation_result), 0U);

    ASSERT_GE(offsetof(ApexInstallationStaged, file_hash), 0U);
}

TEST(BootstrapTypesafeCodeGenTest, AtomWriteFunctions) {
    using namespace android::stats::bs::typesafe;

    typedef int (*WriteApexInstallationRequestedFunc)(const ApexInstallationRequested&);
    WriteApexInstallationRequestedFunc writeApexInstallationRequestedFunc = &stats_write;
    EXPECT_NE(writeApexInstallationRequestedFunc, nullptr);

    typedef int (*WriteApexInstallationStagedFunc)(const ApexInstallationStaged&);
    WriteApexInstallationStagedFunc writeApexInstallationStagedFunc = &stats_write;
    EXPECT_NE(writeApexInstallationStagedFunc, nullptr);

    typedef int (*WriteApexInstallationEndedFunc)(const ApexInstallationEnded&);
    WriteApexInstallationEndedFunc writeApexInstallationEndedFunc = &stats_write;
    EXPECT_NE(writeApexInstallationEndedFunc, nullptr);
}

}  // namespace

#else
GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
