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
#include <google/protobuf/descriptor.h>

#include "frameworks/proto_logging/stats/atoms.pb.h"

#include <vector>

namespace android {
namespace platform_protos {

using android::os::statsd::Atom;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;

/**
 * Tests number of fields per atom
 */

void validateMessageFieldsCount(const FieldDescriptor& field) {
    const Descriptor* atom = field.message_type();
    ASSERT_LE(atom->field_count(), 127) << "Atom " << field.name() << "has too many fields";
}

TEST(PlatformAtomsTests, AtomMaxFieldNumberTest) {

    auto atom = Atom::descriptor();
    // Regular field atoms in Atom
    for (int i = 0; i < atom->field_count(); i++) {
        const FieldDescriptor* atomField = Atom::descriptor()->field(i);
        validateMessageFieldsCount(*atomField);
    }

    // Extension field atoms in Atom.
    std::vector<const FieldDescriptor*> extensions;
    atom->file()->pool()->FindAllExtensions(atom, &extensions);
    for (const FieldDescriptor* atomField : extensions) {
        validateMessageFieldsCount(*atomField);
    }
}

} // namespace platform_protos
} // namespace android
