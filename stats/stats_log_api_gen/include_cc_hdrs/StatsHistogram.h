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

#pragma once

// HEADER_BEGIN
#include <initializer_list>
#include <limits>
#include <memory>
#include <vector>
// HEADER_END

namespace android {
namespace util {
namespace statslogapigen {

// BODY_BEGIN
constexpr float UNDERFLOW_BIN = std::numeric_limits<float>::lowest();

class StatsHistogram final {
public:
    constexpr StatsHistogram(std::vector<float> bins)
        : mBins(std::move(bins)), mBinCounts(mBins.size(), 0) {
    }

    /**
     * Create StatsHistogram with uniform-width bins.
     **/
    static std::unique_ptr<StatsHistogram> createLinearBins(float min, float max, int count);

    /**
     * Create StatsHistogram with bin-widths increasing exponentially.
     **/
    static std::unique_ptr<StatsHistogram> createExponentialBins(float min, float max, int count);

    /**
     * Create StatsHistogram with bin-widths specified by adjacent values in explicitBins
     **/
    static std::unique_ptr<StatsHistogram> createExplicitBins(std::initializer_list<float> bins);

    /**
     * Add a single value to this StatsHistogram.
     **/
    void addValue(float value);

    /**
     * Clear all bin counts
     **/
    void clear();

    /**
     * Get counts for all bins.
     **/
    const std::vector<int>& getBinCounts() const;

    /**
     * Get all the bin boundaries for the histogram
     **/
    const std::vector<float>& getBins() const;

private:
    const std::vector<float> mBins;
    std::vector<int> mBinCounts;
};
// BODY_END

}  // namespace statslogapigen
}  // namespace util
}  // namespace android
