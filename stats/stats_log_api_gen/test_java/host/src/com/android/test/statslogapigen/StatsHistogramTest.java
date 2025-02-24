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

import com.google.common.collect.ImmutableList;
import com.google.testing.junit.testparameterinjector.TestParameterInjector;
import com.google.testing.junit.testparameterinjector.TestParameter;
import com.google.testing.junit.testparameterinjector.TestParameterValuesProvider;

import android.util.statslogapigen.StatsHistogram;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.Arrays;

@RunWith(TestParameterInjector.class)
public class StatsHistogramTest {

    private static final class StatsHistogramProvider extends TestParameterValuesProvider {
        @Override
        protected ImmutableList<?> provideValues(Context context) {
            return ImmutableList.of(
                value(StatsHistogram.createLinearBins(0f, 50f, 5)).withName("linear"),
                value(StatsHistogram.createExponentialBins(5f, 160f, 5)).withName("exponential"),
                value(StatsHistogram.createExplicitBins(-10, -7, 0, 19, 100)).withName("explicit"));
        }
    }

    @Test
    public void testLinearInitialState() throws Exception {
        StatsHistogram hist = StatsHistogram.createLinearBins(0f, 50f, 5);
        assertThat(hist.getBinCounts()).isEqualTo(new int[7]);
        assertThat(hist.getBins()).isEqualTo(
                new float[] {StatsHistogram.UNDERFLOW, 0, 10, 20, 30, 40, 50});
    }

    @Test
    public void testExponentialInitialState() throws Exception {
        StatsHistogram hist = StatsHistogram.createExponentialBins(5f, 160f, 5);
        assertThat(hist.getBinCounts()).isEqualTo(new int[7]);
        assertThat(hist.getBins()).isEqualTo(
                new float[] {StatsHistogram.UNDERFLOW, 5, 10, 20, 40, 80, 160});
    }

    @Test
    public void testExplicitInitialState() throws Exception {
        StatsHistogram hist = StatsHistogram.createExplicitBins(-10, -7, 0, 19, 100);
        assertThat(hist.getBinCounts()).isEqualTo(new int[6]);
        assertThat(hist.getBins()).isEqualTo(
                new float[] {StatsHistogram.UNDERFLOW, -10, -7, 0, 19, 100});
    }

    @Test
    public void testSingleEntryLinear() throws Exception {
        StatsHistogram hist = StatsHistogram.createLinearBins(0f, 50f, 5);
        hist.addValue(18);
        assertThat(hist.getBinCounts()).isEqualTo(new int[] {0, 0, 1, 0, 0, 0, 0});
    }

    @Test
    public void testSingleEntryExponential() throws Exception {
        StatsHistogram hist = StatsHistogram.createExponentialBins(5f, 160f, 5);
        hist.addValue(101);
        assertThat(hist.getBinCounts()).isEqualTo(new int[] {0, 0, 0, 0, 0, 1, 0});
    }

    @Test
    public void testSingleEntryExplicit() throws Exception {
        StatsHistogram hist = StatsHistogram.createExplicitBins(-10, -7, 0, 19, 100);
        hist.addValue(0);
        assertThat(hist.getBinCounts()).isEqualTo(new int[] {0, 0, 0, 1, 0, 0});
    }

    @Test
    public void testUnderflow(
            @TestParameter(valuesProvider = StatsHistogramProvider.class) StatsHistogram hist)
            throws Exception {
        hist.addValue(-100);
        int[] binCounts = hist.getBinCounts();
        assertThat(binCounts[0]).isEqualTo(1);

        // Check that all elements after first element are 0.
        assertThat(Arrays.stream(binCounts).skip(1).distinct()).containsExactly(0);
    }

    @Test
    public void testOverflow(
            @TestParameter(valuesProvider = StatsHistogramProvider.class) StatsHistogram hist)
            throws Exception {
        hist.addValue(200);
        int[] binCounts = hist.getBinCounts();
        assertThat(binCounts[binCounts.length - 1]).isEqualTo(1);

        // Check that all elements before last element are 0.
        assertThat(Arrays.stream(binCounts)
                .limit(binCounts.length - 1).distinct()).containsExactly(0);
    }

    @Test
    public void testAddUnderflowMarker(
            @TestParameter(valuesProvider = StatsHistogramProvider.class) StatsHistogram hist)
            throws Exception {
        hist.addValue(StatsHistogram.UNDERFLOW);
        int[] binCounts = hist.getBinCounts();
        assertThat(binCounts[0]).isEqualTo(1);

        // Check that all elements after first element are 0.
        assertThat(Arrays.stream(binCounts).skip(1).distinct()).containsExactly(0);
    }

    @Test
    public void testMultipleEntriesLinear() throws Exception {
        StatsHistogram hist = StatsHistogram.createLinearBins(0f, 50f, 5);
        hist.addValue(18);
        hist.addValue(40);
        hist.addValue(45);
        hist.addValue(19.99999f);
        hist.addValue(27);
        hist.addValue(0.0000001f);
        assertThat(hist.getBinCounts()).isEqualTo(new int[] {0, 1, 2, 1, 0, 2, 0});
    }

    @Test
    public void testMultipleEntriesExponential() throws Exception {
        StatsHistogram hist = StatsHistogram.createExponentialBins(5f, 160f, 5);
        hist.addValue(101);
        hist.addValue(40);
        hist.addValue(45);
        hist.addValue(159.99999f);
        hist.addValue(160.000001f);
        hist.addValue(80);
        assertThat(hist.getBinCounts()).isEqualTo(new int[] {0, 0, 0, 0, 2, 3, 1});
    }

    @Test
    public void testMultipleEntriesExplicit() throws Exception {
        StatsHistogram hist = StatsHistogram.createExplicitBins(-10, -7, 0, 19, 100);
        hist.addValue(0);
        hist.addValue(-10);
        hist.addValue(1);
        hist.addValue(25);
        hist.addValue(49);
        hist.addValue(-2);
        assertThat(hist.getBinCounts()).isEqualTo(new int[] {0, 1, 1, 2, 2, 0});
    }

    @Test
    public void testClear(
            @TestParameter(valuesProvider = StatsHistogramProvider.class) StatsHistogram hist)
            throws Exception {
        for (float v = -20; v <= 200; v += 3) {
            hist.addValue(v);
        }
        hist.clear();

        // Check that all elements are 0.
        assertThat(Arrays.stream(hist.getBinCounts()).distinct()).containsExactly(0);
    }
}
