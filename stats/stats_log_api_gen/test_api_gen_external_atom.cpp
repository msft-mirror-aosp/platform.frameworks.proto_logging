/*
 * Copyright (C) 2025, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include <test_external_atoms.h>

#include <limits>

namespace android {
namespace api_gen_vendor_tests {

using namespace android::ExternalAtoms;

/**
 * Tests native auto generated code for specific vendor atom contains proper ids
 */
TEST(ApiGenExternalAtomTest, AtomIdConstantsTest) {
    EXPECT_EQ(VENDOR_ATOM1, 105501);
    EXPECT_EQ(VENDOR_ATOM2, 105502);
    EXPECT_EQ(VENDOR_ATOM3, 105503);
    EXPECT_EQ(VENDOR_ATOM4, 105504);
    EXPECT_EQ(VENDOR_ATOM5, 105505);
    EXPECT_EQ(STATE_ATOM1, 105506);
    EXPECT_EQ(STATE_ATOM2, 105507);
    EXPECT_EQ(STATE_ATOM3, 105508);
    EXPECT_EQ(STATE_ATOM4, 105509);
    EXPECT_EQ(TRUNCATE_TIMESTAMP_ATOM1, 105510);
    EXPECT_EQ(TRUNCATE_TIMESTAMP_ATOM2, 105511);
    EXPECT_EQ(VENDOR_ATOM6, 105513);
    EXPECT_EQ(VENDOR_ATOM5_WITH_ANNOTATION, 105514);
}

TEST(ApiGenExternalAtomTest, genExternalAtomApiTest) {
    typedef int (*ExternalAtomWriteFunc1)(int32_t code, char const* arg1, int32_t arg2);
    ExternalAtomWriteFunc1 func1 = &stats_write;
    EXPECT_NE(func1, nullptr);

    typedef int (*ExternalAtomWriteFunc2)(int32_t code, char const* arg1, int32_t arg2, bool arg3);
    ExternalAtomWriteFunc2 func2 = &stats_write;
    EXPECT_NE(func2, nullptr);

    typedef int (*ExternalAtomWriteFunc3)(int32_t code, char const* arg1, int32_t arg2,
                                          int32_t arg3);
    ExternalAtomWriteFunc3 func3 = &stats_write;
    EXPECT_NE(func3, nullptr);

    typedef int (*ExternalAtomWriteFunc4)(int32_t code, char const* arg1, int32_t arg2,
                                          int32_t arg3, int32_t arg4);
    ExternalAtomWriteFunc4 func4 = &stats_write;
    EXPECT_NE(func4, nullptr);

    typedef int (*ExternalAtomWriteFunc5)(int32_t code, char const* arg1, int32_t arg2,
                                          int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
    ExternalAtomWriteFunc5 func5 = &stats_write;
    EXPECT_NE(func5, nullptr);

    typedef int (*ExternalAtomWriteFunc6)(int32_t code, char const* arg1, int32_t arg2,
                                          int32_t arg3, int32_t arg4, int64_t arg5, float arg6,
                                          bool arg7, int32_t arg8, int32_t arg9);
    ExternalAtomWriteFunc6 func6 = &stats_write;
    EXPECT_NE(func6, nullptr);

    typedef int (*ExternalAtomWriteFunc7)(int32_t code, char const* arg1, float arg2, int32_t arg3,
                                          int64_t arg4, const BytesField& arg5);
    ExternalAtomWriteFunc7 func7 = &stats_write;
    EXPECT_NE(func7, nullptr);

    typedef int (*ExternalAtomWriteFunc8)(
            int32_t code, char const* arg1, float arg2, int32_t arg3, int64_t arg4, bool arg5,
            int32_t arg6, const bool* arg7, size_t arg7_length, const std::vector<float>& arg8,
            const std::vector<int32_t>& arg9, const std::vector<int64_t>& arg10,
            const std::vector<char const*>& arg11, const std::vector<int32_t>& arg12);
    ExternalAtomWriteFunc8 func8 = &stats_write;
    EXPECT_NE(func8, nullptr);
}

}  // namespace api_gen_vendor_tests
}  // namespace android
