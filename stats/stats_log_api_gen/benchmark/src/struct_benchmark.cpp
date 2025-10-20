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

#include <benchmark/benchmark.h>
#include <stats_event.h>

#include <cstdint>
#include <string>

namespace {

struct AtomLarge {
    std::string string_field_1;
    std::string string_field_2;
    std::string string_field_3;
    std::string string_field_4;
    std::string string_field_5;
    std::string string_field_6;
    std::string string_field_7;
    std::string string_field_8;
    std::string string_field_9;
    std::string string_field_10;
    int32_t int32_field_1;
    int32_t int32_field_2;
    int32_t int32_field_3;
    int32_t int32_field_4;
    int32_t int32_field_5;
    int32_t int32_field_6;
    int32_t int32_field_7;
    int32_t int32_field_8;
    int32_t int32_field_9;
    int32_t int32_field_10;
    int32_t int32_field_11;
    int32_t int32_field_12;
    int32_t int32_field_13;
    int32_t int32_field_14;
    int32_t int32_field_15;
    int32_t int32_field_16;
    int32_t int32_field_17;
    int32_t int32_field_18;
    int32_t int32_field_19;
    int32_t int32_field_20;
    int64_t int64_field_1;
    int64_t int64_field_2;
    int64_t int64_field_3;
    int64_t int64_field_4;
    int64_t int64_field_5;
    int64_t int64_field_6;
    int64_t int64_field_7;
    int64_t int64_field_8;
    int64_t int64_field_9;
    int64_t int64_field_10;
    int64_t int64_field_11;
    int64_t int64_field_12;
    int64_t int64_field_13;
    int64_t int64_field_14;
    int64_t int64_field_15;
    int64_t int64_field_16;
    int64_t int64_field_17;
    int64_t int64_field_18;
    int64_t int64_field_19;
    int64_t int64_field_20;
    bool bool_field_1;
    bool bool_field_2;
    bool bool_field_3;
    bool bool_field_4;
    bool bool_field_5;
    bool bool_field_6;
    bool bool_field_7;
    bool bool_field_8;
    bool bool_field_9;
    bool bool_field_10;
    float float_field_1;
    float float_field_2;
    float float_field_3;
    float float_field_4;
    float float_field_5;
    float float_field_6;
    float float_field_7;
    float float_field_8;
    float float_field_9;
    float float_field_10;
};

class AtomLargeBuilder {
public:
    AtomLargeBuilder& setStringField1(const std::string& value) {
        atom.string_field_1 = value;
        return *this;
    }
    AtomLargeBuilder& setStringField2(const std::string& value) {
        atom.string_field_2 = value;
        return *this;
    }
    AtomLargeBuilder& setStringField3(const std::string& value) {
        atom.string_field_3 = value;
        return *this;
    }
    AtomLargeBuilder& setStringField4(const std::string& value) {
        atom.string_field_4 = value;
        return *this;
    }
    AtomLargeBuilder& setStringField5(const std::string& value) {
        atom.string_field_5 = value;
        return *this;
    }
    AtomLargeBuilder& setStringField6(const std::string& value) {
        atom.string_field_6 = value;
        return *this;
    }
    AtomLargeBuilder& setStringField7(const std::string& value) {
        atom.string_field_7 = value;
        return *this;
    }
    AtomLargeBuilder& setStringField8(const std::string& value) {
        atom.string_field_8 = value;
        return *this;
    }
    AtomLargeBuilder& setStringField9(const std::string& value) {
        atom.string_field_9 = value;
        return *this;
    }
    AtomLargeBuilder& setStringField10(const std::string& value) {
        atom.string_field_10 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field1(int32_t value) {
        atom.int32_field_1 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field2(int32_t value) {
        atom.int32_field_2 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field3(int32_t value) {
        atom.int32_field_3 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field4(int32_t value) {
        atom.int32_field_4 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field5(int32_t value) {
        atom.int32_field_5 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field6(int32_t value) {
        atom.int32_field_6 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field7(int32_t value) {
        atom.int32_field_7 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field8(int32_t value) {
        atom.int32_field_8 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field9(int32_t value) {
        atom.int32_field_9 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field10(int32_t value) {
        atom.int32_field_10 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field11(int32_t value) {
        atom.int32_field_11 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field12(int32_t value) {
        atom.int32_field_12 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field13(int32_t value) {
        atom.int32_field_13 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field14(int32_t value) {
        atom.int32_field_14 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field15(int32_t value) {
        atom.int32_field_15 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field16(int32_t value) {
        atom.int32_field_16 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field17(int32_t value) {
        atom.int32_field_17 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field18(int32_t value) {
        atom.int32_field_18 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field19(int32_t value) {
        atom.int32_field_19 = value;
        return *this;
    }
    AtomLargeBuilder& setInt32Field20(int32_t value) {
        atom.int32_field_20 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field1(int64_t value) {
        atom.int64_field_1 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field2(int64_t value) {
        atom.int64_field_2 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field3(int64_t value) {
        atom.int64_field_3 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field4(int64_t value) {
        atom.int64_field_4 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field5(int64_t value) {
        atom.int64_field_5 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field6(int64_t value) {
        atom.int64_field_6 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field7(int64_t value) {
        atom.int64_field_7 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field8(int64_t value) {
        atom.int64_field_8 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field9(int64_t value) {
        atom.int64_field_9 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field10(int64_t value) {
        atom.int64_field_10 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field11(int64_t value) {
        atom.int64_field_11 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field12(int64_t value) {
        atom.int64_field_12 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field13(int64_t value) {
        atom.int64_field_13 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field14(int64_t value) {
        atom.int64_field_14 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field15(int64_t value) {
        atom.int64_field_15 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field16(int64_t value) {
        atom.int64_field_16 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field17(int64_t value) {
        atom.int64_field_17 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field18(int64_t value) {
        atom.int64_field_18 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field19(int64_t value) {
        atom.int64_field_19 = value;
        return *this;
    }
    AtomLargeBuilder& setInt64Field20(int64_t value) {
        atom.int64_field_20 = value;
        return *this;
    }
    AtomLargeBuilder& setBoolField1(bool value) {
        atom.bool_field_1 = value;
        return *this;
    }
    AtomLargeBuilder& setBoolField2(bool value) {
        atom.bool_field_2 = value;
        return *this;
    }
    AtomLargeBuilder& setBoolField3(bool value) {
        atom.bool_field_3 = value;
        return *this;
    }
    AtomLargeBuilder& setBoolField4(bool value) {
        atom.bool_field_4 = value;
        return *this;
    }
    AtomLargeBuilder& setBoolField5(bool value) {
        atom.bool_field_5 = value;
        return *this;
    }
    AtomLargeBuilder& setBoolField6(bool value) {
        atom.bool_field_6 = value;
        return *this;
    }
    AtomLargeBuilder& setBoolField7(bool value) {
        atom.bool_field_7 = value;
        return *this;
    }
    AtomLargeBuilder& setBoolField8(bool value) {
        atom.bool_field_8 = value;
        return *this;
    }
    AtomLargeBuilder& setBoolField9(bool value) {
        atom.bool_field_9 = value;
        return *this;
    }
    AtomLargeBuilder& setBoolField10(bool value) {
        atom.bool_field_10 = value;
        return *this;
    }
    AtomLargeBuilder& setFloatField1(float value) {
        atom.float_field_1 = value;
        return *this;
    }
    AtomLargeBuilder& setFloatField2(float value) {
        atom.float_field_2 = value;
        return *this;
    }
    AtomLargeBuilder& setFloatField3(float value) {
        atom.float_field_3 = value;
        return *this;
    }
    AtomLargeBuilder& setFloatField4(float value) {
        atom.float_field_4 = value;
        return *this;
    }
    AtomLargeBuilder& setFloatField5(float value) {
        atom.float_field_5 = value;
        return *this;
    }
    AtomLargeBuilder& setFloatField6(float value) {
        atom.float_field_6 = value;
        return *this;
    }
    AtomLargeBuilder& setFloatField7(float value) {
        atom.float_field_7 = value;
        return *this;
    }
    AtomLargeBuilder& setFloatField8(float value) {
        atom.float_field_8 = value;
        return *this;
    }
    AtomLargeBuilder& setFloatField9(float value) {
        atom.float_field_9 = value;
        return *this;
    }
    AtomLargeBuilder& setFloatField10(float value) {
        atom.float_field_10 = value;
        return *this;
    }

    const AtomLarge& build() const {
        return atom;
    }

private:
    AtomLarge atom;
};

struct AtomMedium {
    std::string string_field_1;
    std::string string_field_2;
    std::string string_field_3;
    std::string string_field_4;
    std::string string_field_5;
    int32_t int32_field_1;
    int32_t int32_field_2;
    int32_t int32_field_3;
    int32_t int32_field_4;
    int32_t int32_field_5;
    int32_t int32_field_6;
    int32_t int32_field_7;
    int32_t int32_field_8;
    int32_t int32_field_9;
    int32_t int32_field_10;
    int64_t int64_field_1;
    int64_t int64_field_2;
    int64_t int64_field_3;
    int64_t int64_field_4;
    int64_t int64_field_5;
    int64_t int64_field_6;
    int64_t int64_field_7;
    int64_t int64_field_8;
    int64_t int64_field_9;
    int64_t int64_field_10;
    bool bool_field_1;
    bool bool_field_2;
    bool bool_field_3;
    bool bool_field_4;
    bool bool_field_5;
    float float_field_1;
    float float_field_2;
    float float_field_3;
    float float_field_4;
    float float_field_5;
};

class AtomMediumBuilder {
public:
    AtomMediumBuilder& setStringField1(const std::string& value) {
        atom.string_field_1 = value;
        return *this;
    }
    AtomMediumBuilder& setStringField2(const std::string& value) {
        atom.string_field_2 = value;
        return *this;
    }
    AtomMediumBuilder& setStringField3(const std::string& value) {
        atom.string_field_3 = value;
        return *this;
    }
    AtomMediumBuilder& setStringField4(const std::string& value) {
        atom.string_field_4 = value;
        return *this;
    }
    AtomMediumBuilder& setStringField5(const std::string& value) {
        atom.string_field_5 = value;
        return *this;
    }
    AtomMediumBuilder& setInt32Field1(int32_t value) {
        atom.int32_field_1 = value;
        return *this;
    }
    AtomMediumBuilder& setInt32Field2(int32_t value) {
        atom.int32_field_2 = value;
        return *this;
    }
    AtomMediumBuilder& setInt32Field3(int32_t value) {
        atom.int32_field_3 = value;
        return *this;
    }
    AtomMediumBuilder& setInt32Field4(int32_t value) {
        atom.int32_field_4 = value;
        return *this;
    }
    AtomMediumBuilder& setInt32Field5(int32_t value) {
        atom.int32_field_5 = value;
        return *this;
    }
    AtomMediumBuilder& setInt32Field6(int32_t value) {
        atom.int32_field_6 = value;
        return *this;
    }
    AtomMediumBuilder& setInt32Field7(int32_t value) {
        atom.int32_field_7 = value;
        return *this;
    }
    AtomMediumBuilder& setInt32Field8(int32_t value) {
        atom.int32_field_8 = value;
        return *this;
    }
    AtomMediumBuilder& setInt32Field9(int32_t value) {
        atom.int32_field_9 = value;
        return *this;
    }
    AtomMediumBuilder& setInt32Field10(int32_t value) {
        atom.int32_field_10 = value;
        return *this;
    }
    AtomMediumBuilder& setInt64Field1(int64_t value) {
        atom.int64_field_1 = value;
        return *this;
    }
    AtomMediumBuilder& setInt64Field2(int64_t value) {
        atom.int64_field_2 = value;
        return *this;
    }
    AtomMediumBuilder& setInt64Field3(int64_t value) {
        atom.int64_field_3 = value;
        return *this;
    }
    AtomMediumBuilder& setInt64Field4(int64_t value) {
        atom.int64_field_4 = value;
        return *this;
    }
    AtomMediumBuilder& setInt64Field5(int64_t value) {
        atom.int64_field_5 = value;
        return *this;
    }
    AtomMediumBuilder& setInt64Field6(int64_t value) {
        atom.int64_field_6 = value;
        return *this;
    }
    AtomMediumBuilder& setInt64Field7(int64_t value) {
        atom.int64_field_7 = value;
        return *this;
    }
    AtomMediumBuilder& setInt64Field8(int64_t value) {
        atom.int64_field_8 = value;
        return *this;
    }
    AtomMediumBuilder& setInt64Field9(int64_t value) {
        atom.int64_field_9 = value;
        return *this;
    }
    AtomMediumBuilder& setInt64Field10(int64_t value) {
        atom.int64_field_10 = value;
        return *this;
    }
    AtomMediumBuilder& setBoolField1(bool value) {
        atom.bool_field_1 = value;
        return *this;
    }
    AtomMediumBuilder& setBoolField2(bool value) {
        atom.bool_field_2 = value;
        return *this;
    }
    AtomMediumBuilder& setBoolField3(bool value) {
        atom.bool_field_3 = value;
        return *this;
    }
    AtomMediumBuilder& setBoolField4(bool value) {
        atom.bool_field_4 = value;
        return *this;
    }
    AtomMediumBuilder& setBoolField5(bool value) {
        atom.bool_field_5 = value;
        return *this;
    }
    AtomMediumBuilder& setFloatField1(float value) {
        atom.float_field_1 = value;
        return *this;
    }
    AtomMediumBuilder& setFloatField2(float value) {
        atom.float_field_2 = value;
        return *this;
    }
    AtomMediumBuilder& setFloatField3(float value) {
        atom.float_field_3 = value;
        return *this;
    }
    AtomMediumBuilder& setFloatField4(float value) {
        atom.float_field_4 = value;
        return *this;
    }
    AtomMediumBuilder& setFloatField5(float value) {
        atom.float_field_5 = value;
        return *this;
    }

    const AtomMedium& build() const {
        return atom;
    }

private:
    AtomMedium atom;
};

struct AtomSmall {
    std::string string_field_1;
    std::string string_field_2;
    int32_t int32_field_1;
    int32_t int32_field_2;
    int64_t int64_field_1;
    int64_t int64_field_2;
    bool bool_field_1;
    bool bool_field_2;
    float float_field_1;
    float float_field_2;
};

class AtomSmallBuilder {
public:
    AtomSmallBuilder& setStringField1(const std::string& value) {
        atom.string_field_1 = value;
        return *this;
    }
    AtomSmallBuilder& setStringField2(const std::string& value) {
        atom.string_field_2 = value;
        return *this;
    }
    AtomSmallBuilder& setInt32Field1(int32_t value) {
        atom.int32_field_1 = value;
        return *this;
    }
    AtomSmallBuilder& setInt32Field2(int32_t value) {
        atom.int32_field_2 = value;
        return *this;
    }
    AtomSmallBuilder& setInt64Field1(int64_t value) {
        atom.int64_field_1 = value;
        return *this;
    }
    AtomSmallBuilder& setInt64Field2(int64_t value) {
        atom.int64_field_2 = value;
        return *this;
    }
    AtomSmallBuilder& setBoolField1(bool value) {
        atom.bool_field_1 = value;
        return *this;
    }
    AtomSmallBuilder& setBoolField2(bool value) {
        atom.bool_field_2 = value;
        return *this;
    }
    AtomSmallBuilder& setFloatField1(float value) {
        atom.float_field_1 = value;
        return *this;
    }
    AtomSmallBuilder& setFloatField2(float value) {
        atom.float_field_2 = value;
        return *this;
    }

    const AtomSmall& build() const {
        return atom;
    }

private:
    AtomSmall atom;
};

int writeAtom(const AtomLarge& atom) {
    AStatsEvent* statsEvent = AStatsEvent_obtain();
    AStatsEvent_setAtomId(statsEvent, 1);
    AStatsEvent_writeString(statsEvent, atom.string_field_1.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_2.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_3.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_4.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_5.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_6.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_7.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_8.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_9.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_10.c_str());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_1);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_2);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_3);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_4);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_5);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_6);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_7);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_8);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_9);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_10);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_11);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_12);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_13);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_14);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_15);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_16);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_17);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_18);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_19);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_20);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_1);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_2);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_3);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_4);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_5);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_6);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_7);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_8);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_9);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_10);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_11);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_12);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_13);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_14);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_15);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_16);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_17);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_18);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_19);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_20);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_1);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_2);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_3);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_4);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_5);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_6);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_7);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_8);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_9);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_10);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_1);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_2);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_3);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_4);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_5);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_6);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_7);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_8);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_9);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_10);
    const int ret = AStatsEvent_write(statsEvent);
    AStatsEvent_release(statsEvent);
    return ret;
}

int writeAtom(const AtomMedium& atom) {
    AStatsEvent* statsEvent = AStatsEvent_obtain();
    AStatsEvent_setAtomId(statsEvent, 2);
    AStatsEvent_writeString(statsEvent, atom.string_field_1.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_2.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_3.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_4.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_5.c_str());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_1);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_2);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_3);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_4);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_5);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_6);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_7);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_8);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_9);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_10);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_1);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_2);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_3);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_4);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_5);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_6);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_7);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_8);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_9);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_10);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_1);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_2);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_3);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_4);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_5);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_1);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_2);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_3);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_4);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_5);
    const int ret = AStatsEvent_write(statsEvent);
    AStatsEvent_release(statsEvent);
    return ret;
}

int writeAtom(const AtomSmall& atom) {
    AStatsEvent* statsEvent = AStatsEvent_obtain();
    AStatsEvent_setAtomId(statsEvent, 3);
    AStatsEvent_writeString(statsEvent, atom.string_field_1.c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_2.c_str());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_1);
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_2);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_1);
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_2);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_1);
    AStatsEvent_writeBool(statsEvent, atom.bool_field_2);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_1);
    AStatsEvent_writeFloat(statsEvent, atom.float_field_2);
    const int ret = AStatsEvent_write(statsEvent);
    AStatsEvent_release(statsEvent);
    return ret;
}

}  // namespace

static void BM_StatsWriteStructLarge(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomLarge atom;
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteStructLarge);

static void BM_StatsWriteStructMedium(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomMedium atom;
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteStructMedium);

static void BM_StatsWriteStructSmall(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomSmall atom;
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteStructSmall);

static void BM_StatsWriteStructWithInitLarge(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomLarge atom = {.string_field_1 = "value1",
                          .string_field_2 = "value2",
                          .string_field_3 = "value3",
                          .string_field_4 = "value4",
                          .string_field_5 = "value5",
                          .string_field_6 = "value6",
                          .string_field_7 = "value7",
                          .string_field_8 = "value8",
                          .string_field_9 = "value9",
                          .string_field_10 = "value10",
                          .int32_field_1 = 1,
                          .int32_field_2 = 2,
                          .int32_field_3 = 3,
                          .int32_field_4 = 4,
                          .int32_field_5 = 5,
                          .int32_field_6 = 6,
                          .int32_field_7 = 7,
                          .int32_field_8 = 8,
                          .int32_field_9 = 9,
                          .int32_field_10 = 10,
                          .int32_field_11 = 11,
                          .int32_field_12 = 12,
                          .int32_field_13 = 13,
                          .int32_field_14 = 14,
                          .int32_field_15 = 15,
                          .int32_field_16 = 16,
                          .int32_field_17 = 17,
                          .int32_field_18 = 18,
                          .int32_field_19 = 19,
                          .int32_field_20 = 20,
                          .int64_field_1 = 1L,
                          .int64_field_2 = 2L,
                          .int64_field_3 = 3L,
                          .int64_field_4 = 4L,
                          .int64_field_5 = 5L,
                          .int64_field_6 = 6L,
                          .int64_field_7 = 7L,
                          .int64_field_8 = 8L,
                          .int64_field_9 = 9L,
                          .int64_field_10 = 10L,
                          .int64_field_11 = 11L,
                          .int64_field_12 = 12L,
                          .int64_field_13 = 13L,
                          .int64_field_14 = 14L,
                          .int64_field_15 = 15L,
                          .int64_field_16 = 16L,
                          .int64_field_17 = 17L,
                          .int64_field_18 = 18L,
                          .int64_field_19 = 19L,
                          .int64_field_20 = 20L,
                          .bool_field_1 = true,
                          .bool_field_2 = false,
                          .bool_field_3 = true,
                          .bool_field_4 = false,
                          .bool_field_5 = true,
                          .bool_field_6 = false,
                          .bool_field_7 = true,
                          .bool_field_8 = false,
                          .bool_field_9 = true,
                          .bool_field_10 = false,
                          .float_field_1 = 1.0f,
                          .float_field_2 = 2.0f,
                          .float_field_3 = 3.0f,
                          .float_field_4 = 4.0f,
                          .float_field_5 = 5.0f,
                          .float_field_6 = 6.0f,
                          .float_field_7 = 7.0f,
                          .float_field_8 = 8.0f,
                          .float_field_9 = 9.0f,
                          .float_field_10 = 10.0f};
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteStructWithInitLarge);

static void BM_StatsWriteStructWithInitMedium(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomMedium atom = {.string_field_1 = "value1",
                           .string_field_2 = "value2",
                           .string_field_3 = "value3",
                           .string_field_4 = "value4",
                           .string_field_5 = "value5",
                           .int32_field_1 = 1,
                           .int32_field_2 = 2,
                           .int32_field_3 = 3,
                           .int32_field_4 = 4,
                           .int32_field_5 = 5,
                           .int32_field_6 = 6,
                           .int32_field_7 = 7,
                           .int32_field_8 = 8,
                           .int32_field_9 = 9,
                           .int32_field_10 = 10,
                           .int64_field_1 = 1L,
                           .int64_field_2 = 2L,
                           .int64_field_3 = 3L,
                           .int64_field_4 = 4L,
                           .int64_field_5 = 5L,
                           .int64_field_6 = 6L,
                           .int64_field_7 = 7L,
                           .int64_field_8 = 8L,
                           .int64_field_9 = 9L,
                           .int64_field_10 = 10L,
                           .bool_field_1 = true,
                           .bool_field_2 = false,
                           .bool_field_3 = true,
                           .bool_field_4 = false,
                           .bool_field_5 = true,
                           .float_field_1 = 1.0f,
                           .float_field_2 = 2.0f,
                           .float_field_3 = 3.0f,
                           .float_field_4 = 4.0f,
                           .float_field_5 = 5.0f};
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteStructWithInitMedium);

static void BM_StatsWriteStructWithBuilderLarge(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomLargeBuilder builder;
        builder.setStringField1("value1")
                .setInt32Field1(1)
                .setInt64Field1(1L)
                .setBoolField1(true)
                .setFloatField1(1.0f);
        builder.setStringField2("value2")
                .setInt32Field2(2)
                .setInt64Field2(2L)
                .setBoolField2(false)
                .setFloatField2(2.0f);
        builder.setStringField3("value3")
                .setInt32Field3(3)
                .setInt64Field3(3L)
                .setBoolField3(true)
                .setFloatField3(3.0f);
        builder.setStringField4("value4")
                .setInt32Field4(4)
                .setInt64Field4(4L)
                .setBoolField4(false)
                .setFloatField4(4.0f);
        builder.setStringField5("value5")
                .setInt32Field5(5)
                .setInt64Field5(5L)
                .setBoolField5(true)
                .setFloatField5(5.0f);
        builder.setStringField6("value6")
                .setInt32Field6(6)
                .setInt64Field6(6L)
                .setBoolField6(false)
                .setFloatField6(6.0f);
        builder.setStringField7("value7")
                .setInt32Field7(7)
                .setInt64Field7(7L)
                .setBoolField7(true)
                .setFloatField7(7.0f);
        builder.setStringField8("value8")
                .setInt32Field8(8)
                .setInt64Field8(8L)
                .setBoolField8(false)
                .setFloatField8(8.0f);
        builder.setStringField9("value9")
                .setInt32Field9(9)
                .setInt64Field9(9L)
                .setBoolField9(true)
                .setFloatField9(9.0f);
        builder.setStringField10("value10")
                .setInt32Field10(10)
                .setInt64Field10(10L)
                .setBoolField10(false)
                .setFloatField10(10.0f);
        builder.setInt32Field11(11).setInt64Field11(11L);
        builder.setInt32Field12(12).setInt64Field12(12L);
        builder.setInt32Field13(13).setInt64Field13(13L);
        builder.setInt32Field14(14).setInt64Field14(14L);
        builder.setInt32Field15(15).setInt64Field15(15L);
        builder.setInt32Field16(16).setInt64Field16(16L);
        builder.setInt32Field17(17).setInt64Field17(17L);
        builder.setInt32Field18(18).setInt64Field18(18L);
        builder.setInt32Field19(19).setInt64Field19(19L);
        builder.setInt32Field20(20).setInt64Field20(20L);
        benchmark::DoNotOptimize(writeAtom(builder.build()));
    }
}
BENCHMARK(BM_StatsWriteStructWithBuilderLarge);

static void BM_StatsWriteStructWithBuilderMedium(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomMediumBuilder builder;
        builder.setStringField1("value1")
                .setInt32Field1(1)
                .setInt64Field1(1L)
                .setBoolField1(true)
                .setFloatField1(1.0f);
        builder.setStringField2("value2")
                .setInt32Field2(2)
                .setInt64Field2(2L)
                .setBoolField2(false)
                .setFloatField2(2.0f);
        builder.setStringField3("value3")
                .setInt32Field3(3)
                .setInt64Field3(3L)
                .setBoolField3(true)
                .setFloatField3(3.0f);
        builder.setStringField4("value4")
                .setInt32Field4(4)
                .setInt64Field4(4L)
                .setBoolField4(false)
                .setFloatField4(4.0f);
        builder.setStringField5("value5")
                .setInt32Field5(5)
                .setInt64Field5(5L)
                .setBoolField5(true)
                .setFloatField5(5.0f);
        builder.setInt32Field6(6).setInt64Field6(6L);
        builder.setInt32Field7(7).setInt64Field7(7L);
        builder.setInt32Field8(8).setInt64Field8(8L);
        builder.setInt32Field9(9).setInt64Field9(9L);
        builder.setInt32Field10(10).setInt64Field10(10L);
        benchmark::DoNotOptimize(writeAtom(builder.build()));
    }
}
BENCHMARK(BM_StatsWriteStructWithBuilderMedium);

static void BM_StatsWriteStructWithInitSmall(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomSmall atom = {.string_field_1 = "value1",
                          .string_field_2 = "value2",
                          .int32_field_1 = 1,
                          .int32_field_2 = 2,
                          .int64_field_1 = 1L,
                          .int64_field_2 = 2L,
                          .bool_field_1 = true,
                          .bool_field_2 = false,
                          .float_field_1 = 1.0f,
                          .float_field_2 = 2.0f};
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteStructWithInitSmall);

static void BM_StatsWriteStructWithBuilderSmall(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomSmallBuilder builder;
        builder.setStringField1("value1")
                .setInt32Field1(1)
                .setInt64Field1(1L)
                .setBoolField1(true)
                .setFloatField1(1.0f);
        builder.setStringField2("value2")
                .setInt32Field2(2)
                .setInt64Field2(2L)
                .setBoolField2(false)
                .setFloatField2(2.0f);
        benchmark::DoNotOptimize(writeAtom(builder.build()));
    }
}
BENCHMARK(BM_StatsWriteStructWithBuilderSmall);
