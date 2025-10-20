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

#include <aidl/android/frameworks/stats/VendorAtom.h>
#include <gtest/gtest.h>
#include <test_generic_vendor_atoms.h>

#include <limits>

namespace android {
namespace api_gen_vendor_tests {

using namespace android::GenericVendorAtoms;
using namespace aidl::android::frameworks::stats;

namespace {

static const int32_t kTestIntValue = 100;
static const char* kTestStringValue = "test_string";

}  // namespace

/**
 * Tests native auto generated code for specific vendor atom contains proper ids
 */
TEST(ApiGenericVendorAtomTest, AtomIdConstantsTest) {
    EXPECT_EQ(TEST_GENERIC_VENDOR_ATOM_REPORTED, 349999);
}

TEST(ApiGenericVendorAtomTest, buildGenericVendorAtomApiTest) {
    typedef VendorAtom (*GenericVendorAtomBuildFunc)(int32_t code, char const* arg1, int32_t arg2);
    GenericVendorAtomBuildFunc func = &createVendorAtom;

    EXPECT_NE(func, nullptr);

    VendorAtom atom = func(TEST_GENERIC_VENDOR_ATOM_REPORTED, kTestStringValue, kTestIntValue);

    EXPECT_EQ(atom.atomId, TEST_GENERIC_VENDOR_ATOM_REPORTED);
    EXPECT_EQ(atom.reverseDomainName, kTestStringValue);
    EXPECT_EQ(atom.values.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom.values[0].get<VendorAtomValue::intValue>(), kTestIntValue);
    EXPECT_EQ(atom.atomAnnotations, std::nullopt);
}

}  // namespace api_gen_vendor_tests
}  // namespace android
