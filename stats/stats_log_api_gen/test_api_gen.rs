/*
 * Copyright (C) 2026 The Android Open Source Project
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

use statslog_rust::{ble_scan_state_changed, test_atom_reported};
use statslog_rust_header::Atoms;

#[test]
fn atom_id_constants_test() {
    assert_eq!(Atoms::BleScanStateChanged as i32, 2);
    assert_eq!(Atoms::ProcessStateChanged as i32, 3);
    assert_eq!(Atoms::BootSequenceReported as i32, 57);
}

#[test]
fn atom_enum_test() {
    assert_eq!(ble_scan_state_changed::State::Off as i32, 0);
    assert_eq!(ble_scan_state_changed::State::On as i32, 1);
    assert_eq!(ble_scan_state_changed::State::Reset as i32, 2);
}

#[test]
fn test_atom_reported_api_test() {
    assert_eq!(Atoms::TestAtomReported as i32, 205);

    assert_eq!(test_atom_reported::State::Unknown as i32, 0);
    assert_eq!(test_atom_reported::State::Off as i32, 1);
    assert_eq!(test_atom_reported::State::On as i32, 2);

    assert_eq!(test_atom_reported::RepeatedEnumField::Unknown as i32, 0);
    assert_eq!(test_atom_reported::RepeatedEnumField::Off as i32, 1);
    assert_eq!(test_atom_reported::RepeatedEnumField::On as i32, 2);

    let result = test_atom_reported::stats_write(
        &[],                           // attribution_chain
        &[],                           // package_names
        42,                            // int_field
        1234567890,                    // long_field
        12.14,                         // float_field
        "test",                        // string_field
        true,                          // boolean_field
        test_atom_reported::State::On, // state
        &[1, 2, 3],                    // bytes_field
        &[10, 20],                     // repeated_int_field
        &[100, 200],                   // repeated_long_field
        &[1.0, 2.0],                   // repeated_float_field
        &["foo", "bar"],               // repeated_string_field
        &[true, false],                // repeated_bool_field
        &[test_atom_reported::RepeatedEnumField::Off, test_atom_reported::RepeatedEnumField::On], // repeated_enum_field
    );
    assert!(result.is_ok());
}
