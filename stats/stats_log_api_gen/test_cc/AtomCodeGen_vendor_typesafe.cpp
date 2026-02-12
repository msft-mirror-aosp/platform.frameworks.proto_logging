/*
 * Copyright (C) 2026, The Android Open Source Project
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
#include <test_vendor_atoms_typesafe.h>

#include <limits>

namespace android {
namespace api_gen_vendor_tests {

using namespace aidl::android::frameworks::stats;
using namespace android::VendorAtoms::typesafe;

/**
 * Tests native auto generated code for specific vendor atom contains proper enums
 */
TEST(ApiGenVendorAtomTypesafeTest, AtomEnumFieldsValues) {
    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType::TYPE_UNKNOWN), 0);
    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType::TYPE_1), 1);
    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType::TYPE_2), 2);
    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType::TYPE_3), 3);

    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType2::ANOTHER_TYPE_UNKNOWN), 0);
    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType2::ANOTHER_TYPE_1), 1);
    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType2::ANOTHER_TYPE_2), 2);
    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType2::ANOTHER_TYPE_3), 3);

    EXPECT_EQ(static_cast<int>(VendorAtom2::EnumType::TYPE_UNKNOWN), 0);
    EXPECT_EQ(static_cast<int>(VendorAtom2::EnumType::TYPE_1), 1);
    EXPECT_EQ(static_cast<int>(VendorAtom2::EnumType::TYPE_2), 2);
    EXPECT_EQ(static_cast<int>(VendorAtom2::EnumType::TYPE_3), 3);

    EXPECT_EQ(static_cast<int>(VendorAtom2::EnumType2::ANOTHER_TYPE_UNKNOWN), 0);
    EXPECT_EQ(static_cast<int>(VendorAtom2::EnumType2::ANOTHER_TYPE_1), 1);
    EXPECT_EQ(static_cast<int>(VendorAtom2::EnumType2::ANOTHER_TYPE_2), 2);
    EXPECT_EQ(static_cast<int>(VendorAtom2::EnumType2::ANOTHER_TYPE_3), 3);

    EXPECT_EQ(static_cast<int>(VendorAtom4::EnumType4::TYPE_UNKNOWN), 0);
    EXPECT_EQ(static_cast<int>(VendorAtom4::EnumType4::TYPE_1), 1);

    EXPECT_EQ(static_cast<int>(VendorAtom4::EnumType5::TYPE_5_UNKNOWN), 0);
    EXPECT_EQ(static_cast<int>(VendorAtom4::EnumType5::TYPE_5_1), 1);
    EXPECT_EQ(static_cast<int>(VendorAtom4::EnumType5::TYPE_5_2), 2);
}

/**
 * Tests native auto generated code for specific vendor atom contains proper field names
 */
TEST(ApiGenVendorAtomTypesafeTest, AtomFieldNames) {
    ASSERT_EQ(offsetof(VendorAtom1, reverse_domain_name), 0U);
    ASSERT_GT(offsetof(VendorAtom1, enum_field1), 0U);
    ASSERT_GT(offsetof(VendorAtom1, enum_field2), 0U);
    ASSERT_GT(offsetof(VendorAtom1, int_value32), 0U);
    ASSERT_GT(offsetof(VendorAtom1, int_value64), 0U);
    ASSERT_GT(offsetof(VendorAtom1, float_value), 0U);
    ASSERT_GT(offsetof(VendorAtom1, bool_value), 0U);
    ASSERT_GT(offsetof(VendorAtom1, enum_field3), 0U);
    ASSERT_GT(offsetof(VendorAtom1, enum_field4), 0U);
}

TEST(ApiGenVendorAtomTypesafeTest, AtomWriteFunctions) {
    typedef VendorAtom (*WriteVendorAtom1Func)(const VendorAtom1&);
    WriteVendorAtom1Func writeVendorAtom1Func = &createVendorAtom;
    EXPECT_NE(writeVendorAtom1Func, nullptr);

    typedef VendorAtom (*WriteVendorAtom2Func)(const VendorAtom2&);
    WriteVendorAtom2Func writeVendorAtom2Func = &createVendorAtom;
    EXPECT_NE(writeVendorAtom2Func, nullptr);

    typedef VendorAtom (*WriteVendorAtom3Func)(const VendorAtom3&);
    WriteVendorAtom3Func writeVendorAtom3Func = &createVendorAtom;
    EXPECT_NE(writeVendorAtom3Func, nullptr);

    typedef VendorAtom (*WriteVendorAtom4Func)(const VendorAtom4&);
    WriteVendorAtom4Func writeVendorAtom4Func = &createVendorAtom;
    EXPECT_NE(writeVendorAtom4Func, nullptr);

    typedef VendorAtom (*WriteVendorAtom5Func)(const VendorAtom5&);
    WriteVendorAtom5Func writeVendorAtom5Func = &createVendorAtom;
    EXPECT_NE(writeVendorAtom5Func, nullptr);

    typedef VendorAtom (*WriteVendorAtomWithStateFunc)(const VendorAtomWithState&);
    WriteVendorAtomWithStateFunc writeVendorAtomWithStateFunc = &createVendorAtom;
    EXPECT_NE(writeVendorAtomWithStateFunc, nullptr);

    typedef VendorAtom (*WriteVendorAtomWithState2Func)(const VendorAtomWithState2&);
    WriteVendorAtomWithState2Func writeVendorAtomWithState2Func = &createVendorAtom;
    EXPECT_NE(writeVendorAtomWithState2Func, nullptr);

    typedef VendorAtom (*WriteVendorAtomWithState3Func)(const VendorAtomWithState3&);
    WriteVendorAtomWithState3Func writeVendorAtomWithState3Func = &createVendorAtom;
    EXPECT_NE(writeVendorAtomWithState3Func, nullptr);

    typedef VendorAtom (*WriteVendorAtomWithState4Func)(const VendorAtomWithState4&);
    WriteVendorAtomWithState4Func writeVendorAtomWithState4Func = &createVendorAtom;
    EXPECT_NE(writeVendorAtomWithState4Func, nullptr);

    typedef VendorAtom (*WriteVendorAtomWithTruncateTimestampFunc)(
            const VendorAtomWithTruncateTimestamp&);
    WriteVendorAtomWithTruncateTimestampFunc writeVendorAtomWithTruncateTimestampFunc =
            &createVendorAtom;
    EXPECT_NE(writeVendorAtomWithTruncateTimestampFunc, nullptr);

    typedef VendorAtom (*WriteVendorAtomWithTruncateTimestamp2Func)(
            const VendorAtomWithTruncateTimestamp2&);
    WriteVendorAtomWithTruncateTimestamp2Func writeVendorAtomWithTruncateTimestamp2Func =
            &createVendorAtom;
    EXPECT_NE(writeVendorAtomWithTruncateTimestamp2Func, nullptr);

    typedef VendorAtom (*WriteVendorAtomWithTruncateTimestamp3Func)(
            const VendorAtomWithTruncateTimestamp3&);
    WriteVendorAtomWithTruncateTimestamp3Func writeVendorAtomWithTruncateTimestamp3Func =
            &createVendorAtom;
    EXPECT_NE(writeVendorAtomWithTruncateTimestamp3Func, nullptr);

    typedef VendorAtom (*WriteVendorAtom6Func)(const VendorAtom6&);
    WriteVendorAtom6Func writeVendorAtom6Func = &createVendorAtom;
    EXPECT_NE(writeVendorAtom6Func, nullptr);

    typedef VendorAtom (*WriteVendorAtom5WithAnnotationFunc)(const VendorAtom5WithAnnotation&);
    WriteVendorAtom5WithAnnotationFunc writeVendorAtom5WithAnnotationFunc = &createVendorAtom;
    EXPECT_NE(writeVendorAtom5WithAnnotationFunc, nullptr);

    typedef VendorAtom (*WriteVendorAtom5WithStateFunc)(const VendorAtom5WithState&);
    WriteVendorAtom5WithStateFunc writeVendorAtom5WithStateFunc = &createVendorAtom;
    EXPECT_NE(writeVendorAtom5WithStateFunc, nullptr);
}

}  // namespace api_gen_vendor_tests
}  // namespace android
