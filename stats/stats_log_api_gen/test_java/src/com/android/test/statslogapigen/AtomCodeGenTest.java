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

package com.android.test.statslogapigen;

import static com.google.common.truth.Truth.assertThat;

import com.android.cts.statslogapigen.CtsAtomsLog;
import com.android.test.statslogapigen.TestAtomsLog;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

/**
 * Runs the stats-log-api-gen tests for atoms java generated code
 */
@RunWith(JUnit4.class)
public class AtomCodeGenTest {
    @Test
    public void testAtomConstants() throws Exception {
        assertThat(TestAtomsLog.BLE_SCAN_STATE_CHANGED).isEqualTo(2);
        assertThat(CtsAtomsLog.TEST_ATOM_REPORTED).isEqualTo(205);
    }

    @Test
    public void testCreateLinearHistogram() throws Exception {
        TestAtomsLog.StatsHistogram hist =
                TestAtomsLog.createTestExtensionAtomReported_LinearHistogramHistogram();
        assertThat(hist.getBinCounts()).isEqualTo(new int[12]);
        assertThat(hist.getBins())
                .isEqualTo(new float[] {TestAtomsLog.StatsHistogram.UNDERFLOW, 0, 10, 20, 30, 40,
                        50, 60, 70, 80, 90, 100});
    }

    @Test
    public void testCreateExponentialHistogram() throws Exception {
        TestAtomsLog.StatsHistogram hist =
                TestAtomsLog.createTestExtensionAtomReported_ExponentialHistogramHistogram();
        assertThat(hist.getBinCounts()).isEqualTo(new int[7]);
        assertThat(hist.getBins())
                .isEqualTo(new float[] {
                        TestAtomsLog.StatsHistogram.UNDERFLOW, 5, 10, 20, 40, 80, 160});
    }

    @Test
    public void testCreateExplicitHistogram() throws Exception {
        TestAtomsLog.StatsHistogram hist =
                TestAtomsLog.createTestExtensionAtomReported_ExplicitHistogramHistogram();
        assertThat(hist.getBinCounts()).isEqualTo(new int[6]);
        assertThat(hist.getBins())
                .isEqualTo(
                        new float[] {TestAtomsLog.StatsHistogram.UNDERFLOW, -10, -7, 0, 19, 100});
    }
}
