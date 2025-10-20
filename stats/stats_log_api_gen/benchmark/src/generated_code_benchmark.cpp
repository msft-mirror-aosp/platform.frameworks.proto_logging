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
#include <stats_annotations.h>
#include <statslog_test.h>

using namespace android::stats;

/**
 * Explicit copy of generated implementation is done to support few atoms
 * copied here to introduce a baseline for historical performance reference
 */
int stats_write_baseline(int32_t code, int32_t arg1, int32_t arg2, int32_t arg3) {
    AStatsEvent* event = AStatsEvent_obtain();
    AStatsEvent_setAtomId(event, code);
    AStatsEvent_writeInt32(event, arg1);
    if (ISOLATED_UID_CHANGED == code) {
        AStatsEvent_addBoolAnnotation(event, ASTATSLOG_ANNOTATION_ID_IS_UID, true);
    }
    if (APP_BREADCRUMB_REPORTED == code) {
        AStatsEvent_addBoolAnnotation(event, ASTATSLOG_ANNOTATION_ID_IS_UID, true);
    }
    AStatsEvent_writeInt32(event, arg2);
    if (ISOLATED_UID_CHANGED == code) {
        AStatsEvent_addBoolAnnotation(event, ASTATSLOG_ANNOTATION_ID_IS_UID, true);
    }
    AStatsEvent_writeInt32(event, arg3);
    const int ret = AStatsEvent_write(event);
    AStatsEvent_release(event);
    return ret;
}

/**
 * This is copy from stats_write_baseline with code only relevant to a  single atom
 */
int stats_write_baseline_simplified(int32_t code, int32_t arg1, int32_t arg2, int32_t arg3) {
    AStatsEvent* statsEvent = AStatsEvent_obtain();
    AStatsEvent_setAtomId(statsEvent, code);
    AStatsEvent_writeInt32(statsEvent, arg1);
    AStatsEvent_addBoolAnnotation(statsEvent, ASTATSLOG_ANNOTATION_ID_IS_UID, true);
    AStatsEvent_writeInt32(statsEvent, arg2);
    AStatsEvent_writeInt32(statsEvent, arg3);
    AStatsEvent_addBoolAnnotation(statsEvent, ASTATSLOG_ANNOTATION_ID_IS_UID, true);
    const int ret = AStatsEvent_write(statsEvent);
    AStatsEvent_release(statsEvent);
    return ret;
}

struct IsolatedUidChanged {
    int32_t parent_uid = 0;
    int32_t isolated_uid = 100;
    int32_t event = 1;
};

/**
 * Same as stats_write_baseline_simplified but signature is atom-typed and arguments
 * are passed via a single struct reference
 */
int writeAtom(const IsolatedUidChanged& atom) {
    AStatsEvent* statsEvent = AStatsEvent_obtain();
    AStatsEvent_setAtomId(statsEvent, ISOLATED_UID_CHANGED);
    AStatsEvent_writeInt32(statsEvent, atom.parent_uid);
    AStatsEvent_addBoolAnnotation(statsEvent, ASTATSLOG_ANNOTATION_ID_IS_UID, true);
    AStatsEvent_writeInt32(statsEvent, atom.isolated_uid);
    AStatsEvent_writeInt32(statsEvent, atom.event);
    AStatsEvent_addBoolAnnotation(statsEvent, ASTATSLOG_ANNOTATION_ID_IS_UID, true);
    const int ret = AStatsEvent_write(statsEvent);
    AStatsEvent_release(statsEvent);
    return ret;
}

static void BM_StatsWriteBaseline(benchmark::State& state) {
    while (state.KeepRunning()) {
        int32_t parent_uid = 0;
        int32_t isolated_uid = 100;
        int32_t event = 1;
        benchmark::DoNotOptimize(
                stats_write_baseline(ISOLATED_UID_CHANGED, parent_uid, isolated_uid, event));
    }
}
BENCHMARK(BM_StatsWriteBaseline);

static void BM_StatsWrite(benchmark::State& state) {
    while (state.KeepRunning()) {
        int32_t parent_uid = 0;
        int32_t isolated_uid = 100;
        int32_t event = 1;
        benchmark::DoNotOptimize(
                stats_write(ISOLATED_UID_CHANGED, parent_uid, isolated_uid, event++));
    }
}
BENCHMARK(BM_StatsWrite);

static void BM_StatsWriteDirect(benchmark::State& state) {
    while (state.KeepRunning()) {
        int32_t parent_uid = 0;
        int32_t isolated_uid = 100;
        int32_t event = 1;
        benchmark::DoNotOptimize(stats_write_baseline_simplified(ISOLATED_UID_CHANGED, parent_uid,
                                                                 isolated_uid, event));
    }
}
BENCHMARK(BM_StatsWriteDirect);

static void BM_StatsWriteStruct(benchmark::State& state) {
    while (state.KeepRunning()) {
        IsolatedUidChanged atom;
        benchmark::DoNotOptimize(writeAtom(atom));
    }
}
BENCHMARK(BM_StatsWriteStruct);

class IsolatedUidChangedBuilder {
public:
    IsolatedUidChangedBuilder& setParentUid(int32_t parent_uid) {
        atom.parent_uid = parent_uid;
        return *this;
    }

    IsolatedUidChangedBuilder& setIsolatedUid(int32_t isolated_uid) {
        atom.isolated_uid = isolated_uid;
        return *this;
    }

    IsolatedUidChangedBuilder& setEvent(int32_t event) {
        atom.event = event;
        return *this;
    }

    void reset() {
        atom = IsolatedUidChanged();
    }

    const IsolatedUidChanged& build() const {
        return atom;
    }

private:
    IsolatedUidChanged atom;
};

static void BM_StatsWriteStructWithBuilder(benchmark::State& state) {
    while (state.KeepRunning()) {
        IsolatedUidChangedBuilder builder;
        builder.setParentUid(0).setIsolatedUid(100).setEvent(1);
        benchmark::DoNotOptimize(writeAtom(builder.build()));
    }
}
BENCHMARK(BM_StatsWriteStructWithBuilder);
