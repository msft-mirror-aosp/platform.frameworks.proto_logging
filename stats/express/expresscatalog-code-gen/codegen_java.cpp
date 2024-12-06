//
// Copyright (C) 2022 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "codegen_java.h"

#include <expresscatalog-utils.h>
#include <stdio.h>

namespace android {
namespace express {

void writeJavaFilePreamble(FILE* fd) {
    fprintf(fd, "// DO NOT EDIT THIS FILE\n");
    fprintf(fd, "// This is auto-generated file by expresscatalog-codegen\n\n");
}

void writeJavaPackagePreamble(FILE* fd, const std::string& packageName) {
    fprintf(fd, "package %s;\n\n", packageName.c_str());

    fprintf(fd, "import android.util.ArrayMap;\n\n");
    fprintf(fd, "import java.util.InputMismatchException;\n\n");
}

const char* getMetricTypeString(int metricType) {
    static const char* metricTypeStringMap[] = {
            "METRIC_TYPE_UNKNOWN",
            "METRIC_TYPE_COUNTER",
            "METRIC_TYPE_HISTOGRAM",
            "METRIC_TYPE_COUNTER_WITH_UID",
            "METRIC_TYPE_HISTOGRAM_WITH_UID",
    };

    if (metricType == 0) {
        return nullptr;
    }

    return metricTypeStringMap[metricType];
}

void writeJavaHashMapInitFunction(FILE* fd, const MetricInfoMap& metricsIds) {
    fprintf(fd, "private static ArrayMap<String, MetricInfo> metricIds;\n");
    fprintf(fd, "static {\n");
    fprintf(fd, "    metricIds = new ArrayMap<String, MetricInfo>();\n");
    for (const auto& metricInfo : metricsIds) {
        const char* metricTypeString = getMetricTypeString(metricInfo.second.type);
        if (metricTypeString != nullptr) {
            fprintf(fd, "    metricIds.put(\"%s\", new MetricInfo(%ldl, %s));\n",
                    metricInfo.first.c_str(), metricInfo.second.hash, metricTypeString);
        } else {
            LOGE("Metric type is undefined for %s. Termination\n", metricInfo.first.c_str());
            exit(-1);
        }
    }
    fprintf(fd, "}\n\n");
}

bool CodeGeneratorJava::generateCodeImpl(FILE* fd, const MetricInfoMap& metricsIds) const {
    writeJavaFilePreamble(fd);
    writeJavaPackagePreamble(fd, mPackageName);

    fprintf(fd, "public final class %s {\n\n", mClassName.c_str());

    // TODO: auto-generate enum int constants
    fprintf(fd, "public static final long INVALID_METRIC_ID = 0;\n");
    fprintf(fd, "public static final int METRIC_TYPE_UNKNOWN = 0;\n");
    fprintf(fd, "public static final int METRIC_TYPE_COUNTER = 1;\n");
    fprintf(fd, "public static final int METRIC_TYPE_HISTOGRAM = 2;\n");
    fprintf(fd, "public static final int METRIC_TYPE_COUNTER_WITH_UID = 3;\n");
    fprintf(fd, "public static final int METRIC_TYPE_HISTOGRAM_WITH_UID = 4;\n\n");

    fprintf(fd, "private static final class MetricInfo {\n");
    fprintf(fd, "   MetricInfo(long hash, int type) {\n");
    fprintf(fd, "       mHash = hash;\n");
    fprintf(fd, "       mType = type;\n");
    fprintf(fd, "   }\n");
    fprintf(fd, "   public long mHash;\n");
    fprintf(fd, "   public int mType;\n");
    fprintf(fd, "}\n\n");

    writeJavaHashMapInitFunction(fd, metricsIds);

    fprintf(fd, "static long getMetricIdHash(String metricId, int type) {\n");
    fprintf(fd, "    MetricInfo info = metricIds.get(metricId);\n");
    fprintf(fd, "    if(info == null || info.mType != type) {\n");
    fprintf(fd, "        return INVALID_METRIC_ID;\n");
    fprintf(fd, "    }\n");
    fprintf(fd, "    return info.mHash;\n");
    fprintf(fd, "}\n\n");

    fprintf(fd, "}\n\n");

    return true;
}

}  // namespace express
}  // namespace android
