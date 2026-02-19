// Copyright (C) 2025 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

use android_frameworks_stats::aidl::android::frameworks::stats::AnnotationId::AnnotationId;
use android_frameworks_stats::aidl::android::frameworks::stats::AnnotationValue::AnnotationValue;
use android_frameworks_stats::aidl::android::frameworks::stats::VendorAtomValue::VendorAtomValue;
use matches::assert_matches;

const TEST_INT_VALUE: i32 = 100;
const TEST_UID_VALUE: i32 = 1000;
const TEST_PID_VALUE: i32 = 3000;
const TEST_LONG_VALUE: i64 = i64::MAX - (TEST_INT_VALUE as i64);
const TEST_FLOAT_VALUE: f32 = TEST_INT_VALUE as f32 / TEST_LONG_VALUE as f32;
const TEST_BOOL_VALUE: bool = true;
const TEST_STRING_VALUE: &str = "test_string";
const TEST_STRING_VALUE2: &str = "test_string2";

trait VendorAtomValueExt {
    fn unwrap_int_value(&self) -> i32;
    fn unwrap_long_value(&self) -> i64;
    fn unwrap_float_value(&self) -> f32;
    fn unwrap_bool_value(&self) -> bool;
    fn unwrap_byte_array_value(&self) -> &[u8];
    fn unwrap_repeated_bool_value(&self) -> &[bool];
    fn unwrap_repeated_int_value(&self) -> &[i32];
    fn unwrap_repeated_long_value(&self) -> &[i64];
    fn unwrap_repeated_float_value(&self) -> &[f32];
    fn unwrap_repeated_string_value(&self) -> &[Option<String>];
}

impl VendorAtomValueExt for VendorAtomValue {
    fn unwrap_int_value(&self) -> i32 {
        match self {
            VendorAtomValue::IntValue(x) => *x,
            _ => panic!("not an IntValue"),
        }
    }
    fn unwrap_long_value(&self) -> i64 {
        match self {
            VendorAtomValue::LongValue(x) => *x,
            _ => panic!("not a LongValue"),
        }
    }
    fn unwrap_float_value(&self) -> f32 {
        match self {
            VendorAtomValue::FloatValue(x) => *x,
            _ => panic!("not a FloatValue"),
        }
    }
    fn unwrap_bool_value(&self) -> bool {
        match self {
            VendorAtomValue::BoolValue(x) => *x,
            _ => panic!("not a BoolValue"),
        }
    }
    fn unwrap_byte_array_value(&self) -> &[u8] {
        match self {
            VendorAtomValue::ByteArrayValue(Some(x)) => x,
            VendorAtomValue::ByteArrayValue(None) => panic!("ByteArrayValue is None"),
            _ => panic!("not a ByteArrayValue"),
        }
    }
    fn unwrap_repeated_bool_value(&self) -> &[bool] {
        match self {
            VendorAtomValue::RepeatedBoolValue(Some(x)) => x,
            VendorAtomValue::RepeatedBoolValue(None) => panic!("RepeatedBoolValue is None"),
            _ => panic!("not a RepeatedBoolValue"),
        }
    }
    fn unwrap_repeated_int_value(&self) -> &[i32] {
        match self {
            VendorAtomValue::RepeatedIntValue(Some(x)) => x,
            VendorAtomValue::RepeatedIntValue(None) => panic!("RepeatedIntValue is None"),
            _ => panic!("not a RepeatedIntValue"),
        }
    }
    fn unwrap_repeated_long_value(&self) -> &[i64] {
        match self {
            VendorAtomValue::RepeatedLongValue(Some(x)) => x,
            VendorAtomValue::RepeatedLongValue(None) => panic!("RepeatedLongValue is None"),
            _ => panic!("not a RepeatedLongValue"),
        }
    }
    fn unwrap_repeated_float_value(&self) -> &[f32] {
        match self {
            VendorAtomValue::RepeatedFloatValue(Some(x)) => x,
            VendorAtomValue::RepeatedFloatValue(None) => panic!("RepeatedFloatValue is None"),
            _ => panic!("not a RepeatedFloatValue"),
        }
    }
    fn unwrap_repeated_string_value(&self) -> &[Option<String>] {
        match self {
            VendorAtomValue::RepeatedStringValue(Some(x)) => x,
            VendorAtomValue::RepeatedStringValue(None) => panic!("RepeatedStringValue is None"),
            _ => panic!("not a RepeatedStringValue"),
        }
    }
}

/// Tests native auto generated code for specific vendor atom contains proper ids
#[test]
fn atom_id_constants_test() {
    assert_eq!(test_vendor_atoms::vendor_atom1::VendorAtom1::CODE, 105501);
    assert_eq!(test_vendor_atoms::vendor_atom2::VendorAtom2::CODE, 105502);
    assert_eq!(test_vendor_atoms::vendor_atom4::VendorAtom4::CODE, 105504);
}

/// Tests native auto generated code for specific vendor atom contains proper enums
#[test]
fn atom_enum_test() {
    assert_eq!(test_vendor_atoms::vendor_atom1::EnumField1::TypeUnknown as i32, 0);
    assert_eq!(test_vendor_atoms::vendor_atom1::EnumField1::Type1 as i32, 1);
    assert_eq!(test_vendor_atoms::vendor_atom1::EnumField1::Type2 as i32, 2);
    assert_eq!(test_vendor_atoms::vendor_atom1::EnumField1::Type3 as i32, 3);

    assert_eq!(test_vendor_atoms::vendor_atom1::EnumField3::AnotherTypeUnknown as i32, 0);
    assert_eq!(test_vendor_atoms::vendor_atom1::EnumField3::AnotherType1 as i32, 1);
    assert_eq!(test_vendor_atoms::vendor_atom1::EnumField3::AnotherType2 as i32, 2);
    assert_eq!(test_vendor_atoms::vendor_atom1::EnumField3::AnotherType3 as i32, 3);

    assert_eq!(test_vendor_atoms::vendor_atom2::EnumField1::TypeUnknown as i32, 0);
    assert_eq!(test_vendor_atoms::vendor_atom2::EnumField1::Type1 as i32, 1);
    assert_eq!(test_vendor_atoms::vendor_atom2::EnumField1::Type2 as i32, 2);
    assert_eq!(test_vendor_atoms::vendor_atom2::EnumField1::Type3 as i32, 3);

    assert_eq!(test_vendor_atoms::vendor_atom2::EnumField3::AnotherTypeUnknown as i32, 0);
    assert_eq!(test_vendor_atoms::vendor_atom2::EnumField3::AnotherType1 as i32, 1);
    assert_eq!(test_vendor_atoms::vendor_atom2::EnumField3::AnotherType2 as i32, 2);
    assert_eq!(test_vendor_atoms::vendor_atom2::EnumField3::AnotherType3 as i32, 3);

    assert_eq!(test_vendor_atoms::vendor_atom4::EnumField::TypeUnknown as i32, 0);
    assert_eq!(test_vendor_atoms::vendor_atom4::EnumField::Type1 as i32, 1);

    assert_eq!(test_vendor_atoms::vendor_atom4::EnumRepeated::Type5Unknown as i32, 0);
    assert_eq!(test_vendor_atoms::vendor_atom4::EnumRepeated::Type51 as i32, 1);
    assert_eq!(test_vendor_atoms::vendor_atom4::EnumRepeated::Type52 as i32, 2);
}

#[test]
fn build_vendor_atom1_api_test() {
    use test_vendor_atoms::vendor_atom1;

    let atom = vendor_atom1::VendorAtom1 {
        reverse_domain_name: TEST_STRING_VALUE,
        enum_field1: vendor_atom1::EnumField1::Type1,
        enum_field2: vendor_atom1::EnumField2::Type2,
        int_value32: TEST_INT_VALUE,
        int_value64: TEST_LONG_VALUE,
        float_value: TEST_FLOAT_VALUE,
        bool_value: TEST_BOOL_VALUE,
        enum_field3: vendor_atom1::EnumField3::AnotherType2,
        enum_field4: vendor_atom1::EnumField4::AnotherType3,
    }
    .to_vendor_atom();

    assert_eq!(atom.atomId, vendor_atom1::VendorAtom1::CODE);
    assert_eq!(atom.reverseDomainName, TEST_STRING_VALUE);
    assert_eq!(atom.values.len(), 8);
    assert_eq!(atom.values[0].unwrap_int_value(), vendor_atom1::EnumField1::Type1 as _);
    assert_eq!(atom.values[1].unwrap_int_value(), vendor_atom1::EnumField1::Type2 as _);
    assert_eq!(atom.values[2].unwrap_int_value(), TEST_INT_VALUE);
    assert_eq!(atom.values[3].unwrap_long_value(), TEST_LONG_VALUE);
    assert_eq!(atom.values[4].unwrap_float_value(), TEST_FLOAT_VALUE);
    assert_eq!(atom.values[5].unwrap_bool_value(), TEST_BOOL_VALUE);
    assert_eq!(atom.values[6].unwrap_int_value(), vendor_atom1::EnumField3::AnotherType2 as _);
    assert_eq!(atom.values[7].unwrap_int_value(), vendor_atom1::EnumField4::AnotherType3 as _);
    assert!(atom.atomAnnotations.is_none());
}

#[test]
fn build_vendor_atom3_api_test() {
    use test_vendor_atoms::vendor_atom3;

    let atom = vendor_atom3::VendorAtom3 {
        reverse_domain_name: TEST_STRING_VALUE,
        int_field: TEST_INT_VALUE,
    }
    .to_vendor_atom();

    assert_eq!(atom.atomId, vendor_atom3::VendorAtom3::CODE);
    assert_eq!(atom.reverseDomainName, TEST_STRING_VALUE);
    assert_eq!(atom.values.len(), 1);
    assert_eq!(atom.values[0].unwrap_int_value(), TEST_INT_VALUE);
    assert!(atom.atomAnnotations.is_none());
}

#[test]
fn build_vendor_atom4_api_test() {
    use test_vendor_atoms::vendor_atom4;

    const TEST_REPEATED_BOOL_VALUE: &[bool] = &[true, false, true];
    const TEST_REPEATED_FLOAT_VALUE: &[f32] = &[1.0, 2.0, 3.0];
    const TEST_REPEATED_INT_VALUE: &[i32] = &[10, 20, 30];
    const TEST_REPEATED_LONG_VALUE: &[i64] = &[100, 200, 300];
    const TEST_REPEATED_STRING_VALUE: &[&str] = &["foo", "bar", "baz"];
    const TEST_REPEATED_ENUM_VALUE: &[vendor_atom4::EnumRepeated] =
        &[vendor_atom4::EnumRepeated::Type51, vendor_atom4::EnumRepeated::Type52];

    let atom = vendor_atom4::VendorAtom4 {
        reverse_domain_name: TEST_STRING_VALUE,
        float_field: TEST_FLOAT_VALUE,
        int_field: TEST_INT_VALUE,
        long_field: TEST_LONG_VALUE,
        bool_field: TEST_BOOL_VALUE,
        enum_field: vendor_atom4::EnumField::Type1,
        bool_repeated: TEST_REPEATED_BOOL_VALUE,
        float_repeated: TEST_REPEATED_FLOAT_VALUE,
        int_repeated: TEST_REPEATED_INT_VALUE,
        long_repeated: TEST_REPEATED_LONG_VALUE,
        string_repeated: TEST_REPEATED_STRING_VALUE,
        enum_repeated: TEST_REPEATED_ENUM_VALUE,
    }
    .to_vendor_atom();

    assert_eq!(atom.atomId, vendor_atom4::VendorAtom4::CODE);
    assert_eq!(atom.reverseDomainName, TEST_STRING_VALUE);
    assert_eq!(atom.values.len(), 11);
    assert_eq!(atom.values[0].unwrap_float_value(), TEST_FLOAT_VALUE);
    assert_eq!(atom.values[1].unwrap_int_value(), TEST_INT_VALUE);
    assert_eq!(atom.values[2].unwrap_long_value(), TEST_LONG_VALUE);
    assert_eq!(atom.values[3].unwrap_bool_value(), TEST_BOOL_VALUE);
    assert_eq!(atom.values[4].unwrap_int_value(), vendor_atom4::EnumField::Type1 as _);
    assert_eq!(atom.values[5].unwrap_repeated_bool_value(), TEST_REPEATED_BOOL_VALUE);
    assert_eq!(atom.values[6].unwrap_repeated_float_value(), TEST_REPEATED_FLOAT_VALUE);
    assert_eq!(atom.values[7].unwrap_repeated_int_value(), TEST_REPEATED_INT_VALUE);
    assert_eq!(atom.values[8].unwrap_repeated_long_value(), TEST_REPEATED_LONG_VALUE);

    let expected_strings: Vec<Option<String>> =
        TEST_REPEATED_STRING_VALUE.iter().map(|&s| Some(s.to_string())).collect();
    assert_eq!(atom.values[9].unwrap_repeated_string_value(), expected_strings.as_slice());

    let expected_enums: Vec<i32> = TEST_REPEATED_ENUM_VALUE.iter().map(|&e| e as i32).collect();
    assert_eq!(atom.values[10].unwrap_repeated_int_value(), expected_enums.as_slice());

    assert!(atom.valuesAnnotations.is_none());
    assert!(atom.atomAnnotations.is_none());
}

#[test]
fn build_vendor_atom5_api_test() {
    use test_vendor_atoms::vendor_atom5;

    // The C++ buildVendorAtom5ApiTest test uses a serialized TestNestedMessage.
    // Here we use raw bytes without protobuf.
    let nested_data = TEST_STRING_VALUE2.as_bytes();
    let atom = vendor_atom5::VendorAtom5 {
        reverse_domain_name: TEST_STRING_VALUE,
        float_field: TEST_FLOAT_VALUE,
        int_field: TEST_INT_VALUE,
        long_field: TEST_LONG_VALUE,
        nested_message_field: nested_data,
    }
    .to_vendor_atom();

    assert_eq!(atom.atomId, vendor_atom5::VendorAtom5::CODE);
    assert_eq!(atom.reverseDomainName, TEST_STRING_VALUE);
    assert_eq!(atom.values.len(), 4);
    assert_eq!(atom.values[0].unwrap_float_value(), TEST_FLOAT_VALUE);
    assert_eq!(atom.values[1].unwrap_int_value(), TEST_INT_VALUE);
    assert_eq!(atom.values[2].unwrap_long_value(), TEST_LONG_VALUE);
    assert_eq!(atom.values[3].unwrap_byte_array_value(), nested_data);
    assert!(atom.valuesAnnotations.is_none()); // C++ checks atomAnnotations, Rust generated code puts annotations in valuesAnnotations
    assert!(atom.atomAnnotations.is_none());
}

#[test]
fn build_atom_with_truncate_timestamp_test() {
    use test_vendor_atoms::{truncate_timestamp_atom1, truncate_timestamp_atom2};

    let atom1 = truncate_timestamp_atom1::TruncateTimestampAtom1 {
        reverse_domain_name: TEST_STRING_VALUE,
        state: truncate_timestamp_atom1::State::TestState1,
    }
    .to_vendor_atom();
    assert_eq!(atom1.atomId, truncate_timestamp_atom1::TruncateTimestampAtom1::CODE);
    assert_eq!(atom1.reverseDomainName, TEST_STRING_VALUE);
    assert_eq!(atom1.values.len(), 1);
    assert_eq!(
        atom1.values[0].unwrap_int_value(),
        truncate_timestamp_atom1::State::TestState1 as _
    );
    assert!(atom1.atomAnnotations.is_some());
    assert_eq!(atom1.atomAnnotations.as_ref().unwrap().len(), 1);
    assert!(atom1.atomAnnotations.as_ref().unwrap()[0].is_some());
    assert_eq!(
        atom1.atomAnnotations.as_ref().unwrap()[0].as_ref().unwrap().annotationId,
        AnnotationId::TRUNCATE_TIMESTAMP
    );
    assert_matches!(
        atom1.atomAnnotations.as_ref().unwrap()[0].as_ref().unwrap().value,
        AnnotationValue::BoolValue(true)
    );

    let atom2 = truncate_timestamp_atom2::TruncateTimestampAtom2 {
        reverse_domain_name: TEST_STRING_VALUE,
        int_value: TEST_INT_VALUE,
    }
    .to_vendor_atom();
    assert_eq!(atom2.atomId, truncate_timestamp_atom2::TruncateTimestampAtom2::CODE);
    assert_eq!(atom2.reverseDomainName, TEST_STRING_VALUE);
    assert_eq!(atom2.values.len(), 1);
    assert_eq!(atom2.values[0].unwrap_int_value(), TEST_INT_VALUE);
    assert!(atom2.atomAnnotations.is_some());
    assert_eq!(atom2.atomAnnotations.as_ref().unwrap().len(), 1);
    assert!(atom2.atomAnnotations.as_ref().unwrap()[0].is_some());
    assert_eq!(
        atom2.atomAnnotations.as_ref().unwrap()[0].as_ref().unwrap().annotationId,
        AnnotationId::TRUNCATE_TIMESTAMP
    );
    assert_matches!(
        atom2.atomAnnotations.as_ref().unwrap()[0].as_ref().unwrap().value,
        AnnotationValue::BoolValue(true)
    );
}

#[test]
fn build_atom_with_exclusive_state_annotation_test() {
    use test_vendor_atoms::state_atom3;

    let atom = state_atom3::StateAtom3 {
        reverse_domain_name: TEST_STRING_VALUE,
        state: state_atom3::State::TestState3,
    }
    .to_vendor_atom();

    assert_eq!(atom.atomId, state_atom3::StateAtom3::CODE);
    assert_eq!(atom.reverseDomainName, TEST_STRING_VALUE);
    assert_eq!(atom.values.len(), 1);
    assert_eq!(atom.values[0].unwrap_int_value(), state_atom3::State::TestState3 as _);
    assert!(atom.valuesAnnotations.is_some());
    assert_eq!(atom.valuesAnnotations.as_ref().unwrap().len(), 1);
    assert!(atom.valuesAnnotations.as_ref().unwrap()[0].is_some());
    assert_eq!(atom.valuesAnnotations.as_ref().unwrap()[0].as_ref().unwrap().valueIndex, 0);
    assert_eq!(atom.valuesAnnotations.as_ref().unwrap()[0].as_ref().unwrap().annotations.len(), 1);
    assert_eq!(
        atom.valuesAnnotations.as_ref().unwrap()[0].as_ref().unwrap().annotations[0].annotationId,
        AnnotationId::EXCLUSIVE_STATE
    );
    assert_matches!(
        atom.valuesAnnotations.as_ref().unwrap()[0].as_ref().unwrap().annotations[0].value,
        AnnotationValue::BoolValue(true)
    );
    assert!(atom.atomAnnotations.is_none());
}

#[test]
fn build_atom_with_exclusive_state_and_primary_field_annotation_test() {
    use test_vendor_atoms::state_atom1;

    let atom = state_atom1::StateAtom1 {
        reverse_domain_name: TEST_STRING_VALUE,
        uid: TEST_UID_VALUE,
        state: state_atom1::State::TestState3,
    }
    .to_vendor_atom();

    assert_eq!(atom.atomId, state_atom1::StateAtom1::CODE);
    assert_eq!(atom.reverseDomainName, TEST_STRING_VALUE);
    assert_eq!(atom.values.len(), 2);
    assert_eq!(atom.values[0].unwrap_int_value(), TEST_UID_VALUE);
    assert_eq!(atom.values[1].unwrap_int_value(), state_atom1::State::TestState3 as _);
    assert!(atom.valuesAnnotations.is_some());
    assert_eq!(atom.valuesAnnotations.as_ref().unwrap().len(), 2);
    assert!(atom.valuesAnnotations.as_ref().unwrap()[0].is_some());
    assert_eq!(atom.valuesAnnotations.as_ref().unwrap()[0].as_ref().unwrap().valueIndex, 0);
    assert_eq!(atom.valuesAnnotations.as_ref().unwrap()[0].as_ref().unwrap().annotations.len(), 1);
    assert_eq!(
        atom.valuesAnnotations.as_ref().unwrap()[0].as_ref().unwrap().annotations[0].annotationId,
        AnnotationId::PRIMARY_FIELD
    );
    assert_matches!(
        atom.valuesAnnotations.as_ref().unwrap()[0].as_ref().unwrap().annotations[0].value,
        AnnotationValue::BoolValue(true)
    );
    assert!(atom.valuesAnnotations.as_ref().unwrap()[1].is_some());
    assert_eq!(atom.valuesAnnotations.as_ref().unwrap()[1].as_ref().unwrap().valueIndex, 1);
    assert_eq!(atom.valuesAnnotations.as_ref().unwrap()[1].as_ref().unwrap().annotations.len(), 1);
    assert_eq!(
        atom.valuesAnnotations.as_ref().unwrap()[1].as_ref().unwrap().annotations[0].annotationId,
        AnnotationId::EXCLUSIVE_STATE
    );
    assert_matches!(
        atom.valuesAnnotations.as_ref().unwrap()[1].as_ref().unwrap().annotations[0].value,
        AnnotationValue::BoolValue(true)
    );
    assert!(atom.atomAnnotations.is_none());
}

#[test]
fn build_atom_with_exclusive_state_and_two_primary_field_annotation_test() {
    use test_vendor_atoms::state_atom2;

    let atom = state_atom2::StateAtom2 {
        reverse_domain_name: TEST_STRING_VALUE,
        uid: TEST_UID_VALUE,
        pid: TEST_PID_VALUE,
        state: state_atom2::State::TestState2,
    }
    .to_vendor_atom();

    assert_eq!(atom.atomId, state_atom2::StateAtom2::CODE);
    assert_eq!(atom.reverseDomainName, TEST_STRING_VALUE);
    assert_eq!(atom.values.len(), 3);
    assert_eq!(atom.values[0].unwrap_int_value(), TEST_UID_VALUE);
    assert_eq!(atom.values[1].unwrap_int_value(), TEST_PID_VALUE);
    assert_eq!(atom.values[2].unwrap_int_value(), state_atom2::State::TestState2 as _);

    assert!(atom.valuesAnnotations.is_some());
    let annotations = atom.valuesAnnotations.as_ref().unwrap();
    assert_eq!(annotations.len(), 3);

    assert!(annotations[0].is_some());
    let annotation_set0 = annotations[0].as_ref().unwrap();
    assert_eq!(annotation_set0.valueIndex, 0);
    assert_eq!(annotation_set0.annotations.len(), 1);
    assert_eq!(annotation_set0.annotations[0].annotationId, AnnotationId::PRIMARY_FIELD);
    assert_matches!(annotation_set0.annotations[0].value, AnnotationValue::BoolValue(true));

    assert!(annotations[1].is_some());
    let annotation_set1 = annotations[1].as_ref().unwrap();
    assert_eq!(annotation_set1.valueIndex, 1);
    assert_eq!(annotation_set1.annotations.len(), 1);
    assert_eq!(annotation_set1.annotations[0].annotationId, AnnotationId::PRIMARY_FIELD);
    assert_matches!(annotation_set1.annotations[0].value, AnnotationValue::BoolValue(true));

    assert!(annotations[2].is_some());
    let annotation_set2 = annotations[2].as_ref().unwrap();
    assert_eq!(annotation_set2.valueIndex, 2);
    assert_eq!(annotation_set2.annotations.len(), 1);
    assert_eq!(annotation_set2.annotations[0].annotationId, AnnotationId::EXCLUSIVE_STATE);
    assert_matches!(annotation_set2.annotations[0].value, AnnotationValue::BoolValue(true));

    assert!(atom.atomAnnotations.is_none());
}

#[test]
fn build_atom_with_multiple_annotations_per_value_test() {
    use test_vendor_atoms::state_atom4;

    let atom = state_atom4::StateAtom4 {
        reverse_domain_name: TEST_STRING_VALUE,
        state: state_atom4::State::On,
        some_flag: TEST_BOOL_VALUE,
    }
    .to_vendor_atom();

    assert_eq!(atom.atomId, state_atom4::StateAtom4::CODE);
    assert_eq!(atom.reverseDomainName, TEST_STRING_VALUE);
    assert_eq!(atom.values.len(), 2);
    assert_eq!(atom.values[0].unwrap_int_value(), state_atom4::State::On as _);
    assert_eq!(atom.values[1].unwrap_bool_value(), TEST_BOOL_VALUE);

    assert!(atom.valuesAnnotations.is_some());
    let annotations = atom.valuesAnnotations.as_ref().unwrap();
    assert_eq!(annotations.len(), 2);

    assert!(annotations[0].is_some());
    let annotation_set0 = annotations[0].as_ref().unwrap();
    assert_eq!(annotation_set0.valueIndex, 0);
    assert_eq!(annotation_set0.annotations.len(), 2);
    assert_eq!(annotation_set0.annotations[0].annotationId, AnnotationId::EXCLUSIVE_STATE);
    assert_matches!(annotation_set0.annotations[0].value, AnnotationValue::BoolValue(true));
    assert_eq!(annotation_set0.annotations[1].annotationId, AnnotationId::STATE_NESTED);
    assert_matches!(annotation_set0.annotations[1].value, AnnotationValue::BoolValue(true));

    assert!(annotations[1].is_some());
    let annotation_set1 = annotations[1].as_ref().unwrap();
    assert_eq!(annotation_set1.valueIndex, 1);
    assert_eq!(annotation_set1.annotations.len(), 1);
    assert_eq!(annotation_set1.annotations[0].annotationId, AnnotationId::PRIMARY_FIELD);
    assert_matches!(annotation_set1.annotations[0].value, AnnotationValue::BoolValue(true));

    assert!(atom.atomAnnotations.is_none());
}

#[test]
fn build_atom_with_trigger_reset_annotation_test() {
    use test_vendor_atoms::state_atom4;

    let atom = state_atom4::StateAtom4 {
        reverse_domain_name: TEST_STRING_VALUE,
        state: state_atom4::State::Reset,
        some_flag: TEST_BOOL_VALUE,
    }
    .to_vendor_atom();

    const DEFAULT_STATE_VALUE: i32 = state_atom4::State::Off as i32;

    assert_eq!(atom.atomId, state_atom4::StateAtom4::CODE);
    assert_eq!(atom.reverseDomainName, TEST_STRING_VALUE);
    assert_eq!(atom.values.len(), 2);
    assert_eq!(atom.values[0].unwrap_int_value(), state_atom4::State::Reset as _);
    assert_eq!(atom.values[1].unwrap_bool_value(), TEST_BOOL_VALUE);

    assert!(atom.valuesAnnotations.is_some());
    let annotations = atom.valuesAnnotations.as_ref().unwrap();
    assert_eq!(annotations.len(), 2);

    assert!(annotations[0].is_some());
    let annotation_set0 = annotations[0].as_ref().unwrap();
    assert_eq!(annotation_set0.valueIndex, 0);
    assert_eq!(annotation_set0.annotations.len(), 3);
    assert_eq!(annotation_set0.annotations[0].annotationId, AnnotationId::EXCLUSIVE_STATE);
    assert_matches!(annotation_set0.annotations[0].value, AnnotationValue::BoolValue(true));
    assert_eq!(annotation_set0.annotations[1].annotationId, AnnotationId::STATE_NESTED);
    assert_matches!(annotation_set0.annotations[1].value, AnnotationValue::BoolValue(true));
    assert_eq!(annotation_set0.annotations[2].annotationId, AnnotationId::TRIGGER_STATE_RESET);
    assert_matches!(
        annotation_set0.annotations[2].value,
        AnnotationValue::IntValue(DEFAULT_STATE_VALUE)
    );

    assert!(annotations[1].is_some());
    let annotation_set1 = annotations[1].as_ref().unwrap();
    assert_eq!(annotation_set1.valueIndex, 1);
    assert_eq!(annotation_set1.annotations.len(), 1);
    assert_eq!(annotation_set1.annotations[0].annotationId, AnnotationId::PRIMARY_FIELD);
    assert_matches!(annotation_set1.annotations[0].value, AnnotationValue::BoolValue(true));

    assert!(atom.atomAnnotations.is_none());
}
