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

#include <google/protobuf/descriptor.h>
#include <gtest/gtest.h>

#include <vector>

#include "frameworks/proto_logging/stats/atoms.pb.h"

namespace android {
namespace generic_vendor_protos {

using android::os::statsd::Atom;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;

bool isGenericVendorAtom(int tag) {
    return (tag >= 300000 && tag <= 399999);
}

TEST(GenericVendorAtomsTests, ReverseDomainNameTest) {
    auto atom = Atom::descriptor();

    // Extension field atoms in Atom.
    std::vector<const FieldDescriptor*> extensions;
    atom->file()->pool()->FindAllExtensions(atom, &extensions);
    for (const FieldDescriptor* atomField : extensions) {
        // test atom id is within generic vendor atoms range
        if (!isGenericVendorAtom(atomField->number())) {
            continue;
        }

        // if yes test first field is a string reverse_domain_name
        const Descriptor* atom = atomField->message_type();
        ASSERT_EQ(atom->field(0)->number(), 1)
                << "Atom " << atom->name() << "has wrong tag for first field";
        ASSERT_EQ(atom->field(0)->type(), FieldDescriptor::TYPE_STRING)
                << "Atom " << atom->name() << "has wrong type for first field";
        ASSERT_EQ(atom->field(0)->name(), "reverse_domain_name")
                << "Atom " << atom->name() << "has wrong name for first field";
    }
}

}  // namespace generic_vendor_protos
}  // namespace android
