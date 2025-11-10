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
#include <src/atom_large.pb.h>
#include <src/atom_medium.pb.h>
#include <src/atom_small.pb.h>
#include <stats_event.h>

namespace {

int writeAtom(const AtomLarge& atom) {
    AStatsEvent* statsEvent = AStatsEvent_obtain();
    AStatsEvent_setAtomId(statsEvent, 1);
    AStatsEvent_writeString(statsEvent, atom.string_field_1().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_2().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_3().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_4().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_5().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_6().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_7().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_8().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_9().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_10().c_str());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_1());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_2());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_3());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_4());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_5());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_6());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_7());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_8());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_9());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_10());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_11());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_12());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_13());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_14());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_15());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_16());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_17());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_18());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_19());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_20());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_1());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_2());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_3());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_4());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_5());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_6());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_7());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_8());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_9());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_10());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_11());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_12());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_13());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_14());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_15());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_16());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_17());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_18());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_19());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_20());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_1());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_2());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_3());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_4());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_5());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_6());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_7());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_8());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_9());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_10());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_1());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_2());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_3());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_4());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_5());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_6());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_7());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_8());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_9());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_10());
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_1()));
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_2()));
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_3()));
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_4()));
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_5()));
    const int ret = AStatsEvent_write(statsEvent);
    AStatsEvent_release(statsEvent);
    return ret;
}

int writeAtom(const AtomMedium& atom) {
    AStatsEvent* statsEvent = AStatsEvent_obtain();
    AStatsEvent_setAtomId(statsEvent, 2);
    AStatsEvent_writeString(statsEvent, atom.string_field_1().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_2().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_3().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_4().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_5().c_str());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_1());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_2());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_3());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_4());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_5());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_6());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_7());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_8());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_9());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_10());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_1());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_2());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_3());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_4());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_5());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_6());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_7());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_8());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_9());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_10());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_1());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_2());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_3());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_4());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_5());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_1());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_2());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_3());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_4());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_5());
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_1()));
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_2()));
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_3()));
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_4()));
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_5()));
    const int ret = AStatsEvent_write(statsEvent);
    AStatsEvent_release(statsEvent);
    return ret;
}

int writeAtom(const AtomSmall& atom) {
    AStatsEvent* statsEvent = AStatsEvent_obtain();
    AStatsEvent_setAtomId(statsEvent, 3);
    AStatsEvent_writeString(statsEvent, atom.string_field_1().c_str());
    AStatsEvent_writeString(statsEvent, atom.string_field_2().c_str());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_1());
    AStatsEvent_writeInt32(statsEvent, atom.int32_field_2());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_1());
    AStatsEvent_writeInt64(statsEvent, atom.int64_field_2());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_1());
    AStatsEvent_writeBool(statsEvent, atom.bool_field_2());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_1());
    AStatsEvent_writeFloat(statsEvent, atom.float_field_2());
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_1()));
    AStatsEvent_writeInt32(statsEvent, static_cast<int32_t>(atom.enum_field_2()));
    const int ret = AStatsEvent_write(statsEvent);
    AStatsEvent_release(statsEvent);
    return ret;
}

}  // namespace

static void BM_StatsWriteProtobufLarge(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomLarge atom;
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteProtobufLarge);

static void BM_StatsWriteProtobufMedium(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomMedium atom;
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteProtobufMedium);

static void BM_StatsWriteProtobufSmall(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomSmall atom;
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteProtobufSmall);

static void BM_StatsWriteProtobufWithInitLarge(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomLarge atom;
        atom.set_string_field_1("value1");
        atom.set_string_field_2("value2");
        atom.set_string_field_3("value3");
        atom.set_string_field_4("value4");
        atom.set_string_field_5("value5");
        atom.set_string_field_6("value6");
        atom.set_string_field_7("value7");
        atom.set_string_field_8("value8");
        atom.set_string_field_9("value9");
        atom.set_string_field_10("value10");
        atom.set_int32_field_1(1);
        atom.set_int32_field_2(2);
        atom.set_int32_field_3(3);
        atom.set_int32_field_4(4);
        atom.set_int32_field_5(5);
        atom.set_int32_field_6(6);
        atom.set_int32_field_7(7);
        atom.set_int32_field_8(8);
        atom.set_int32_field_9(9);
        atom.set_int32_field_10(10);
        atom.set_int32_field_11(11);
        atom.set_int32_field_12(12);
        atom.set_int32_field_13(13);
        atom.set_int32_field_14(14);
        atom.set_int32_field_15(15);
        atom.set_int32_field_16(16);
        atom.set_int32_field_17(17);
        atom.set_int32_field_18(18);
        atom.set_int32_field_19(19);
        atom.set_int32_field_20(20);
        atom.set_int64_field_1(1L);
        atom.set_int64_field_2(2L);
        atom.set_int64_field_3(3L);
        atom.set_int64_field_4(4L);
        atom.set_int64_field_5(5L);
        atom.set_int64_field_6(6L);
        atom.set_int64_field_7(7L);
        atom.set_int64_field_8(8L);
        atom.set_int64_field_9(9L);
        atom.set_int64_field_10(10L);
        atom.set_int64_field_11(11L);
        atom.set_int64_field_12(12L);
        atom.set_int64_field_13(13L);
        atom.set_int64_field_14(14L);
        atom.set_int64_field_15(15L);
        atom.set_int64_field_16(16L);
        atom.set_int64_field_17(17L);
        atom.set_int64_field_18(18L);
        atom.set_int64_field_19(19L);
        atom.set_int64_field_20(20L);
        atom.set_bool_field_1(true);
        atom.set_bool_field_2(false);
        atom.set_bool_field_3(true);
        atom.set_bool_field_4(false);
        atom.set_bool_field_5(true);
        atom.set_bool_field_6(false);
        atom.set_bool_field_7(true);
        atom.set_bool_field_8(false);
        atom.set_bool_field_9(true);
        atom.set_bool_field_10(false);
        atom.set_float_field_1(1.0f);
        atom.set_float_field_2(2.0f);
        atom.set_float_field_3(3.0f);
        atom.set_float_field_4(4.0f);
        atom.set_float_field_5(5.0f);
        atom.set_float_field_6(6.0f);
        atom.set_float_field_7(7.0f);
        atom.set_float_field_8(8.0f);
        atom.set_float_field_9(9.0f);
        atom.set_float_field_10(10.0f);
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteProtobufWithInitLarge);

static void BM_StatsWriteProtobufWithInitMedium(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomMedium atom;
        atom.set_string_field_1("value1");
        atom.set_string_field_2("value2");
        atom.set_string_field_3("value3");
        atom.set_string_field_4("value4");
        atom.set_string_field_5("value5");
        atom.set_int32_field_1(1);
        atom.set_int32_field_2(2);
        atom.set_int32_field_3(3);
        atom.set_int32_field_4(4);
        atom.set_int32_field_5(5);
        atom.set_int32_field_6(6);
        atom.set_int32_field_7(7);
        atom.set_int32_field_8(8);
        atom.set_int32_field_9(9);
        atom.set_int32_field_10(10);
        atom.set_int64_field_1(1L);
        atom.set_int64_field_2(2L);
        atom.set_int64_field_3(3L);
        atom.set_int64_field_4(4L);
        atom.set_int64_field_5(5L);
        atom.set_int64_field_6(6L);
        atom.set_int64_field_7(7L);
        atom.set_int64_field_8(8L);
        atom.set_int64_field_9(9L);
        atom.set_int64_field_10(10L);
        atom.set_bool_field_1(true);
        atom.set_bool_field_2(false);
        atom.set_bool_field_3(true);
        atom.set_bool_field_4(false);
        atom.set_bool_field_5(true);
        atom.set_float_field_1(1.0f);
        atom.set_float_field_2(2.0f);
        atom.set_float_field_3(3.0f);
        atom.set_float_field_4(4.0f);
        atom.set_float_field_5(5.0f);
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteProtobufWithInitMedium);

static void BM_StatsWriteProtobufWithInitSmall(benchmark::State& state) {
    while (state.KeepRunning()) {
        AtomSmall atom;
        atom.set_string_field_1("value1");
        atom.set_string_field_2("value2");
        atom.set_int32_field_1(1);
        atom.set_int32_field_2(2);
        atom.set_int64_field_1(1L);
        atom.set_int64_field_2(2L);
        atom.set_bool_field_1(true);
        atom.set_bool_field_2(false);
        atom.set_float_field_1(1.0f);
        atom.set_float_field_2(2.0f);
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteProtobufWithInitSmall);
