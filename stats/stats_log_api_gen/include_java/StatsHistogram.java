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

package android.util.statslogapigen;

// HEADER_BEGIN
import java.util.Arrays;
// HEADER_END

/**
 * @hide
 **/
// BODY_BEGIN
public final class StatsHistogram {
    private final float[] mBins;
    private final int[] mBinCounts;

    public static final float UNDERFLOW = -Float.MAX_VALUE;

    private StatsHistogram(final float[] bins) {
        mBins = bins;
        mBinCounts = new int[bins.length];
    }

    /**
     * Create StatsHistogram with uniform-width bins.
     **/
    public static StatsHistogram createLinearBins(float min, float max, int count) {
        // 2 extra bins for underflow and overflow.
        float[] bins = new float[count + 2];
        bins[0] = UNDERFLOW;
        bins[1] = min;
        bins[bins.length - 1] = max;

        float binWidth = (max - min) / count;
        for (int i = 2; i < bins.length - 1; i++) {
            bins[i] = bins[i - 1] + binWidth;
        }

        return new StatsHistogram(bins);
    }

    /**
     * Create StatsHistogram with bin-widths increasing exponentially.
     **/
    public static StatsHistogram createExponentialBins(float min, float max, int count) {
        // 2 extra bins for underflow and overflow
        float[] bins = new float[count + 2];
        bins[0] = UNDERFLOW;
        bins[1] = min;
        bins[bins.length - 1] = max;

        // Determine the scale factor f, such that max = min * f^count.
        // So, f = (max / min)^(1 / count) ie. f is the count'th-root of max / min.
        float factor = (float) Math.pow(max / min, 1.0 / count);
        for (int i = 2; i < bins.length - 1; i++) {
            bins[i] = bins[i - 1] * factor;
        }

        return new StatsHistogram(bins);
    }

    /**
     * Create StatsHistogram with bin-widths specified by adjacent values in explicitBins
     **/
    public static StatsHistogram createExplicitBins(float... explicitBins) {
        // 1 extra bin for underflow.
        float[] bins = new float[explicitBins.length + 1];
        bins[0] = UNDERFLOW;
        System.arraycopy(explicitBins, 0, bins, 1, explicitBins.length);

        return new StatsHistogram(bins);
    }

    /**
     * Add a single value to this StatsHistogram.
     **/
    public void addValue(float value) {
        int i = 0;
        for (; i < mBins.length - 1; i++) {
            if (value < mBins[i + 1]) {
                break;
            }
        }

        mBinCounts[i]++;
    }

    /**
     * Clear all bin counts
     **/
    public void clear() {
        Arrays.fill(mBinCounts, 0);
    }

    /**
     * Get counts for all bins.
     **/
    public int[] getBinCounts() {
        return mBinCounts;
    }

    /**
     * Get all the bin boundaries for the histogram
     **/
    public float[] getBins() {
        return mBins;
    }
}
// BODY_END
