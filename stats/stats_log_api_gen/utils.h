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

#ifndef ANDROID_STATS_LOG_API_GEN_UTILS_H
#define ANDROID_STATS_LOG_API_GEN_UTILS_H

#include <google/protobuf/compiler/importer.h>
#include <stdio.h>
#include <string.h>

#include <map>
#include <set>
#include <vector>

#include "Collation.h"

namespace android {
namespace stats_log_api_gen {

const char DEFAULT_CPP_NAMESPACE[] = "android,util";
const char DEFAULT_CPP_HEADER_IMPORT[] = "statslog.h";

const int API_LEVEL_CURRENT = 10000;
const int API_Q = 29;
const int API_R = 30;
const int API_S = 31;
const int API_S_V2 = 32;
const int API_T = 33;
const int API_U = 34;

const int JAVA_MODULE_REQUIRES_FLOAT = 0x01;
const int JAVA_MODULE_REQUIRES_ATTRIBUTION = 0x02;

const char ANNOTATION_CONSTANT_NAME_PREFIX[] = "ANNOTATION_ID_";
const char ANNOTATION_CONSTANT_NAME_VENDOR_PREFIX[] = "AnnotationId.";
const char ANNOTATION_CONSTANT_NAME_VENDOR_NATIVE_PREFIX[] = "AnnotationId::";
const string HISTOGRAM_STEM("StatsHistogram");

struct AnnotationStruct {
    string name;
    int minApiLevel;
    AnnotationStruct(string name, int minApiLevel)
        : name(std::move(name)), minApiLevel(minApiLevel) {};
};

void build_non_chained_decl_map(const Atoms& atoms,
                                std::map<int, AtomDeclSet::const_iterator>* decl_map);

const map<AnnotationId, AnnotationStruct>& get_annotation_id_constants(const string& prefix);

string get_java_build_version_code(int apiLevel);

string get_restriction_category_str(int annotationValue);

string make_constant_name(const string& str);

string snake_to_pascal(const string& snake);

string to_cpp_typesafe_name(const AtomField& field);

const char* cpp_type_name(java_type_t type, bool isVendorAtomLogging = false);

const char* java_type_name(java_type_t type);

bool is_repeated_field(java_type_t type);

bool is_primitive_field(java_type_t type);

AtomDeclSet get_annotations(int argIndex, const FieldNumberToAtomDeclSet& fieldNumberToAtomDeclSet);

vector<AtomField> get_enum_fields(const AtomDecl& atomDecl);

// Common Native helpers
void write_namespace(FILE* out, const string& cppNamespaces);

void write_closing_namespace(FILE* out, const string& cppNamespaces);

void write_native_atom_constants(FILE* out, const Atoms& atoms, const AtomDecl& attributionDecl,
                                 const string& methodName = "stats_write",
                                 bool isVendorAtomLogging = false);

void write_native_atom_enums(FILE* out, const Atoms& atoms);

int write_native_atom_enums_typesafe(FILE* out, const AtomDecl& atomFields, bool useScopedEnums);

int write_native_atom_types(FILE* out, const Atoms& atoms);

void write_native_method_signature(FILE* out, const string& signaturePrefix,
                                   const vector<java_type_t>& signature,
                                   const AtomDecl& attributionDecl, const string& closer,
                                   bool isVendorAtomLogging = false);

void write_native_method_header(FILE* out, const string& methodName,
                                const SignatureInfoMap& signatureInfoMap,
                                const AtomDecl& attributionDecl, bool isVendorAtomLogging = false);

void write_native_header_preamble(FILE* out, const Atoms& atoms, const string& cppNamespace,
                                  bool bootstrap, bool isVendorAtomLogging = false);

void write_native_header_epilogue(FILE* out, const string& cppNamespace);

// Common Java helpers.
void write_java_atom_codes(FILE* out, const Atoms& atoms, const bool supportWorkSource);

void write_java_enum_values(FILE* out, const Atoms& atoms);

int write_java_method_signature(FILE* out, const vector<java_type_t>& signature,
                                const AtomDecl& attributionDecl);

void write_java_usage(FILE* out, const string& method_name, const string& atom_code_name,
                      const AtomDecl& atom, const bool supportWorkSource);

int write_java_non_chained_methods(FILE* out, const SignatureInfoMap& signatureInfoMap,
                                   const bool staticMethods);

int write_java_work_source_methods(FILE* out, const SignatureInfoMap& signatureInfoMap);

class MFErrorCollector : public google::protobuf::compiler::MultiFileErrorCollector {
public:
    void RecordError(absl::string_view filename, int line, int column,
                     absl::string_view message) override {
        fprintf(stderr, "[Error] %.*s:%d:%d - %.*s\n", static_cast<int>(filename.size()),
                filename.data(), line, column, static_cast<int>(message.size()), message.data());
    }
};

int get_max_requires_api_level(int minApiLevel, const AtomDeclSet* atomDeclSet,
                               const vector<java_type_t>& signature);

bool has_histograms(const AtomDeclSet& decls);

bool has_attribution_node(const AtomDeclSet& decls);

void write_native_histogram_helper_declarations(FILE* out, const AtomDeclSet& atomDeclSet);

int write_native_histogram_helper_definitions(FILE* out, const AtomDeclSet& atomDeclSet);

int write_srcs_header(FILE* out, const char* path, const std::vector<std::string>& excludeList);

int write_java_srcs_classes(FILE* out, const char* path,
                            const std::vector<std::string>& excludeList);

int write_cc_srcs_classes(FILE* out, const char* path, const std::vector<std::string>& excludeList);

int write_java_histogram_helpers(FILE* out, const AtomDeclSet& atomDeclSet,
                                 const bool staticMethods);

bool contains_repeated_field(const vector<AtomField>& atomFields);

}  // namespace stats_log_api_gen
}  // namespace android

#endif  // ANDROID_STATS_LOG_API_GEN_UTILS_H
