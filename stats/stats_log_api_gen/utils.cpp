/*
 * Copyright (C) 2019, The Android Open Source Project
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

#include "utils.h"

#include <stdio.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Collation.h"
#include "frameworks/proto_logging/stats/atom_field_options.pb.h"
#include "settings_provider.h"

namespace android {
namespace stats_log_api_gen {

namespace fs = std::filesystem;

using std::ifstream;
using std::map;
using std::string;
using std::vector;

/**
 * Inlining this method because "android-base/strings.h" is not available on
 * google3.
 */
static vector<string> Split(const string& s, const string& delimiters) {
    vector<string> result;

    size_t base = 0;
    size_t found;
    while (true) {
        found = s.find_first_of(delimiters, base);
        result.push_back(s.substr(base, found - base));
        if (found == s.npos) break;
        base = found + 1;
    }

    return result;
}

static void write_native_histogram_helper_signature(FILE* out, const string& atomName,
                                                    const string& fieldName) {
    fprintf(out,
            "std::unique_ptr<%s> "
            "create_%s__%s_histogram()",
            HISTOGRAM_STEM.c_str(), atomName.c_str(), fieldName.c_str());
}

static int write_native_histogram_helper_definition(
        FILE* out, const string& atomName, const string& fieldName,
        const os::statsd::HistogramBinOption& histBinOption) {
    int errorCount = 0;

    // Print method signature.
    write_native_histogram_helper_signature(out, atomName, fieldName);
    fprintf(out, " {\n");

    fprintf(out, "%*sreturn %s::create", 8, "", HISTOGRAM_STEM.c_str());
    if (histBinOption.has_generated_bins()) {
        const os::statsd::HistogramBinOption::GeneratedBins& genBins =
                histBinOption.generated_bins();
        switch (genBins.strategy()) {
            case os::statsd::HistogramBinOption::GeneratedBins::LINEAR:
                fprintf(out, "Linear");
                break;
            case os::statsd::HistogramBinOption::GeneratedBins::EXPONENTIAL:
                fprintf(out, "Exponential");
                break;
            default:
                errorCount++;
        }
        fprintf(out, "Bins(%f, %f, %d);\n", genBins.min(), genBins.max(), genBins.count());
    } else if (histBinOption.has_explicit_bins()) {
        const os::statsd::HistogramBinOption::ExplicitBins& explicitBins =
                histBinOption.explicit_bins();
        fprintf(out, "ExplicitBins({");
        const char* separator = "";
        for (const float bin : explicitBins.bin()) {
            fprintf(out, "%s%f", separator, bin);
            separator = ", ";
        }
        fprintf(out, "});\n");
    }
    fprintf(out, "}\n\n");

    return errorCount;
}

static int write_java_histogram_helper(FILE* out, const string& atomName, const string& fieldName,
                                       const os::statsd::HistogramBinOption& histBinOption,
                                       const bool staticMethods) {
    int errorCount = 0;

    // Print method signature.
    const char* methodPrefix = staticMethods ? "static " : "";
    fprintf(out, "%*spublic %s%s create%s_%sHistogram() {\n", 4, "", methodPrefix,
            HISTOGRAM_STEM.c_str(), snake_to_pascal(atomName).c_str(),
            snake_to_pascal(fieldName).c_str());

    fprintf(out, "%*sreturn %s.create", 8, "", HISTOGRAM_STEM.c_str());
    if (histBinOption.has_generated_bins()) {
        const os::statsd::HistogramBinOption::GeneratedBins& genBins =
                histBinOption.generated_bins();
        switch (genBins.strategy()) {
            case os::statsd::HistogramBinOption::GeneratedBins::LINEAR:
                fprintf(out, "Linear");
                break;
            case os::statsd::HistogramBinOption::GeneratedBins::EXPONENTIAL:
                fprintf(out, "Exponential");
                break;
            default:
                errorCount++;
        }
        fprintf(out, "Bins(%ff, %ff, %d);\n", genBins.min(), genBins.max(), genBins.count());
    } else if (histBinOption.has_explicit_bins()) {
        const os::statsd::HistogramBinOption::ExplicitBins& explicitBins =
                histBinOption.explicit_bins();
        fprintf(out, "ExplicitBins(");
        const char* separator = "";
        for (const float bin : explicitBins.bin()) {
            fprintf(out, "%s%ff", separator, bin);
            separator = ", ";
        }
        fprintf(out, ");\n");
    }
    fprintf(out, "%*s}\n\n", 4, "");

    return errorCount;
}

static void write_header_histogram_sources(FILE* out, const Atoms& atoms, bool includeExtraSrcs) {
    const bool hasHistograms = has_histograms(atoms.decls);
    if (includeExtraSrcs) {
#ifdef CC_INCLUDE_HDRS_DIR
        const vector<string> excludeList =
                hasHistograms ? vector<string>{} : vector<string>{HISTOGRAM_STEM};
        write_cc_srcs_classes(out, CC_INCLUDE_HDRS_DIR, excludeList);
#endif
    } else if (hasHistograms) {
        fprintf(out, "using namespace android::util::statslogapigen;\n\n");
    }

    if (hasHistograms) {
        write_native_histogram_helper_declarations(out, atoms.decls);
    }
}

static int write_src_header(FILE* out, const fs::path& filePath) {
    ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        fprintf(stderr, "Could not open file: %s", filePath.c_str());
        return 1;
    }

    string line;
    bool atImports = false;
    while (std::getline(fileStream, line)) {
        if (line == "// HEADER_BEGIN") {
            atImports = true;
        } else if (line == "// HEADER_END") {
            break;
        } else if (atImports) {
            fprintf(out, "%s\n", line.c_str());
        }
    }
    fileStream.close();

    return 0;
}

static int write_src_body(FILE* out, const fs::path& filePath, int indent,
                          const std::function<bool(string& firstLine)>& firstLineTransformer) {
    ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        fprintf(stderr, "Could not open file: %s\n", filePath.c_str());
        return 1;
    }

    string line;
    bool atClassDef = false;

    while (std::getline(fileStream, line)) {
        if (line == "// BODY_BEGIN") {
            std::getline(fileStream, line);
            if (firstLineTransformer && !firstLineTransformer(line)) {
                fprintf(stderr, "First line transform failed: %s\n", filePath.c_str());
                return 1;
            }
            fprintf(out, "%*s%s\n", indent, "", line.c_str());
            atClassDef = true;
        } else if (line == "// BODY_END") {
            break;
        } else if (atClassDef) {
            fprintf(out, "%*s%s\n", indent, "", line.c_str());
        }
    }
    fileStream.close();

    return 0;
}

static int write_srcs_bodies(FILE* out, const char* path, int indent,
                             const vector<string>& excludeList,
                             const std::function<bool(string& firstLine)>& firstLineTransformer) {
    int errors = 0;
    const string fullPath = get_data_dir_path(path);
    for (const fs::path& filePath : fs::directory_iterator(fullPath)) {
        // Inline source bodies from filePath if it's not in excludeList.
        if (std::find(excludeList.begin(), excludeList.end(), filePath.stem()) ==
            excludeList.end()) {
            errors += write_src_body(out, filePath, indent, firstLineTransformer);
        }
    }

    return errors;
}

static int write_cpp_sources(FILE* out, const Atoms& atoms, const string& cppNamespace,
                             bool includeExtraSrcs) {
    const bool hasHistograms = has_histograms(atoms.decls);
#ifdef CC_INCLUDE_SRCS_DIR
    const vector<string> excludeList =
            hasHistograms ? vector<string>{} : vector<string>{HISTOGRAM_STEM};
    if (includeExtraSrcs) {
        write_srcs_header(out, CC_INCLUDE_SRCS_DIR, excludeList);
    }
#endif

    write_namespace(out, cppNamespace);

    int ret = 0;
#ifdef CC_INCLUDE_SRCS_DIR
    if (includeExtraSrcs) {
        ret = write_cc_srcs_classes(out, CC_INCLUDE_SRCS_DIR, excludeList);
        if (ret != 0) {
            return ret;
        }
    }
#endif

    // Write histogram helper definitions if any histogram annotations are present.
    if (hasHistograms) {
        ret = write_native_histogram_helper_definitions(out, atoms.decls);
        if (ret != 0) {
            return ret;
        }
    }
    return ret;
}

static bool make_java_class_static(string& line) {
    const size_t pos = line.find(' ');
    if (pos == string::npos) {
        return false;
    }
    line.insert(pos, " static");
    return true;
}

void build_non_chained_decl_map(const Atoms& atoms,
                                std::map<int, AtomDeclSet::const_iterator>* decl_map) {
    for (AtomDeclSet::const_iterator atomIt = atoms.non_chained_decls.begin();
         atomIt != atoms.non_chained_decls.end(); atomIt++) {
        decl_map->insert(std::make_pair((*atomIt)->code, atomIt));
    }
}

const map<AnnotationId, AnnotationStruct>& get_annotation_id_constants(const string& prefix) {
    static const map<AnnotationId, AnnotationStruct>* ANNOTATION_ID_CONSTANTS =
            new map<AnnotationId, AnnotationStruct>{
                    {ANNOTATION_ID_IS_UID, AnnotationStruct(prefix + "IS_UID", API_S)},
                    {ANNOTATION_ID_TRUNCATE_TIMESTAMP,
                     AnnotationStruct(prefix + "TRUNCATE_TIMESTAMP", API_S)},
                    {ANNOTATION_ID_PRIMARY_FIELD,
                     AnnotationStruct(prefix + "PRIMARY_FIELD", API_S)},
                    {ANNOTATION_ID_EXCLUSIVE_STATE,
                     AnnotationStruct(prefix + "EXCLUSIVE_STATE", API_S)},
                    {ANNOTATION_ID_PRIMARY_FIELD_FIRST_UID,
                     AnnotationStruct(prefix + "PRIMARY_FIELD_FIRST_UID", API_S)},
                    {ANNOTATION_ID_DEFAULT_STATE,
                     AnnotationStruct(prefix + "DEFAULT_STATE", API_S)},
                    {ANNOTATION_ID_TRIGGER_STATE_RESET,
                     AnnotationStruct(prefix + "TRIGGER_STATE_RESET", API_S)},
                    {ANNOTATION_ID_STATE_NESTED, AnnotationStruct(prefix + "STATE_NESTED", API_S)},
                    {ANNOTATION_ID_RESTRICTION_CATEGORY,
                     AnnotationStruct(prefix + "RESTRICTION_CATEGORY", API_U)},
                    {ANNOTATION_ID_FIELD_RESTRICTION_PERIPHERAL_DEVICE_INFO,
                     AnnotationStruct(prefix + "FIELD_RESTRICTION_PERIPHERAL_DEVICE_INFO", API_U)},
                    {ANNOTATION_ID_FIELD_RESTRICTION_APP_USAGE,
                     AnnotationStruct(prefix + "FIELD_RESTRICTION_APP_USAGE", API_U)},
                    {ANNOTATION_ID_FIELD_RESTRICTION_APP_ACTIVITY,
                     AnnotationStruct(prefix + "FIELD_RESTRICTION_APP_ACTIVITY", API_U)},
                    {ANNOTATION_ID_FIELD_RESTRICTION_HEALTH_CONNECT,
                     AnnotationStruct(prefix + "FIELD_RESTRICTION_HEALTH_CONNECT", API_U)},
                    {ANNOTATION_ID_FIELD_RESTRICTION_ACCESSIBILITY,
                     AnnotationStruct(prefix + "FIELD_RESTRICTION_ACCESSIBILITY", API_U)},
                    {ANNOTATION_ID_FIELD_RESTRICTION_SYSTEM_SEARCH,
                     AnnotationStruct(prefix + "FIELD_RESTRICTION_SYSTEM_SEARCH", API_U)},
                    {ANNOTATION_ID_FIELD_RESTRICTION_USER_ENGAGEMENT,
                     AnnotationStruct(prefix + "FIELD_RESTRICTION_USER_ENGAGEMENT", API_U)},
                    {ANNOTATION_ID_FIELD_RESTRICTION_AMBIENT_SENSING,
                     AnnotationStruct(prefix + "FIELD_RESTRICTION_AMBIENT_SENSING", API_U)},
                    {ANNOTATION_ID_FIELD_RESTRICTION_DEMOGRAPHIC_CLASSIFICATION,
                     AnnotationStruct(prefix + "FIELD_RESTRICTION_DEMOGRAPHIC_CLASSIFICATION",
                                      API_U)},
            };

    return *ANNOTATION_ID_CONSTANTS;
}

string get_java_build_version_code(int apiLevel) {
    switch (apiLevel) {
        case API_Q:
            return "Build.VERSION_CODES.Q";
        case API_R:
            return "Build.VERSION_CODES.R";
        case API_S:
            return "Build.VERSION_CODES.S";
        case API_S_V2:
            return "Build.VERSION_CODES.S_V2";
        case API_T:
            return "Build.VERSION_CODES.TIRAMISU";
        case API_U:
            return "Build.VERSION_CODES.UPSIDE_DOWN_CAKE";
        default:
            return "Build.VERSION_CODES.CUR_DEVELOPMENT";
    }
}

string get_restriction_category_str(int annotationValue) {
    switch (annotationValue) {
        case os::statsd::RestrictionCategory::RESTRICTION_DIAGNOSTIC:
            return "RESTRICTION_CATEGORY_DIAGNOSTIC";
        case os::statsd::RestrictionCategory::RESTRICTION_SYSTEM_INTELLIGENCE:
            return "RESTRICTION_CATEGORY_SYSTEM_INTELLIGENCE";
        case os::statsd::RestrictionCategory::RESTRICTION_AUTHENTICATION:
            return "RESTRICTION_CATEGORY_AUTHENTICATION";
        case os::statsd::RestrictionCategory::RESTRICTION_FRAUD_AND_ABUSE:
            return "RESTRICTION_CATEGORY_FRAUD_AND_ABUSE";
        default:
            return "";
    }
}

/**
 * Turn lower and camel case into upper case with underscores.
 */
string make_constant_name(const string& str) {
    string result;
    const int N = str.size();
    bool underscore_next = false;
    for (int i = 0; i < N; i++) {
        char c = str[i];
        if (c >= 'A' && c <= 'Z') {
            if (underscore_next) {
                result += '_';
                underscore_next = false;
            }
        } else if (c >= 'a' && c <= 'z') {
            c = 'A' + c - 'a';
            underscore_next = true;
        } else if (c == '_') {
            underscore_next = false;
        }
        result += c;
    }
    return result;
}

/**
 * Convert snake_case to PascalCase
 */
string snake_to_pascal(const string& snake) {
    string pascal;
    bool capitalize = true;
    for (const char c : snake) {
        if (c == '_') {
            capitalize = true;
        } else if (capitalize) {
            pascal += std::toupper(c);
            capitalize = false;
        } else {
            pascal += c;
        }
    }
    return pascal;
}

vector<AtomField> get_enum_fields(const AtomDecl& atomDecl) {
    vector<AtomField> enum_fields;
    for (const auto& field : atomDecl.fields) {
        if (field.javaType == JAVA_TYPE_ENUM || field.javaType == JAVA_TYPE_ENUM_ARRAY) {
            enum_fields.push_back(field);
        }
    }
    return enum_fields;
}

string to_cpp_typesafe_name(const AtomField& field) {
    switch (field.javaType) {
        case JAVA_TYPE_BOOLEAN:
            return "bool";
        case JAVA_TYPE_INT:
            return "int32_t";
        case JAVA_TYPE_ENUM:
            return field.enumTypeName;
        case JAVA_TYPE_LONG:
            return "int64_t";
        case JAVA_TYPE_FLOAT:
            return "float";
        case JAVA_TYPE_DOUBLE:
            return "double";
        case JAVA_TYPE_STRING:
            return "std::string";
        case JAVA_TYPE_BYTE_ARRAY:
            return "std::vector<uint8_t>";
        case JAVA_TYPE_BOOLEAN_ARRAY:
            return "std::vector<uint8_t>";
        case JAVA_TYPE_INT_ARRAY:
            return "std::vector<int32_t>";
        case JAVA_TYPE_ENUM_ARRAY:
            return "std::vector<" + field.enumTypeName + ">";
        case JAVA_TYPE_LONG_ARRAY:
            return "std::vector<int64_t>";
        case JAVA_TYPE_FLOAT_ARRAY:
            return "std::vector<float>";
        case JAVA_TYPE_STRING_ARRAY:
            return "std::vector<std::string>";
        case JAVA_TYPE_DOUBLE_ARRAY:
            return "std::vector<double>";
        default:
            return "UNKNOWN";
    }
}

const char* cpp_type_name(java_type_t type, bool isVendorAtomLogging) {
    switch (type) {
        case JAVA_TYPE_BOOLEAN:
            return "bool";
        case JAVA_TYPE_INT:  // Fallthrough.
        case JAVA_TYPE_ENUM:
            return "int32_t";
        case JAVA_TYPE_LONG:
            return "int64_t";
        case JAVA_TYPE_FLOAT:
            return "float";
        case JAVA_TYPE_DOUBLE:
            return "double";
        case JAVA_TYPE_STRING:
            return "char const*";
        case JAVA_TYPE_BYTE_ARRAY:
            return isVendorAtomLogging ? "const std::vector<uint8_t>&" : "const BytesField&";
        case JAVA_TYPE_BOOLEAN_ARRAY:
            return isVendorAtomLogging ? "const std::vector<bool>&" : "const bool*";
        case JAVA_TYPE_INT_ARRAY:  // Fallthrough.
        case JAVA_TYPE_ENUM_ARRAY:
            return "const std::vector<int32_t>&";
        case JAVA_TYPE_LONG_ARRAY:
            return "const std::vector<int64_t>&";
        case JAVA_TYPE_FLOAT_ARRAY:
            return "const std::vector<float>&";
        case JAVA_TYPE_STRING_ARRAY:
            return "const std::vector<char const*>&";
        case JAVA_TYPE_DOUBLE_ARRAY:
            return "const std::vector<double>&";
        default:
            return "UNKNOWN";
    }
}

const char* java_type_name(java_type_t type) {
    switch (type) {
        case JAVA_TYPE_BOOLEAN:
            return "boolean";
        case JAVA_TYPE_INT:  // Fallthrough.
        case JAVA_TYPE_ENUM:
            return "int";
        case JAVA_TYPE_LONG:
            return "long";
        case JAVA_TYPE_FLOAT:
            return "float";
        case JAVA_TYPE_DOUBLE:
            return "double";
        case JAVA_TYPE_STRING:
            return "java.lang.String";
        case JAVA_TYPE_BYTE_ARRAY:
            return "byte[]";
        case JAVA_TYPE_BOOLEAN_ARRAY:
            return "boolean[]";
        case JAVA_TYPE_INT_ARRAY:  // Fallthrough.
        case JAVA_TYPE_ENUM_ARRAY:
            return "int[]";
        case JAVA_TYPE_LONG_ARRAY:
            return "long[]";
        case JAVA_TYPE_FLOAT_ARRAY:
            return "float[]";
        case JAVA_TYPE_STRING_ARRAY:
            return "java.lang.String[]";
        case JAVA_TYPE_DOUBLE_ARRAY:
            return "double[]";
        default:
            return "UNKNOWN";
    }
}

// Does not include AttributionChain type.
bool is_repeated_field(java_type_t type) {
    switch (type) {
        case JAVA_TYPE_BOOLEAN_ARRAY:
        case JAVA_TYPE_INT_ARRAY:
        case JAVA_TYPE_FLOAT_ARRAY:
        case JAVA_TYPE_LONG_ARRAY:
        case JAVA_TYPE_STRING_ARRAY:
        case JAVA_TYPE_ENUM_ARRAY:
            return true;
        default:
            return false;
    }
}

static bool contains_repeated_field(const vector<java_type_t>& signature) {
    for (const java_type_t& javaType : signature) {
        if (is_repeated_field(javaType)) {
            return true;
        }
    }
    return false;
}

bool contains_repeated_field(const vector<AtomField>& atomFields) {
    for (auto& field : atomFields) {
        if (is_repeated_field(field.javaType)) {
            return true;
        }
    }
    return false;
}

bool is_primitive_field(java_type_t type) {
    switch (type) {
        case JAVA_TYPE_BOOLEAN:
        case JAVA_TYPE_INT:
        case JAVA_TYPE_LONG:
        case JAVA_TYPE_FLOAT:
        case JAVA_TYPE_STRING:
        case JAVA_TYPE_ENUM:
            return true;
        default:
            return false;
    }
}

// Native
// Writes namespaces for the cpp and header files
void write_namespace(FILE* out, const string& cppNamespaces) {
    const vector<string> cppNamespaceVec = Split(cppNamespaces, ",");
    for (const string& cppNamespace : cppNamespaceVec) {
        fprintf(out, "namespace %s {\n\n", cppNamespace.c_str());
    }
}

// Writes namespace closing brackets for cpp and header files.
void write_closing_namespace(FILE* out, const string& cppNamespaces) {
    vector<string> cppNamespaceVec = Split(cppNamespaces, ",");
    for (auto it = cppNamespaceVec.rbegin(); it != cppNamespaceVec.rend(); ++it) {
        fprintf(out, "} // namespace %s\n", it->c_str());
    }
}

static void write_cpp_usage(FILE* out, const string& method_name, const string& atom_code_name,
                            const AtomDecl& atom, const AtomDecl& attributionDecl,
                            bool isVendorAtomLogging = false) {
    const char* delimiterStr = method_name.find('(') == string::npos ? "(" : " ";
    fprintf(out, "     * Usage: %s%s%s", method_name.c_str(), delimiterStr, atom_code_name.c_str());

    for (vector<AtomField>::const_iterator field = atom.fields.begin(); field != atom.fields.end();
         field++) {
        if (field->javaType == JAVA_TYPE_ATTRIBUTION_CHAIN) {
            for (const auto& chainField : attributionDecl.fields) {
                if (chainField.javaType == JAVA_TYPE_STRING) {
                    fprintf(out, ", const std::vector<%s>& %s", cpp_type_name(chainField.javaType),
                            chainField.name.c_str());
                } else {
                    fprintf(out, ", const %s* %s, size_t %s_length",
                            cpp_type_name(chainField.javaType), chainField.name.c_str(),
                            chainField.name.c_str());
                }
            }
        } else {
            fprintf(out, ", %s %s", cpp_type_name(field->javaType, isVendorAtomLogging),
                    field->name.c_str());
        }
    }
    fprintf(out, ");\n");
}

void write_native_atom_constants(FILE* out, const Atoms& atoms, const AtomDecl& attributionDecl,
                                 const string& methodName, bool isVendorAtomLogging) {
    fprintf(out, "/**\n");
    fprintf(out, " * Constants for atom codes.\n");
    fprintf(out, " */\n");
    fprintf(out, "enum {\n");

    std::map<int, AtomDeclSet::const_iterator> atom_code_to_non_chained_decl_map;
    build_non_chained_decl_map(atoms, &atom_code_to_non_chained_decl_map);

    size_t i = 0;
    // Print atom constants
    for (AtomDeclSet::const_iterator atomIt = atoms.decls.begin(); atomIt != atoms.decls.end();
         atomIt++) {
        const string constant = make_constant_name((*atomIt)->name);
        fprintf(out, "\n");
        fprintf(out, "    /**\n");
        fprintf(out, "     * %s %s\n", (*atomIt)->message.c_str(), (*atomIt)->name.c_str());
        write_cpp_usage(out, methodName, constant, **atomIt, attributionDecl, isVendorAtomLogging);

        auto non_chained_decl = atom_code_to_non_chained_decl_map.find((*atomIt)->code);
        if (non_chained_decl != atom_code_to_non_chained_decl_map.end()) {
            write_cpp_usage(out, methodName + "_non_chained", constant, **non_chained_decl->second,
                            attributionDecl, isVendorAtomLogging);
        }
        fprintf(out, "     */\n");
        char const* const comma = (i == atoms.decls.size() - 1) ? "" : ",";
        fprintf(out, "    %s = %d%s\n", constant.c_str(), (*atomIt)->code, comma);
        i++;
    }
    fprintf(out, "\n");
    fprintf(out, "};\n");
    fprintf(out, "\n");
}

void write_native_atom_enums(FILE* out, const Atoms& atoms) {
    // Print constants for the enum values.
    fprintf(out, "//\n");
    fprintf(out, "// Constants for enum values\n");
    fprintf(out, "//\n\n");
    for (AtomDeclSet::const_iterator atomIt = atoms.decls.begin(); atomIt != atoms.decls.end();
         atomIt++) {
        for (vector<AtomField>::const_iterator field = (*atomIt)->fields.begin();
             field != (*atomIt)->fields.end(); field++) {
            if (field->javaType == JAVA_TYPE_ENUM || field->javaType == JAVA_TYPE_ENUM_ARRAY) {
                fprintf(out, "// Values for %s.%s\n", (*atomIt)->message.c_str(),
                        field->name.c_str());
                for (map<int, string>::const_iterator value = field->enumValues.begin();
                     value != field->enumValues.end(); value++) {
                    fprintf(out, "const int32_t %s__%s__%s = %d;\n",
                            make_constant_name((*atomIt)->message).c_str(),
                            make_constant_name(field->name).c_str(),
                            make_constant_name(value->second).c_str(), value->first);
                }
                fprintf(out, "\n");
            }
        }
    }
}

int write_native_atom_types(FILE* out, const Atoms& atoms, const char* pushedApiName) {
    for (auto& atomDecl : atoms.decls) {
        if (atomDecl->atomType == ATOM_TYPE_PUSHED) {
            fprintf(out, "// Usage: %s(const %s& atom);\n", pushedApiName,
                    atomDecl->message.c_str());
        } else {
            fprintf(out,
                    "// Usage: addAStatsEvent(AStatsEventList* pulled_data, const %s& atom);\n",
                    atomDecl->message.c_str());
        }

        fprintf(out, "struct %s final {\n", atomDecl->message.c_str());

        // write enum definitions
        if (write_native_atom_enums_typesafe(out, *atomDecl,
                                             /*useScopedEnums=*/true) != 0) {
            return 1;
        }

        // write fields
        for (auto& field : atomDecl->fields) {
            if (field.javaType == JAVA_TYPE_ATTRIBUTION_CHAIN) {
                fprintf(out, "  std::vector<AttributionNode> %s;\n", field.name.c_str());
            } else {
                fprintf(out, "  %s %s;\n", to_cpp_typesafe_name(field).c_str(), field.name.c_str());
            }
        }

        fprintf(out, "};\n\n");
    }
    return 0;
}

int write_native_atom_enums_typesafe(FILE* out, const AtomDecl& atomFields, bool useScopedEnums) {
    // maps proto enumType to its full type name
    map<string, string> processedEnums;
    for (auto& field : get_enum_fields(atomFields)) {
        // There might be N fields with the same enum type
        // avoid duplication definitions
        if (processedEnums.find(field.enumTypeName) != processedEnums.end()) {
            // test if fully qualified name is also the same
            if (processedEnums[field.enumTypeName] != field.enumTypeNameFull) {
                fprintf(stderr, "Duplicated enum type name detected %s vs %s\n",
                        processedEnums[field.enumTypeName].c_str(), field.enumTypeNameFull.c_str());
                fprintf(stderr,
                        "Unsupported fully qualified type names generation. "
                        "Vote up for http://b/460830603\n");
                return 1;
            }
            continue;
        }

        processedEnums.insert(std::make_pair(field.enumTypeName, field.enumTypeNameFull));

        const char* scopedEnumSpecifier = useScopedEnums ? "class " : "";

        fprintf(out, "  enum %s%s {\n", scopedEnumSpecifier, field.enumTypeName.c_str());
        size_t i = 0;
        for (map<int, string>::const_iterator value = field.enumValues.begin();
             value != field.enumValues.end(); value++) {
            fprintf(out, "    %s = %d", make_constant_name(value->second).c_str(), value->first);
            char const* const comma = (i == field.enumValues.size() - 1) ? "" : ",";
            fprintf(out, "%s\n", comma);
            i++;
        }

        fprintf(out, "  };\n\n");
    }
    return 0;
}

void write_native_method_signature(FILE* out, const string& signaturePrefix,
                                   const vector<java_type_t>& signature,
                                   const AtomDecl& attributionDecl, const string& closer,
                                   bool isVendorAtomLogging) {
    fprintf(out, "%sint32_t code", signaturePrefix.c_str());
    int argIndex = 1;
    for (vector<java_type_t>::const_iterator arg = signature.begin(); arg != signature.end();
         arg++) {
        if (*arg == JAVA_TYPE_ATTRIBUTION_CHAIN) {
            for (const auto& chainField : attributionDecl.fields) {
                if (chainField.javaType == JAVA_TYPE_STRING) {
                    fprintf(out, ", const std::vector<%s>& %s",
                            cpp_type_name(chainField.javaType, isVendorAtomLogging),
                            chainField.name.c_str());
                } else {
                    fprintf(out, ", const %s* %s, size_t %s_length",
                            cpp_type_name(chainField.javaType, isVendorAtomLogging),
                            chainField.name.c_str(), chainField.name.c_str());
                }
            }
        } else {
            fprintf(out, ", %s arg%d", cpp_type_name(*arg, isVendorAtomLogging), argIndex);

            if (*arg == JAVA_TYPE_BOOLEAN_ARRAY && !isVendorAtomLogging) {
                fprintf(out, ", size_t arg%d_length", argIndex);
            }
        }
        argIndex++;
    }
    fprintf(out, ")%s\n", closer.c_str());
}

void write_native_method_header(FILE* out, const string& methodName,
                                const SignatureInfoMap& signatureInfoMap,
                                const AtomDecl& attributionDecl, bool isVendorAtomLogging) {
    for (const auto& [signature, _] : signatureInfoMap) {
        string closer =
                contains_repeated_field(signature) ? "\n__INTRODUCED_IN(__ANDROID_API_T__);" : ";";
        write_native_method_signature(out, methodName, signature, attributionDecl, closer,
                                      isVendorAtomLogging);
    }
}

void write_native_header_preamble(FILE* out, const Atoms& atoms, const string& cppNamespace,
                                  InterfaceApi interfaceApi, bool includeExtraSrcs) {
    const bool includePull =
            !atoms.pulledAtomsSignatureInfoMap.empty() && interfaceApi != InterfaceApi::BOOTSTRAP;
    const bool includeHistogram = has_histograms(atoms.decls);

    // Print prelude
    fprintf(out, "// This file is autogenerated\n");
    fprintf(out, "\n");
    fprintf(out, "#pragma once\n");
    fprintf(out, "\n");

    if (interfaceApi != InterfaceApi::VENDOR) {
        fprintf(out, "#include <stdint.h>\n");
        fprintf(out, "#include <vector>\n");
        fprintf(out, "#include <map>\n");
        fprintf(out, "#include <set>\n");
        fprintf(out, "#include <memory>\n");
        if (includePull) {
            fprintf(out, "#include <stats_pull_atom_callback.h>\n");
        }
    }

    switch (interfaceApi) {
        case InterfaceApi::PLATFORM:
            fprintf(out, "#include <stddef.h>\n");
            fprintf(out, "\n");
            fprintf(out, "#ifndef __ANDROID_API_T__\n");
            fprintf(out, "#define __ANDROID_API_T__ 33\n");
            fprintf(out, "#endif\n");
            fprintf(out, "#ifndef __INTRODUCED_IN\n");
            fprintf(out, "#define __INTRODUCED_IN(api_level)\n");
            fprintf(out, "#endif\n");
            break;
        case InterfaceApi::VENDOR:
            fprintf(out, "#include <aidl/android/frameworks/stats/VendorAtom.h>\n");
            break;
        default:
            break;
    }

    if (includeExtraSrcs) {  // Inline headers from CC_INCLUDE_HDRS_DIR like StatsHistogram.h
#ifdef CC_INCLUDE_HDRS_DIR
        const vector<string> excludeList =
                includeHistogram ? vector<string>{} : vector<string>{HISTOGRAM_STEM};
        write_srcs_header(out, CC_INCLUDE_HDRS_DIR, excludeList);
#endif
    } else if (includeHistogram) {  // StatsHistogram is linked via a lib.
        fprintf(out, "#include <StatsHistogram.h>\n");
    }

    fprintf(out, "\n");

    write_namespace(out, cppNamespace);
    fprintf(out, "\n");
    fprintf(out, "/*\n");
    fprintf(out, " * API For logging statistics events.\n");
    fprintf(out, " */\n");

    write_header_histogram_sources(out, atoms, includeExtraSrcs);
}

void write_native_header_epilogue(FILE* out, const string& cppNamespace) {
    write_closing_namespace(out, cppNamespace);
}

int write_native_source_preamble(FILE* out, const Atoms& atoms, const string& importHeader,
                                 const int minApiLevel, const string& cppNamespace,
                                 InterfaceApi interfaceApi, bool includeExtraSrcs) {
    // Print prelude
    fprintf(out, "// This file is autogenerated\n");
    fprintf(out, "\n");

    fprintf(out, "#include <%s>\n", importHeader.c_str());
    switch (interfaceApi) {
        case InterfaceApi::PLATFORM:
            if (minApiLevel == API_Q) {
                fprintf(out, "#include <StatsEventCompat.h>\n");
            } else {
                fprintf(out, "#include <stats_event.h>\n");
            }

            if (minApiLevel > API_R) {
                fprintf(out, "#include <stats_annotations.h>\n");
            }

            if (minApiLevel > API_Q && !atoms.pulledAtomsSignatureInfoMap.empty()) {
                fprintf(out, "#include <stats_pull_atom_callback.h>\n");
            }
            break;
        case InterfaceApi::BOOTSTRAP:
            fprintf(out, "#include <StatsBootstrapAtomClient.h>\n");
            fprintf(out, "#include <android/os/StatsBootstrapAtom.h>\n");
            fprintf(out, "#include <utils/String16.h>\n");
            break;
        case InterfaceApi::VENDOR:
            fprintf(out, "#include <aidl/android/frameworks/stats/VendorAtom.h>\n\n");

            fprintf(out, "using namespace aidl::android::frameworks::stats;\n");
            fprintf(out, "using std::make_optional;\n");
            fprintf(out, "using std::optional;\n");
            fprintf(out, "using std::vector;\n");
            fprintf(out, "using std::string;\n\n");

            break;
        default:
            fprintf(stderr, "Encountered unsupported InterfaceApi (%d)",
                    static_cast<int>(interfaceApi));
            return 1;
    }

    return write_cpp_sources(out, atoms, cppNamespace, includeExtraSrcs);
}

// Java
void write_java_atom_codes(FILE* out, const Atoms& atoms, const bool supportWorkSource) {
    fprintf(out, "    // Constants for atom codes.\n");

    std::map<int, AtomDeclSet::const_iterator> atom_code_to_non_chained_decl_map;
    build_non_chained_decl_map(atoms, &atom_code_to_non_chained_decl_map);

    // Print constants for the atom codes.
    for (AtomDeclSet::const_iterator atomIt = atoms.decls.begin(); atomIt != atoms.decls.end();
         atomIt++) {
        const string constant = make_constant_name((*atomIt)->name);
        fprintf(out, "\n");
        fprintf(out, "    /**\n");
        fprintf(out, "     * %s %s<br>\n", (*atomIt)->message.c_str(), (*atomIt)->name.c_str());
        write_java_usage(out, "write", constant, **atomIt, /*supportWorkSource=*/false);
        if (supportWorkSource) {
            write_java_usage(out, "write", constant, **atomIt,
                             /*supportWorkSource=*/true);
        }
        auto non_chained_decl = atom_code_to_non_chained_decl_map.find((*atomIt)->code);
        if (non_chained_decl != atom_code_to_non_chained_decl_map.end()) {
            write_java_usage(out, "write_non_chained", constant, **(non_chained_decl->second),
                             /*supportWorkSource=*/false);
        }
        fprintf(out, "     */\n");
        fprintf(out, "    public static final int %s = %d;\n", constant.c_str(), (*atomIt)->code);
    }
    fprintf(out, "\n");
}

void write_java_enum_values(FILE* out, const Atoms& atoms) {
    fprintf(out, "    // Constants for enum values.\n\n");
    for (AtomDeclSet::const_iterator atomIt = atoms.decls.begin(); atomIt != atoms.decls.end();
         atomIt++) {
        for (vector<AtomField>::const_iterator field = (*atomIt)->fields.begin();
             field != (*atomIt)->fields.end(); field++) {
            if (field->javaType == JAVA_TYPE_ENUM || field->javaType == JAVA_TYPE_ENUM_ARRAY) {
                fprintf(out, "    // Values for %s.%s\n", (*atomIt)->message.c_str(),
                        field->name.c_str());
                for (map<int, string>::const_iterator value = field->enumValues.begin();
                     value != field->enumValues.end(); value++) {
                    fprintf(out, "    public static final int %s__%s__%s = %d;\n",
                            make_constant_name((*atomIt)->message).c_str(),
                            make_constant_name(field->name).c_str(),
                            make_constant_name(value->second).c_str(), value->first);
                }
                fprintf(out, "\n");
            }
        }
    }
}

int write_java_method_signature(FILE* out, const vector<java_type_t>& signature,
                                const AtomDecl& attributionDecl) {
    int argIndex = 1;
    for (vector<java_type_t>::const_iterator arg = signature.begin(); arg != signature.end();
         arg++) {
        if (*arg == JAVA_TYPE_ATTRIBUTION_CHAIN) {
            if (attributionDecl.fields.empty()) {
                fprintf(stderr, "Encountered incompatible attribution chain atom definition");
                return 1;
            }
            for (const auto& chainField : attributionDecl.fields) {
                fprintf(out, ", %s[] %s", java_type_name(chainField.javaType),
                        chainField.name.c_str());
            }
        } else {
            fprintf(out, ", %s arg%d", java_type_name(*arg), argIndex);
        }
        argIndex++;
    }
    return 0;
}

void write_java_usage(FILE* out, const string& method_name, const string& atom_code_name,
                      const AtomDecl& atom, const bool supportWorkSource) {
    fprintf(out, "     * Usage: StatsLog.%s(StatsLog.%s", method_name.c_str(),
            atom_code_name.c_str());
    for (vector<AtomField>::const_iterator field = atom.fields.begin(); field != atom.fields.end();
         field++) {
        if (field->javaType == JAVA_TYPE_ATTRIBUTION_CHAIN) {
            if (supportWorkSource) {
                fprintf(out, ", android.os.WorkSource workSource");
            } else {
                fprintf(out, ", int[] uid, java.lang.String[] tag");
            }
        } else if (field->javaType == JAVA_TYPE_BYTE_ARRAY) {
            fprintf(out, ", byte[] %s", field->name.c_str());
        } else {
            fprintf(out, ", %s %s", java_type_name(field->javaType), field->name.c_str());
        }
    }
    fprintf(out, ");<br>\n");
}

int write_java_non_chained_methods(FILE* out, const SignatureInfoMap& signatureInfoMap,
                                   const bool staticMethods) {
    const char* methodPrefix = staticMethods ? "static " : "";
    for (auto signatureInfoMapIt = signatureInfoMap.begin();
         signatureInfoMapIt != signatureInfoMap.end(); signatureInfoMapIt++) {
        // Print method signature.
        fprintf(out, "    public %svoid write_non_chained(int code", methodPrefix);
        vector<java_type_t> signature = signatureInfoMapIt->first;
        int argIndex = 1;
        for (vector<java_type_t>::const_iterator arg = signature.begin(); arg != signature.end();
             arg++) {
            if (*arg == JAVA_TYPE_ATTRIBUTION_CHAIN) {
                fprintf(stderr, "Non chained signatures should not have attribution chains.\n");
                return 1;
            } else {
                fprintf(out, ", %s arg%d", java_type_name(*arg), argIndex);
            }
            argIndex++;
        }
        fprintf(out, ") {\n");

        fprintf(out, "        write(code");
        argIndex = 1;
        for (vector<java_type_t>::const_iterator arg = signature.begin(); arg != signature.end();
             arg++) {
            // First two args are uid and tag of attribution chain.
            if (argIndex == 1) {
                fprintf(out, ", new int[] {arg%d}", argIndex);
            } else if (argIndex == 2) {
                fprintf(out, ", new java.lang.String[] {arg%d}", argIndex);
            } else {
                fprintf(out, ", arg%d", argIndex);
            }
            argIndex++;
        }
        fprintf(out, ");\n");
        fprintf(out, "    }\n");
        fprintf(out, "\n");
    }
    return 0;
}

int write_java_work_source_methods(FILE* out, const SignatureInfoMap& signatureInfoMap) {
    fprintf(out, "    // WorkSource methods.\n");
    for (auto signatureInfoMapIt = signatureInfoMap.begin();
         signatureInfoMapIt != signatureInfoMap.end(); signatureInfoMapIt++) {
        vector<java_type_t> signature = signatureInfoMapIt->first;
        // Determine if there is Attribution in this signature.
        int attributionArg = -1;
        int argIndexMax = 0;
        for (vector<java_type_t>::const_iterator arg = signature.begin(); arg != signature.end();
             arg++) {
            argIndexMax++;
            if (*arg == JAVA_TYPE_ATTRIBUTION_CHAIN) {
                if (attributionArg > -1) {
                    fprintf(stderr, "An atom contains multiple AttributionNode fields.\n");
                    fprintf(stderr, "This is not supported. Aborting WorkSource method writing.\n");
                    fprintf(out,
                            "\n// Invalid for WorkSource: more than one attribution "
                            "chain.\n");
                    return 1;
                }
                attributionArg = argIndexMax;
            }
        }
        if (attributionArg < 0) {
            continue;
        }

        fprintf(out, "\n");
        // Method header (signature)
        fprintf(out, "    public static void write(int code");
        int argIndex = 1;
        for (vector<java_type_t>::const_iterator arg = signature.begin(); arg != signature.end();
             arg++) {
            if (*arg == JAVA_TYPE_ATTRIBUTION_CHAIN) {
                fprintf(out, ", android.os.WorkSource ws");
            } else {
                fprintf(out, ", %s arg%d", java_type_name(*arg), argIndex);
            }
            argIndex++;
        }
        fprintf(out, ") {\n");

        // write_non_chained() component. TODO: Remove when flat uids are no longer
        // needed.
        fprintf(out, "        for (int i = 0; i < ws.size(); ++i) {\n");
        fprintf(out, "            write_non_chained(code");
        for (int argIndex = 1; argIndex <= argIndexMax; argIndex++) {
            if (argIndex == attributionArg) {
                fprintf(out, ", ws.getUid(i), ws.getPackageName(i)");
            } else {
                fprintf(out, ", arg%d", argIndex);
            }
        }
        fprintf(out, ");\n");
        fprintf(out, "        }\n");  // close for-loop

        // write() component.
        fprintf(out,
                "        java.util.List<android.os.WorkSource.WorkChain> workChains = "
                "ws.getWorkChains();\n");
        fprintf(out, "        if (workChains != null) {\n");
        fprintf(out,
                "            for (android.os.WorkSource.WorkChain wc : workChains) "
                "{\n");
        fprintf(out, "                write(code");
        for (int argIndex = 1; argIndex <= argIndexMax; argIndex++) {
            if (argIndex == attributionArg) {
                fprintf(out, ", wc.getUids(), wc.getTags()");
            } else {
                fprintf(out, ", arg%d", argIndex);
            }
        }
        fprintf(out, ");\n");
        fprintf(out, "            }\n");  // close for-loop
        fprintf(out, "        }\n");      // close if
        fprintf(out, "    }\n");          // close method
    }
    return 0;
}

static bool contains_restricted(const AtomDeclSet& atomDeclSet) {
    for (const auto& decl : atomDeclSet) {
        if (decl->restricted) {
            return true;
        }
    }
    return false;
}

int get_max_requires_api_level(int minApiLevel, const AtomDeclSet* atomDeclSet,
                               const vector<java_type_t>& signature) {
    if (atomDeclSet != nullptr && contains_restricted(*atomDeclSet)) {
        return API_U;
    }
    if (contains_repeated_field(signature)) {
        return API_T;
    }
    if (minApiLevel <= API_Q) {
        return API_Q;  // for StatsLog.writeRaw()
    }
    return 0;
}

bool has_histograms(const AtomDeclSet& decls) {
    return std::find_if_not(decls.begin(), decls.end(), [](const shared_ptr<AtomDecl>& decl) {
               return decl->fieldNameToHistBinOption.empty();
           }) != decls.end();
}

bool has_attribution_node(const AtomDeclSet& decls) {
    return std::any_of(decls.cbegin(), decls.cend(), [](const shared_ptr<AtomDecl>& decl) {
        return std::any_of(decl->fields.cbegin(), decl->fields.cend(), [](const AtomField& field) {
            return field.javaType == JAVA_TYPE_ATTRIBUTION_CHAIN;
        });
    });
}

void write_native_histogram_helper_declarations(FILE* out, const AtomDeclSet& atomDeclSet) {
    for (const shared_ptr<AtomDecl>& atomDecl : atomDeclSet) {
        for (const auto& [fieldName, histBinOption] : atomDecl->fieldNameToHistBinOption) {
            write_native_histogram_helper_signature(out, atomDecl->name, fieldName);
            fprintf(out, ";\n");
        }
    }
    fprintf(out, "\n");
}

int write_native_histogram_helper_definitions(FILE* out, const AtomDeclSet& atomDeclSet) {
    int errors = 0;
    for (const shared_ptr<AtomDecl>& atomDecl : atomDeclSet) {
        for (const auto& [fieldName, histBinOption] : atomDecl->fieldNameToHistBinOption) {
            errors += write_native_histogram_helper_definition(out, atomDecl->name, fieldName,
                                                               histBinOption);
        }
    }
    return errors;
}

int write_srcs_header(FILE* out, const char* path, const vector<string>& excludeList) {
    int errors = 0;
    const string fullPath = get_data_dir_path(path);
    for (const fs::path& filePath : fs::directory_iterator(fullPath)) {
        // Add headers from filePath if it's not in excludeList.
        if (std::find(excludeList.begin(), excludeList.end(), filePath.stem()) ==
            excludeList.end()) {
            errors += write_src_header(out, filePath);
        }
    }

    return errors;
}

int write_java_srcs_classes(FILE* out, const char* path, const vector<string>& excludeList) {
    return write_srcs_bodies(out, path, 4 /* indent */, excludeList, make_java_class_static);
}

int write_cc_srcs_classes(FILE* out, const char* path, const vector<string>& excludeList) {
    return write_srcs_bodies(out, path, 0 /* indent */, excludeList, nullptr /* nameTransformer */);
}

int write_java_histogram_helpers(FILE* out, const AtomDeclSet& atomDeclSet,
                                 const bool staticMethods) {
    int errors = 0;
    for (const shared_ptr<AtomDecl>& atomDecl : atomDeclSet) {
        for (const auto& [fieldName, histBinOption] : atomDecl->fieldNameToHistBinOption) {
            errors += write_java_histogram_helper(out, atomDecl->name, fieldName, histBinOption,
                                                  staticMethods);
        }
    }
    return errors;
}

string replace_all(string str, const string& from, const string& to) {
    size_t start = 0;
    while ((start = str.find(from, start)) != string::npos) {
        str.replace(start, from.length(), to);
        start += to.length();
    }
    return str;
}

}  // namespace stats_log_api_gen
}  // namespace android
