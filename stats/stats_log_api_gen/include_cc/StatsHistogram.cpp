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

#include "StatsHistogram.h"

// HEADER_BEGIN
#include <algorithm>
#include <cmath>
#include <cstdlib>
// HEADER_END

namespace android {
namespace util {
namespace statslogapigen {

// BODY_BEGIN
std::unique_ptr<StatsHistogram> StatsHistogram::createLinearBins(float min, float max, int count) {
    // 2 extra bins for underflow and overflow.
    std::vector<float> bins(count + 2);
    bins[0] = UNDERFLOW_BIN;
    bins[1] = min;
    bins.back() = max;

    const float binWidth = (max - min) / count;
    float curBin = min;

    // Generate values starting from 3rd element to (n-1)th element.
    std::generate(bins.begin() + 2, bins.end() - 1,
                  [&curBin, binWidth]() { return curBin += binWidth; });

    return std::make_unique<StatsHistogram>(bins);
}

std::unique_ptr<StatsHistogram> StatsHistogram::createExponentialBins(float min, float max,
                                                                      int count) {
    // 2 extra bins for underflow and overflow.
    std::vector<float> bins(count + 2);
    bins[0] = UNDERFLOW_BIN;
    bins[1] = min;
    bins.back() = max;

    // Determine the scale factor f, such that max = min * f^count.
    // So, f = (max / min)^(1 / count) ie. f is the count'th-root of max / min.
    const float factor = std::pow(max / min, 1.0 / count);

    // Generate values starting from 3rd element to (n-1)th element.
    float curBin = bins[1];
    std::generate(bins.begin() + 2, bins.end() - 1,
                  [&curBin, factor]() { return curBin *= factor; });

    return std::make_unique<StatsHistogram>(bins);
}

std::unique_ptr<StatsHistogram> StatsHistogram::createExplicitBins(
        std::initializer_list<float> bins) {
    // 1 extra bin for underflow.
    std::vector<float> actualBins(bins.size() + 1);
    actualBins[0] = UNDERFLOW_BIN;
    std::copy(bins.begin(), bins.end(), actualBins.begin() + 1);

    return std::make_unique<StatsHistogram>(actualBins);
}

void StatsHistogram::addValue(float value) {
    size_t index = 0;
    for (; index < mBins.size() - 1; index++) {
        if (value < mBins[index + 1]) {
            break;
        }
    }
    mBinCounts[index]++;
}

void StatsHistogram::clear() {
    std::fill(mBinCounts.begin(), mBinCounts.end(), 0);
}

const std::vector<int>& StatsHistogram::getBinCounts() const {
    return mBinCounts;
}

const std::vector<float>& StatsHistogram::getBins() const {
    return mBins;
}
// BODY_END

}  // namespace statslogapigen
}  // namespace util
}  // namespace android
