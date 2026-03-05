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
#include <frameworks/proto_logging/stats/stats_log_api_gen/test_vendor_atoms.pb.h>
#include <gtest/gtest.h>
#include <test_vendor_atoms_typesafe.h>

#include <limits>

namespace android {
namespace api_gen_vendor_tests {

using namespace aidl::android::frameworks::stats;
using namespace android::VendorAtoms::typesafe;

using std::string;
using std::vector;

namespace {

static const int32_t kTestIntValue = 100;
static const int32_t kTestUidValue = 1000;
static const int32_t kTestPidValue = 3000;
static const int64_t kTestLongValue = std::numeric_limits<int64_t>::max() - kTestIntValue;
static const float kTestFloatValue = (float)kTestIntValue / kTestLongValue;
static const bool kTestBoolValue = true;
static const char* kTestStringValue = "test_string";
static const char* kTestStringValue2 = "test_string2";

}  // namespace

/**
 * Tests native auto generated code for specific vendor atom contains proper enums
 */
TEST(ApiGenVendorAtomTypesafeTest, AtomEnumFieldsValues) {

    // tests confirm scoped and non-scoped enum values access
    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType::TYPE_UNKNOWN), 0);
    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType::TYPE_1), 1);
    EXPECT_EQ(static_cast<int>(VendorAtom1::TYPE_2), 2);
    EXPECT_EQ(static_cast<int>(VendorAtom1::TYPE_3), 3);

    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType2::ANOTHER_TYPE_UNKNOWN), 0);
    EXPECT_EQ(static_cast<int>(VendorAtom1::EnumType2::ANOTHER_TYPE_1), 1);
    EXPECT_EQ(static_cast<int>(VendorAtom1::ANOTHER_TYPE_2), 2);
    EXPECT_EQ(static_cast<int>(VendorAtom1::ANOTHER_TYPE_3), 3);

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

TEST(ApiGenVendorAtomTypesafeTest, AtomFieldCount) {
    ASSERT_EQ(VendorAtom1::kFieldsCount, 9);
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

    typedef VendorAtom (*WriteVendorAtom6Func)(const VendorAtom6&);
    WriteVendorAtom6Func writeVendorAtom6Func = &createVendorAtom;
    EXPECT_NE(writeVendorAtom6Func, nullptr);

    typedef VendorAtom (*WriteVendorAtom5WithAnnotationFunc)(const VendorAtom5WithAnnotation&);
    WriteVendorAtom5WithAnnotationFunc writeVendorAtom5WithAnnotationFunc = &createVendorAtom;
    EXPECT_NE(writeVendorAtom5WithAnnotationFunc, nullptr);
}

TEST(ApiGenVendorAtomTypesafeTest, buildVendorAtom1ApiTest) {
    typedef VendorAtom (*VendorAtom1BuildFunc)(const VendorAtom1& atom);
    VendorAtom1BuildFunc func = &createVendorAtom;

    ASSERT_NE(func, nullptr);

    VendorAtom1 atomData{.reverse_domain_name = kTestStringValue,
                         .enum_field1 = VendorAtom1::EnumType::TYPE_1,
                         .enum_field2 = VendorAtom1::EnumType::TYPE_2,
                         .int_value32 = kTestIntValue,
                         .int_value64 = kTestLongValue,
                         .float_value = kTestFloatValue,
                         .bool_value = kTestBoolValue,
                         .enum_field3 = VendorAtom1::EnumType2::ANOTHER_TYPE_2,
                         .enum_field4 = VendorAtom1::EnumType2::ANOTHER_TYPE_3};

    VendorAtom atom = func(atomData);

    EXPECT_EQ(atom.atomId, stats_log_api_gen::Atom::kVendorAtom1FieldNumber);
    EXPECT_EQ(atom.reverseDomainName, kTestStringValue);
    ASSERT_EQ(atom.values.size(), static_cast<size_t>(8));
    EXPECT_EQ(atom.values[0].get<VendorAtomValue::intValue>(),
              static_cast<int>(VendorAtom1::EnumType::TYPE_1));
    EXPECT_EQ(atom.values[1].get<VendorAtomValue::intValue>(),
              static_cast<int>(VendorAtom1::EnumType::TYPE_2));
    EXPECT_EQ(atom.values[2].get<VendorAtomValue::intValue>(), kTestIntValue);
    EXPECT_EQ(atom.values[3].get<VendorAtomValue::longValue>(), kTestLongValue);
    EXPECT_EQ(atom.values[4].get<VendorAtomValue::floatValue>(), kTestFloatValue);
    EXPECT_EQ(atom.values[5].get<VendorAtomValue::boolValue>(), kTestBoolValue);
    EXPECT_EQ(atom.values[6].get<VendorAtomValue::intValue>(),
              static_cast<int>(VendorAtom1::EnumType2::ANOTHER_TYPE_2));
    EXPECT_EQ(atom.values[7].get<VendorAtomValue::intValue>(),
              static_cast<int>(VendorAtom1::EnumType2::ANOTHER_TYPE_3));
    EXPECT_EQ(atom.atomAnnotations, std::nullopt);
}

TEST(ApiGenVendorAtomTypesafeTest, buildVendorAtom3ApiTest) {
    typedef VendorAtom (*VendorAtom3BuildFunc)(const VendorAtom3& atom);
    VendorAtom3BuildFunc func = &createVendorAtom;

    ASSERT_NE(func, nullptr);

    VendorAtom3 atomData{.reverse_domain_name = kTestStringValue, .int_field = kTestIntValue};

    VendorAtom atom = func(atomData);
    EXPECT_EQ(atom.atomId, stats_log_api_gen::Atom::kVendorAtom3FieldNumber);
    EXPECT_EQ(atom.reverseDomainName, kTestStringValue);
    ASSERT_EQ(atom.values.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom.values[0].get<VendorAtomValue::intValue>(), kTestIntValue);
    EXPECT_EQ(atom.atomAnnotations, std::nullopt);
}

TEST(ApiGenVendorAtomTypesafeTest, buildVendorAtom4ApiTest) {
    typedef VendorAtom (*VendorAtom4BuildFunc)(const VendorAtom4& atom);
    VendorAtom4BuildFunc func = &createVendorAtom;

    ASSERT_NE(func, nullptr);

    VendorAtom4 atomData{
            .reverse_domain_name = kTestStringValue,
            .float_field = kTestFloatValue,
            .int_field = kTestIntValue,
            .long_field = kTestLongValue,
            .bool_field = kTestBoolValue,
            .enum_field = VendorAtom4::EnumType4::TYPE_1,
            .bool_repeated = {true, false, true},
            .float_repeated = {kTestFloatValue, kTestFloatValue + 1.f, kTestFloatValue + 2.f},
            .int_repeated = {kTestIntValue, kTestIntValue + 1, kTestIntValue + 2},
            .long_repeated = {kTestLongValue, kTestLongValue + 1, kTestLongValue + 2},
            .string_repeated = {kTestStringValue, kTestStringValue2, kTestStringValue},
            .enum_repeated = {VendorAtom4::EnumType5::TYPE_5_1, VendorAtom4::EnumType5::TYPE_5_2}};

    const vector<int> kRepeatedEnum{static_cast<int>(atomData.enum_repeated[0]),
                                    static_cast<int>(atomData.enum_repeated[1])};
    const vector<bool> kRepeatedBool{atomData.bool_repeated.begin(), atomData.bool_repeated.end()};

    VendorAtom atom = func(atomData);

    EXPECT_EQ(atom.atomId, stats_log_api_gen::Atom::kVendorAtom4FieldNumber);
    EXPECT_EQ(atom.reverseDomainName, kTestStringValue);
    ASSERT_EQ(atom.values.size(), static_cast<size_t>(11));
    EXPECT_EQ(atom.values[0].get<VendorAtomValue::floatValue>(), kTestFloatValue);
    EXPECT_EQ(atom.values[1].get<VendorAtomValue::intValue>(), kTestIntValue);
    EXPECT_EQ(atom.values[2].get<VendorAtomValue::longValue>(), kTestLongValue);
    EXPECT_EQ(atom.values[3].get<VendorAtomValue::boolValue>(), kTestBoolValue);
    EXPECT_EQ(atom.values[4].get<VendorAtomValue::intValue>(),
              static_cast<int>(VendorAtom4::EnumType4::TYPE_1));
    EXPECT_EQ(atom.values[5].get<VendorAtomValue::repeatedBoolValue>(), kRepeatedBool);
    EXPECT_EQ(atom.values[6].get<VendorAtomValue::repeatedFloatValue>(), atomData.float_repeated);
    EXPECT_EQ(atom.values[7].get<VendorAtomValue::repeatedIntValue>(), atomData.int_repeated);
    EXPECT_EQ(atom.values[8].get<VendorAtomValue::repeatedLongValue>(), atomData.long_repeated);
    EXPECT_TRUE(atom.values[9].get<VendorAtomValue::repeatedStringValue>().has_value());
    EXPECT_EQ(atom.values[9].get<VendorAtomValue::repeatedStringValue>()->size(),
              atomData.string_repeated.size());
    const auto& repeatedStringValue = *atom.values[9].get<VendorAtomValue::repeatedStringValue>();
    for (size_t i = 0; i < atomData.string_repeated.size(); i++) {
        EXPECT_EQ(atomData.string_repeated[i], *repeatedStringValue[i]);
    }
    EXPECT_EQ(atom.values[10].get<VendorAtomValue::repeatedIntValue>(), kRepeatedEnum);
    EXPECT_EQ(atom.atomAnnotations, std::nullopt);
}

TEST(ApiGenVendorAtomTypesafeTest, buildVendorAtom5ApiTest) {
    typedef VendorAtom (*VendorAtom5BuildFunc)(const VendorAtom5&);
    VendorAtom5BuildFunc func = &createVendorAtom;

    ASSERT_NE(func, nullptr);

    ::android::stats_log_api_gen::TestNestedMessage nestedMessage;
    nestedMessage.set_float_field(kTestFloatValue);
    nestedMessage.set_int_field(kTestIntValue);
    nestedMessage.set_long_field(kTestLongValue);

    string nestedMessageString;
    nestedMessage.SerializeToString(&nestedMessageString);

    vector<uint8_t> nestedMessageBytes(nestedMessageString.begin(), nestedMessageString.end());

    VendorAtom5 atomData{.reverse_domain_name = kTestStringValue,
                         .float_field = kTestFloatValue,
                         .int_field = kTestIntValue,
                         .long_field = kTestLongValue,
                         .nested_message_field = nestedMessageBytes};

    VendorAtom atom = func(atomData);

    EXPECT_EQ(atom.atomId, stats_log_api_gen::Atom::kVendorAtom5FieldNumber);
    EXPECT_EQ(atom.reverseDomainName, kTestStringValue);
    ASSERT_EQ(atom.values.size(), static_cast<size_t>(4));
    EXPECT_EQ(atom.values[0].get<VendorAtomValue::floatValue>(), kTestFloatValue);
    EXPECT_EQ(atom.values[1].get<VendorAtomValue::intValue>(), kTestIntValue);
    EXPECT_EQ(atom.values[2].get<VendorAtomValue::longValue>(), kTestLongValue);
    EXPECT_EQ(atom.values[3].get<VendorAtomValue::byteArrayValue>(), nestedMessageBytes);
    EXPECT_EQ(atom.atomAnnotations, std::nullopt);

    string nestedMessageStringResult(atom.values[3].get<VendorAtomValue::byteArrayValue>()->begin(),
                                     atom.values[3].get<VendorAtomValue::byteArrayValue>()->end());
    EXPECT_EQ(nestedMessageStringResult, nestedMessageString);

    ::android::stats_log_api_gen::TestNestedMessage nestedMessageResult;
    nestedMessageResult.ParseFromString(nestedMessageStringResult);
    EXPECT_EQ(nestedMessageResult.float_field(), kTestFloatValue);
    EXPECT_EQ(nestedMessageResult.int_field(), kTestIntValue);
    EXPECT_EQ(nestedMessageResult.long_field(), kTestLongValue);
}

TEST(ApiGenVendorAtomTypesafeTest, buildAtomWithTruncateTimestampTest) {
    /**
     * Expected signature equal to VendorAtomWithTruncateTimestampCreateFunc to log
     * 2 different atoms with truncate_timestamp
     *      VendorAtomWithTruncateTimestamp truncateTimestampAtom1 = 105510 [
     *          (android.os.statsd.truncate_timestamp) = true
     *      ];
     *      VendorAtomWithTruncateTimestamp2 truncateTimestampAtom2 = 105511 [
     *          (android.os.statsd.truncate_timestamp) = true
     *      ];
     */
    typedef VendorAtom (*VendorAtomWithTruncateTimestampCreateFunc)(
            const VendorAtomWithTruncateTimestamp&);
    VendorAtomWithTruncateTimestampCreateFunc func = &createVendorAtom;

    typedef VendorAtom (*VendorAtomWithTruncateTimestamp2CreateFunc)(
            const VendorAtomWithTruncateTimestamp2&);
    VendorAtomWithTruncateTimestamp2CreateFunc func2 = &createVendorAtom;

    ASSERT_NE(func, nullptr);
    ASSERT_NE(func2, nullptr);

    VendorAtomWithTruncateTimestamp atom1Data{
            .reverse_domain_name = kTestStringValue,
            .state = VendorAtomWithTruncateTimestamp::TestState::TEST_STATE_1};

    VendorAtom atom1 = func(atom1Data);
    EXPECT_EQ(atom1.atomId, stats_log_api_gen::Atom::kTruncateTimestampAtom1FieldNumber);
    EXPECT_EQ(atom1.reverseDomainName, kTestStringValue);
    ASSERT_EQ(atom1.values.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom1.values[0].get<VendorAtomValue::intValue>(),
              static_cast<int>(VendorAtomWithTruncateTimestamp::TestState::TEST_STATE_1));
    ASSERT_NE(atom1.atomAnnotations, std::nullopt);
    ASSERT_EQ(atom1.atomAnnotations->size(), static_cast<size_t>(1));
    ASSERT_NE(atom1.atomAnnotations.value()[0], std::nullopt);
    EXPECT_EQ(atom1.atomAnnotations.value()[0]->annotationId, AnnotationId::TRUNCATE_TIMESTAMP);
    EXPECT_TRUE(atom1.atomAnnotations.value()[0]->value.get<AnnotationValue::boolValue>());

    VendorAtomWithTruncateTimestamp2 atom2Data{.reverse_domain_name = kTestStringValue,
                                               .int_value = kTestIntValue};

    VendorAtom atom2 = func2(atom2Data);
    EXPECT_EQ(atom2.atomId, stats_log_api_gen::Atom::kTruncateTimestampAtom2FieldNumber);
    EXPECT_EQ(atom2.reverseDomainName, kTestStringValue);
    ASSERT_EQ(atom2.values.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom2.values[0].get<VendorAtomValue::intValue>(), kTestIntValue);
    ASSERT_NE(atom2.atomAnnotations, std::nullopt);
    ASSERT_EQ(atom2.atomAnnotations->size(), static_cast<size_t>(1));
    ASSERT_NE(atom2.atomAnnotations.value()[0], std::nullopt);
    EXPECT_EQ(atom2.atomAnnotations.value()[0]->annotationId, AnnotationId::TRUNCATE_TIMESTAMP);
    EXPECT_TRUE(atom2.atomAnnotations.value()[0]->value.get<AnnotationValue::boolValue>());
}

TEST(ApiGenVendorAtomTypesafeTest, buildAtomWithExclusiveStateAnnotationTest) {
    typedef VendorAtom (*VendorAtomWithStateCreateFunc)(const VendorAtomWithState3&);
    VendorAtomWithStateCreateFunc func = &createVendorAtom;

    ASSERT_NE(func, nullptr);

    VendorAtomWithState3 atomData{.reverse_domain_name = kTestStringValue,
                                  .state = VendorAtomWithState3::TestState::TEST_STATE_3};

    VendorAtom atom = func(atomData);
    EXPECT_EQ(atom.atomId, stats_log_api_gen::Atom::kStateAtom3FieldNumber);
    EXPECT_EQ(atom.reverseDomainName, kTestStringValue);
    ASSERT_EQ(atom.values.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom.values[0].get<VendorAtomValue::intValue>(),
              static_cast<int>(VendorAtomWithState3::TestState::TEST_STATE_3));
    ASSERT_NE(atom.valuesAnnotations, std::nullopt);
    EXPECT_EQ(atom.valuesAnnotations->size(), static_cast<size_t>(1));
    ASSERT_NE(atom.valuesAnnotations.value()[0], std::nullopt);
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->valueIndex, 0);
    ASSERT_EQ(atom.valuesAnnotations.value()[0]->annotations.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->annotations[0].annotationId,
              AnnotationId::EXCLUSIVE_STATE);
    EXPECT_TRUE(atom.valuesAnnotations.value()[0]
                        ->annotations[0]
                        .value.get<AnnotationValue::boolValue>());

    EXPECT_EQ(atom.atomAnnotations, std::nullopt);
}

TEST(ApiGenVendorAtomTypesafeTest, buildAtomWithExclusiveStateAndTwoPrimaryFieldAnnotationTest) {
    typedef VendorAtom (*VendorAtomWithStateCreateFunc)(const VendorAtomWithState2&);
    VendorAtomWithStateCreateFunc func = &createVendorAtom;

    ASSERT_NE(func, nullptr);

    VendorAtomWithState2 atomData{.reverse_domain_name = kTestStringValue,
                                  .uid = kTestUidValue,
                                  .pid = kTestPidValue,
                                  .state = VendorAtomWithState2::TestState::TEST_STATE_2};

    VendorAtom atom = func(atomData);
    EXPECT_EQ(atom.atomId, stats_log_api_gen::Atom::kStateAtom2FieldNumber);
    EXPECT_EQ(atom.reverseDomainName, kTestStringValue);
    ASSERT_EQ(atom.values.size(), static_cast<size_t>(3));
    EXPECT_EQ(atom.values[0].get<VendorAtomValue::intValue>(), kTestUidValue);
    EXPECT_EQ(atom.values[1].get<VendorAtomValue::intValue>(), kTestPidValue);
    EXPECT_EQ(atom.values[2].get<VendorAtomValue::intValue>(),
              static_cast<int>(VendorAtomWithState2::TestState::TEST_STATE_2));
    ASSERT_NE(atom.valuesAnnotations, std::nullopt);
    ASSERT_EQ(atom.valuesAnnotations->size(), static_cast<size_t>(3));
    ASSERT_NE(atom.valuesAnnotations.value()[0], std::nullopt);
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->valueIndex, 0);
    ASSERT_EQ(atom.valuesAnnotations.value()[0]->annotations.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->annotations[0].annotationId,
              AnnotationId::PRIMARY_FIELD);
    EXPECT_TRUE(atom.valuesAnnotations.value()[0]
                        ->annotations[0]
                        .value.get<AnnotationValue::boolValue>());
    ASSERT_NE(atom.valuesAnnotations.value()[1], std::nullopt);
    EXPECT_EQ(atom.valuesAnnotations.value()[1]->valueIndex, 1);
    ASSERT_EQ(atom.valuesAnnotations.value()[1]->annotations.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom.valuesAnnotations.value()[1]->annotations[0].annotationId,
              AnnotationId::PRIMARY_FIELD);
    EXPECT_TRUE(atom.valuesAnnotations.value()[1]
                        ->annotations[0]
                        .value.get<AnnotationValue::boolValue>());
    ASSERT_NE(atom.valuesAnnotations.value()[2], std::nullopt);
    EXPECT_EQ(atom.valuesAnnotations.value()[2]->valueIndex, 2);
    ASSERT_EQ(atom.valuesAnnotations.value()[2]->annotations.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom.valuesAnnotations.value()[2]->annotations[0].annotationId,
              AnnotationId::EXCLUSIVE_STATE);
    EXPECT_TRUE(atom.valuesAnnotations.value()[2]
                        ->annotations[0]
                        .value.get<AnnotationValue::boolValue>());

    EXPECT_EQ(atom.atomAnnotations, std::nullopt);
}

TEST(ApiGenVendorAtomTypesafeTest, buildAtomWithMultipleAnnotationsPerValueTest) {
    typedef VendorAtom (*VendorAtomWithStateCreateFunc)(const VendorAtomWithState4&);
    VendorAtomWithStateCreateFunc func = &createVendorAtom;

    ASSERT_NE(func, nullptr);

    VendorAtomWithState4 atomData{.reverse_domain_name = kTestStringValue,
                                  .state = VendorAtomWithState4::State::ON,
                                  .some_flag = kTestBoolValue};

    VendorAtom atom = func(atomData);
    EXPECT_EQ(atom.atomId, stats_log_api_gen::Atom::kStateAtom4FieldNumber);
    EXPECT_EQ(atom.reverseDomainName, kTestStringValue);
    ASSERT_EQ(atom.values.size(), static_cast<size_t>(2));
    EXPECT_EQ(atom.values[0].get<VendorAtomValue::intValue>(),
              static_cast<int>(VendorAtomWithState4::State::ON));
    EXPECT_EQ(atom.values[1].get<VendorAtomValue::boolValue>(), kTestBoolValue);
    ASSERT_NE(atom.valuesAnnotations, std::nullopt);
    ASSERT_EQ(atom.valuesAnnotations->size(), static_cast<size_t>(2));
    ASSERT_NE(atom.valuesAnnotations.value()[0], std::nullopt);
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->valueIndex, 0);
    ASSERT_EQ(atom.valuesAnnotations.value()[0]->annotations.size(), static_cast<size_t>(2));
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->annotations[0].annotationId,
              AnnotationId::EXCLUSIVE_STATE);
    EXPECT_TRUE(atom.valuesAnnotations.value()[0]
                        ->annotations[0]
                        .value.get<AnnotationValue::boolValue>());
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->annotations[1].annotationId,
              AnnotationId::STATE_NESTED);
    EXPECT_TRUE(atom.valuesAnnotations.value()[0]
                        ->annotations[1]
                        .value.get<AnnotationValue::boolValue>());
    ASSERT_NE(atom.valuesAnnotations.value()[1], std::nullopt);
    EXPECT_EQ(atom.valuesAnnotations.value()[1]->valueIndex, 1);
    ASSERT_EQ(atom.valuesAnnotations.value()[1]->annotations.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom.valuesAnnotations.value()[1]->annotations[0].annotationId,
              AnnotationId::PRIMARY_FIELD);
    EXPECT_EQ(atom.atomAnnotations, std::nullopt);
}

TEST(ApiGenVendorAtomTypesafeTest, buildAtomWithTriggerResetAnnotationTest) {
    typedef VendorAtom (*VendorAtomWithStateCreateFunc)(const VendorAtomWithState4&);
    VendorAtomWithStateCreateFunc func = &createVendorAtom;

    ASSERT_NE(func, nullptr);

    VendorAtomWithState4 atomData{.reverse_domain_name = kTestStringValue,
                                  .state = VendorAtomWithState4::State::RESET,
                                  .some_flag = kTestBoolValue};

    const int kDefaultStateValue = static_cast<int>(VendorAtomWithState4::State::OFF);

    VendorAtom atom = func(atomData);
    EXPECT_EQ(atom.atomId, stats_log_api_gen::Atom::kStateAtom4FieldNumber);
    EXPECT_EQ(atom.reverseDomainName, kTestStringValue);
    ASSERT_EQ(atom.values.size(), static_cast<size_t>(2));
    EXPECT_EQ(atom.values[0].get<VendorAtomValue::intValue>(),
              static_cast<int>(VendorAtomWithState4::State::RESET));
    EXPECT_EQ(atom.values[1].get<VendorAtomValue::boolValue>(), kTestBoolValue);
    ASSERT_NE(atom.valuesAnnotations, std::nullopt);
    ASSERT_EQ(atom.valuesAnnotations->size(), static_cast<size_t>(2));
    ASSERT_NE(atom.valuesAnnotations.value()[0], std::nullopt);
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->valueIndex, 0);
    ASSERT_EQ(atom.valuesAnnotations.value()[0]->annotations.size(), static_cast<size_t>(3));
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->annotations[0].annotationId,
              AnnotationId::EXCLUSIVE_STATE);
    EXPECT_TRUE(atom.valuesAnnotations.value()[0]
                        ->annotations[0]
                        .value.get<AnnotationValue::boolValue>());
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->annotations[1].annotationId,
              AnnotationId::STATE_NESTED);
    EXPECT_TRUE(atom.valuesAnnotations.value()[0]
                        ->annotations[1]
                        .value.get<AnnotationValue::boolValue>());
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->annotations[2].annotationId,
              AnnotationId::TRIGGER_STATE_RESET);
    EXPECT_EQ(atom.valuesAnnotations.value()[0]
                      ->annotations[2]
                      .value.get<AnnotationValue::intValue>(),
              kDefaultStateValue);
    ASSERT_NE(atom.valuesAnnotations.value()[1], std::nullopt);
    EXPECT_EQ(atom.valuesAnnotations.value()[1]->valueIndex, 1);
    ASSERT_EQ(atom.valuesAnnotations.value()[1]->annotations.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom.valuesAnnotations.value()[1]->annotations[0].annotationId,
              AnnotationId::PRIMARY_FIELD);
    EXPECT_EQ(atom.atomAnnotations, std::nullopt);
}

TEST(ApiGenVendorAtomTypesafeTest, buildAtomWithExclusiveStateAndPrimaryFieldAnnotationTest) {
    typedef VendorAtom (*VendorAtomWithStateCreateFunc)(const VendorAtomWithState&);
    VendorAtomWithStateCreateFunc func = &createVendorAtom;

    ASSERT_NE(func, nullptr);

    VendorAtomWithState atomData{.reverse_domain_name = kTestStringValue,
                                 .uid = kTestUidValue,
                                 .state = VendorAtomWithState::TestState::TEST_STATE_1};

    VendorAtom atom = func(atomData);
    EXPECT_EQ(atom.atomId, stats_log_api_gen::Atom::kStateAtom1FieldNumber);
    EXPECT_EQ(atom.reverseDomainName, kTestStringValue);
    ASSERT_EQ(atom.values.size(), static_cast<size_t>(2));
    EXPECT_EQ(atom.values[0].get<VendorAtomValue::intValue>(), kTestUidValue);
    EXPECT_EQ(atom.values[1].get<VendorAtomValue::intValue>(),
              static_cast<int>(VendorAtomWithState::TestState::TEST_STATE_1));
    ASSERT_NE(atom.valuesAnnotations, std::nullopt);
    ASSERT_EQ(atom.valuesAnnotations->size(), static_cast<size_t>(2));
    ASSERT_NE(atom.valuesAnnotations.value()[0], std::nullopt);
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->valueIndex, 0);
    ASSERT_EQ(atom.valuesAnnotations.value()[0]->annotations.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom.valuesAnnotations.value()[0]->annotations[0].annotationId,
              AnnotationId::PRIMARY_FIELD);
    EXPECT_TRUE(atom.valuesAnnotations.value()[0]
                        ->annotations[0]
                        .value.get<AnnotationValue::boolValue>());
    ASSERT_NE(atom.valuesAnnotations.value()[1], std::nullopt);
    EXPECT_EQ(atom.valuesAnnotations.value()[1]->valueIndex, 1);
    ASSERT_EQ(atom.valuesAnnotations.value()[1]->annotations.size(), static_cast<size_t>(1));
    EXPECT_EQ(atom.valuesAnnotations.value()[1]->annotations[0].annotationId,
              AnnotationId::EXCLUSIVE_STATE);
    EXPECT_TRUE(atom.valuesAnnotations.value()[1]
                        ->annotations[0]
                        .value.get<AnnotationValue::boolValue>());

    EXPECT_EQ(atom.atomAnnotations, std::nullopt);
}

TEST(ApiGenVendorAtomTypesafeTest, AtomFieldsDefaultValues) {
    VendorAtom4 atomData;

    EXPECT_EQ(atomData.reverse_domain_name, kTestStringValue);
    EXPECT_EQ(atomData.float_field, -1.1f);
    EXPECT_EQ(atomData.int_field, 15);
    EXPECT_EQ(atomData.long_field, -67);
    EXPECT_EQ(atomData.bool_field, true);
    EXPECT_EQ(atomData.enum_field, VendorAtom4::EnumType4::TYPE_1);

    VendorAtom4 atomData2{};
    EXPECT_EQ(atomData2.reverse_domain_name, kTestStringValue);
    EXPECT_EQ(atomData2.float_field, -1.1f);
    EXPECT_EQ(atomData2.int_field, 15);
    EXPECT_EQ(atomData2.long_field, -67);
    EXPECT_EQ(atomData2.bool_field, true);
    EXPECT_EQ(atomData.enum_field, VendorAtom4::EnumType4::TYPE_1);

    VendorAtomWithState3 atomData3;
    EXPECT_EQ(atomData3.state, VendorAtomWithState3::TestState::TEST_STATE_1);
}

}  // namespace api_gen_vendor_tests
}  // namespace android
