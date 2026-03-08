/*
 * Copyright (C) 2023, The Android Open Source Project
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

#include "native_writer_vendor.h"

#include <stdio.h>

#include <string>
#include <vector>

#include "Collation.h"
#include "utils.h"

namespace android {
namespace stats_log_api_gen {

using std::string;

static void write_native_vendor_annotation_header(FILE* out, const string& annotationName,
                                                  const char* indent) {
    fprintf(out, "%s{\n", indent);
    fprintf(out, "%s    Annotation annotation;\n", indent);
    fprintf(out, "%s    annotation.annotationId = %s;\n", indent, annotationName.c_str());
}

static void write_native_vendor_annotation_footer(FILE* out, const char* indent) {
    fprintf(out, "%s    annotations.push_back(std::move(annotation));\n", indent);
    fprintf(out, "%s}\n", indent);
}

static void write_native_vendor_annotation_int(FILE* out, const string& annotationName, int value,
                                               const char* indent) {
    write_native_vendor_annotation_header(out, annotationName, indent);
    fprintf(out, "%s    annotation.value.set<AnnotationValue::intValue>(%d);\n", indent, value);
    write_native_vendor_annotation_footer(out, indent);
}

static void write_native_vendor_annotation_int_constant(FILE* out, const string& annotationName,
                                                        const string& constantName,
                                                        const char* indent) {
    write_native_vendor_annotation_header(out, annotationName, indent);
    fprintf(out, "%s    annotation.value.set<AnnotationValue::intValue>(%s);\n", indent,
            constantName.c_str());
    write_native_vendor_annotation_footer(out, indent);
}

static void write_native_vendor_annotation_bool(FILE* out, const string& annotationName, bool value,
                                                const char* indent) {
    write_native_vendor_annotation_header(out, annotationName, indent);
    fprintf(out, "%s    annotation.value.set<AnnotationValue::boolValue>(%s);\n", indent,
            value ? "true" : "false");
    write_native_vendor_annotation_footer(out, indent);
}

/**
 * Writes field annotations
 */
static void write_field_annotations(FILE* out, const AnnotationSet& annotations,
                                    const char* fieldName, const char* indent) {
    static const map<AnnotationId, AnnotationStruct>& ANNOTATION_ID_CONSTANTS =
            get_annotation_id_constants(ANNOTATION_CONSTANT_NAME_VENDOR_NATIVE_PREFIX);

    int resetState = -1;
    int defaultState = -1;

    for (const shared_ptr<Annotation>& annotation : annotations) {
        const AnnotationStruct& annotationConstant =
                ANNOTATION_ID_CONSTANTS.at(annotation->annotationId);
        switch (annotation->type) {
            case ANNOTATION_TYPE_INT:
                switch (annotation->annotationId) {
                    case ANNOTATION_ID_TRIGGER_STATE_RESET:
                        resetState = annotation->value.intValue;
                        break;
                    case ANNOTATION_ID_DEFAULT_STATE:
                        defaultState = annotation->value.intValue;
                        break;
                    case ANNOTATION_ID_RESTRICTION_CATEGORY:
                        write_native_vendor_annotation_int_constant(
                                out, annotationConstant.name,
                                get_restriction_category_str(annotation->value.intValue), indent);
                        break;
                    default:
                        write_native_vendor_annotation_int(out, annotationConstant.name,
                                                           annotation->value.intValue, indent);
                        break;
                }
                break;
            case ANNOTATION_TYPE_BOOL:
                write_native_vendor_annotation_bool(out, annotationConstant.name,
                                                    annotation->value.boolValue, indent);
                break;
            default:
                break;
        }
    }

    if (defaultState != -1 && resetState != -1) {
        const string nextIndentString = string(indent) + "    ";
        const char* nextIndent = nextIndentString.c_str();
        const AnnotationStruct& annotationConstant =
                ANNOTATION_ID_CONSTANTS.at(ANNOTATION_ID_TRIGGER_STATE_RESET);
        fprintf(out, "%sif (static_cast<int32_t>(%s) == %d) {\n", indent, fieldName, resetState);
        write_native_vendor_annotation_int(out, annotationConstant.name, defaultState, nextIndent);
        fprintf(out, "%s}\n", indent);
    }
}

static void write_native_annotations_vendor_for_field(FILE* out, int fieldIndex,
                                                      const AtomDeclSet& atomDeclSet) {
    if (atomDeclSet.empty()) {
        return;
    }

    const char* indent = "    ";
    const char* indent2 = "        ";
    const char* indent3 = "            ";

    const int valueIndex = fieldIndex - 2;

    for (const shared_ptr<AtomDecl>& atomDecl : atomDeclSet) {
        const string atomConstant = make_constant_name(atomDecl->name);
        fprintf(out, "%sif (%s == code) {\n", indent, atomConstant.c_str());

        if (fieldIndex == ATOM_ID_FIELD_NUMBER) {
            fprintf(out, "%sstd::vector<std::optional<Annotation>> annotations;\n", indent2);
        } else {
            fprintf(out, "%sstd::vector<Annotation> annotations;\n", indent2);
        }

        const string fieldName = string("arg") + std::to_string(fieldIndex);
        const AnnotationSet& annotations = atomDecl->fieldNumberToAnnotations.at(fieldIndex);
        write_field_annotations(out, annotations, fieldName.c_str(), indent2);

        if (fieldIndex == ATOM_ID_FIELD_NUMBER) {
            fprintf(out, "%satomAnnotations = std::move(annotations);\n", indent2);
        } else {
            fprintf(out, "%sif (annotations.size() > 0) {\n", indent2);
            fprintf(out, "%sAnnotationSet field%dAnnotations;\n", indent3, valueIndex);
            fprintf(out, "%sfield%dAnnotations.valueIndex = %d;\n", indent3, valueIndex,
                    valueIndex);
            fprintf(out, "%sfield%dAnnotations.annotations = std::move(annotations);\n", indent3,
                    valueIndex);
            fprintf(out, "%sfieldsAnnotations.push_back(std::move(field%dAnnotations));\n", indent3,
                    valueIndex);
            fprintf(out, "%s}\n", indent2);
        }
        fprintf(out, "%s}\n", indent);
    }
}

static int write_native_create_vendor_atom_methods(FILE* out,
                                                   const SignatureInfoMap& signatureInfoMap,
                                                   const AtomDecl& attributionDecl) {
    fprintf(out, "\n");
    for (const auto& [signature, fieldNumberToAtomDeclSet] : signatureInfoMap) {
        // TODO (b/264922532): provide vendor implementation to skip arg1 for reverseDomainName
        write_native_method_signature(out, "VendorAtom createVendorAtom(", signature,
                                      attributionDecl, " {", /*isVendorAtomLogging=*/true);

        fprintf(out, "    VendorAtom atom;\n");

        // Write method body.
        fprintf(out, "    atom.atomId = code;\n");
        fprintf(out, "    atom.reverseDomainName = arg1;\n");

        // Exclude first field - which is reverseDomainName
        const int vendorAtomValuesCount = signature.size() - 1;
        fprintf(out, "    vector<VendorAtomValue> values(%d);\n", vendorAtomValuesCount);

        // Use 1-based index to access signature arguments
        for (int argIndex = 2; argIndex <= signature.size(); argIndex++) {
            const java_type_t& argType = signature[argIndex - 1];

            const int atomValueIndex = argIndex - 2;

            switch (argType) {
                case JAVA_TYPE_BYTE_ARRAY:
                    fprintf(out,
                            "    "
                            "values[%d].set<VendorAtomValue::byteArrayValue>(arg%d);\n",
                            atomValueIndex, argIndex);
                    break;
                case JAVA_TYPE_BOOLEAN:
                    fprintf(out, "    values[%d].set<VendorAtomValue::boolValue>(arg%d);\n",
                            atomValueIndex, argIndex);
                    break;
                case JAVA_TYPE_INT:
                    [[fallthrough]];
                case JAVA_TYPE_ENUM:
                    fprintf(out, "    values[%d].set<VendorAtomValue::intValue>(arg%d);\n",
                            atomValueIndex, argIndex);
                    break;
                case JAVA_TYPE_FLOAT:
                    fprintf(out, "    values[%d].set<VendorAtomValue::floatValue>(arg%d);\n",
                            atomValueIndex, argIndex);
                    break;
                case JAVA_TYPE_LONG:
                    fprintf(out, "    values[%d].set<VendorAtomValue::longValue>(arg%d);\n",
                            atomValueIndex, argIndex);
                    break;
                case JAVA_TYPE_STRING:
                    fprintf(out, "    values[%d].set<VendorAtomValue::stringValue>(arg%d);\n",
                            atomValueIndex, argIndex);
                    break;
                case JAVA_TYPE_BOOLEAN_ARRAY:
                    fprintf(out, "    values[%d].set<VendorAtomValue::repeatedBoolValue>(arg%d);\n",
                            atomValueIndex, argIndex);
                    break;
                case JAVA_TYPE_INT_ARRAY:
                    [[fallthrough]];
                case JAVA_TYPE_ENUM_ARRAY:
                    fprintf(out, "    values[%d].set<VendorAtomValue::repeatedIntValue>(arg%d);\n",
                            atomValueIndex, argIndex);
                    break;
                case JAVA_TYPE_FLOAT_ARRAY:
                    fprintf(out,
                            "    values[%d].set<VendorAtomValue::repeatedFloatValue>(arg%d);\n",
                            atomValueIndex, argIndex);
                    break;
                case JAVA_TYPE_LONG_ARRAY:
                    fprintf(out, "    values[%d].set<VendorAtomValue::repeatedLongValue>(arg%d);\n",
                            atomValueIndex, argIndex);
                    break;
                case JAVA_TYPE_STRING_ARRAY:
                    fprintf(out, "    {\n");
                    fprintf(out, "    vector<optional<string>> arrayValue(\n");
                    fprintf(out, "        arg%d.begin(), arg%d.end());\n", argIndex, argIndex);
                    fprintf(out,
                            "    "
                            "values[%d].set<VendorAtomValue::repeatedStringValue>(std::move("
                            "arrayValue));\n",
                            atomValueIndex);
                    fprintf(out, "    }\n");
                    break;
                default:
                    // Unsupported types: OBJECT, DOUBLE, JAVA_TYPE_ATTRIBUTION_CHAIN
                    fprintf(stderr, "Encountered unsupported type(%d)\n", argType);
                    return 1;
            }
        }
        fprintf(out, "    atom.values = std::move(values);\n");  // end method body.

        // check will be there an atom for this signature with atom level annotations
        const auto& atomAnnotations =
                get_annotations<AtomDeclSet>(ATOM_ID_FIELD_NUMBER, fieldNumberToAtomDeclSet);
        if (atomAnnotations) {
            fprintf(out, "    std::vector<std::optional<Annotation>> atomAnnotations;\n");
            write_native_annotations_vendor_for_field(out, ATOM_ID_FIELD_NUMBER, *atomAnnotations);
            fprintf(out, "    if (atomAnnotations.size() > 0) {\n");
            fprintf(out, "        atom.atomAnnotations = std::move(atomAnnotations);\n");
            fprintf(out, "    }\n\n");
        }

        // Create fieldsAnnotations instance only in case if there is an atom fields with annotation
        // for this signature
        bool atomHasFieldsAnnotation = false;
        for (int argIndex = 2; argIndex <= signature.size(); argIndex++) {
            if (get_annotations<AtomDeclSet>(argIndex, fieldNumberToAtomDeclSet)) {
                atomHasFieldsAnnotation = true;
                break;
            }
        }

        if (atomHasFieldsAnnotation) {
            fprintf(out, "    std::vector<std::optional<AnnotationSet>> fieldsAnnotations;\n");
            for (int argIndex = 2; argIndex <= signature.size(); argIndex++) {
                const auto& fieldAnnotations =
                        get_annotations<AtomDeclSet>(argIndex, fieldNumberToAtomDeclSet);
                if (fieldAnnotations) {
                    write_native_annotations_vendor_for_field(out, argIndex, *fieldAnnotations);
                }
            }
            fprintf(out, "    if (fieldsAnnotations.size() > 0) {\n");
            fprintf(out, "        atom.valuesAnnotations = std::move(fieldsAnnotations);\n");
            fprintf(out, "    }\n\n");
        }

        fprintf(out, "    // elision of copy operations is permitted on return\n");
        fprintf(out, "    return atom;\n");
        fprintf(out, "}\n\n");  // end method.
    }
    return 0;
}

static int write_native_vendor_method_body_typesafe(FILE* out, const AtomDecl& atomDecl) {
    const char* indent2 = "        ";

    fprintf(out, "VendorAtom createVendorAtom(const %s& atom) {\n", atomDecl.message.c_str());
    fprintf(out, "    VendorAtom result;\n");

    // Write method body.
    fprintf(out, "    result.atomId = %d;\n", atomDecl.code);
    fprintf(out, "    result.reverseDomainName = atom.reverse_domain_name;\n");

    // Exclude first field - which is reverseDomainName
    fprintf(out, "    vector<VendorAtomValue> values(%d);\n",
            static_cast<int>(atomDecl.fields.size()) - 1);

    bool atomHasFieldsAnnotation = false;
    // looping over atomDecl->fields due to we need to have access to field names
    for (auto& field : atomDecl.fields) {
        // Exclude first field - which is reverseDomainName
        if (field.fieldNumber == 1) {
            continue;
        }
        const char* fName = field.name.c_str();
        const int atomValueIndex = field.fieldNumber - 2;

        switch (field.javaType) {
            case JAVA_TYPE_BYTE_ARRAY:
                fprintf(out,
                        "    "
                        "values[%d].set<VendorAtomValue::byteArrayValue>(atom.%s);\n",
                        atomValueIndex, fName);
                break;
            case JAVA_TYPE_BOOLEAN:
                fprintf(out, "    values[%d].set<VendorAtomValue::boolValue>(atom.%s);\n",
                        atomValueIndex, fName);
                break;
            case JAVA_TYPE_INT:
                [[fallthrough]];
            case JAVA_TYPE_ENUM:
                fprintf(out,
                        "    "
                        "values[%d].set<VendorAtomValue::intValue>(static_cast<int>(atom.%s));\n",
                        atomValueIndex, fName);
                break;
            case JAVA_TYPE_FLOAT:
                fprintf(out, "    values[%d].set<VendorAtomValue::floatValue>(atom.%s);\n",
                        atomValueIndex, fName);
                break;
            case JAVA_TYPE_LONG:
                fprintf(out, "    values[%d].set<VendorAtomValue::longValue>(atom.%s);\n",
                        atomValueIndex, fName);
                break;
            case JAVA_TYPE_STRING:
                fprintf(out, "    values[%d].set<VendorAtomValue::stringValue>(atom.%s);\n",
                        atomValueIndex, fName);
                break;
            case JAVA_TYPE_BOOLEAN_ARRAY:
                fprintf(out, "    {\n");
                fprintf(out, "        vector<bool> arrayValue(atom.%s.begin(), atom.%s.end());\n",
                        fName, fName);
                fprintf(out,
                        "        "
                        "values[%d].set<VendorAtomValue::repeatedBoolValue>(std::move(arrayValue));"
                        "\n",
                        atomValueIndex);
                fprintf(out, "    }\n");
                break;
            case JAVA_TYPE_INT_ARRAY:
                fprintf(out, "    values[%d].set<VendorAtomValue::repeatedIntValue>(atom.%s);\n",
                        atomValueIndex, fName);
                break;
            case JAVA_TYPE_ENUM_ARRAY:
                fprintf(out, "    {\n");
                fprintf(out, "        vector<int> arrayValue(atom.%s.size());\n", fName);
                fprintf(out, "        for (int i = 0; i < atom.%s.size(); i++) {\n", fName);
                fprintf(out, "            arrayValue[i] = static_cast<int>(atom.%s[i]);\n", fName);
                fprintf(out, "        }\n");
                fprintf(out,
                        "        "
                        "values[%d].set<VendorAtomValue::repeatedIntValue>(std::move(arrayValue));"
                        "\n",
                        atomValueIndex);
                fprintf(out, "    }\n");
                break;
            case JAVA_TYPE_FLOAT_ARRAY:
                fprintf(out, "    values[%d].set<VendorAtomValue::repeatedFloatValue>(atom.%s);\n",
                        atomValueIndex, fName);
                break;
            case JAVA_TYPE_LONG_ARRAY:
                fprintf(out, "    values[%d].set<VendorAtomValue::repeatedLongValue>(atom.%s);\n",
                        atomValueIndex, fName);
                break;
            case JAVA_TYPE_STRING_ARRAY:
                fprintf(out, "    {\n");
                fprintf(out, "        vector<optional<string>> arrayValue(\n");
                fprintf(out, "            atom.%s.begin(), atom.%s.end());\n", fName, fName);
                fprintf(out,
                        "        "
                        "values[%d].set<VendorAtomValue::repeatedStringValue>(std::move("
                        "arrayValue));\n",
                        atomValueIndex);
                fprintf(out, "    }\n");
                break;

            default:
                // Unsupported types: OBJECT, DOUBLE, JAVA_TYPE_ATTRIBUTION_CHAIN
                fprintf(stderr, "Encountered unsupported type(%d) for field \"%s::%s\".\n",
                        field.javaType, atomDecl.message.c_str(), fName);
                return -1;
        }
        atomHasFieldsAnnotation = atomHasFieldsAnnotation ||
                                  get_annotations<AnnotationSet>(field.fieldNumber,
                                                                 atomDecl.fieldNumberToAnnotations);
    }

    fprintf(out, "    result.values = std::move(values);\n");

    // Create fieldsAnnotations instance only in case if there are fields with annotations
    // for this atom
    if (atomHasFieldsAnnotation) {
        fprintf(out, "    // fields level annotations\n");
        fprintf(out, "    std::vector<std::optional<AnnotationSet>> fieldsAnnotations;\n");
        for (int fieldIdx = 1; fieldIdx < atomDecl.fields.size(); fieldIdx++) {
            const int fieldProtoNumber = atomDecl.fields[fieldIdx].fieldNumber;
            const auto& fieldAnnotations = get_annotations<AnnotationSet>(
                    fieldProtoNumber, atomDecl.fieldNumberToAnnotations);
            if (fieldAnnotations) {
                const int valueIndex = fieldProtoNumber - 2;
                fprintf(out, "    {\n");
                fprintf(out, "        std::vector<Annotation> annotations;\n");
                const string atomWithfieldName = string("atom.") + atomDecl.fields[fieldIdx].name;
                write_field_annotations(out, *fieldAnnotations, atomWithfieldName.c_str(), indent2);
                fprintf(out, "        AnnotationSet fieldAnnotations;\n");
                fprintf(out, "        fieldAnnotations.valueIndex = %d;\n", valueIndex);
                fprintf(out, "        fieldAnnotations.annotations = std::move(annotations);\n");
                fprintf(out, "        fieldsAnnotations.push_back(std::move(fieldAnnotations));\n");
                fprintf(out, "    }\n");
            }
        }
        fprintf(out, "    result.valuesAnnotations = std::move(fieldsAnnotations);\n");
    }

    const auto& atomAnnotations =
            get_annotations<AnnotationSet>(ATOM_ID_FIELD_NUMBER, atomDecl.fieldNumberToAnnotations);
    if (atomAnnotations) {
        fprintf(out, "    // atom level annotations\n");
        fprintf(out, "    {\n");
        fprintf(out, "        std::vector<std::optional<Annotation>> annotations;\n");
        write_field_annotations(out, *atomAnnotations, "", indent2);
        fprintf(out, "        result.atomAnnotations = std::move(annotations);\n");
        fprintf(out, "    }\n");
    }

    fprintf(out, "    return result;\n");
    fprintf(out, "}\n\n");
    return 0;
}

static int write_native_create_vendor_atom_methods_typesafe(FILE* out, const Atoms& atoms) {
    for (auto& atomDecl : atoms.decls) {
        if (atomDecl->atomType == ATOM_TYPE_PUSHED) {
            const int ret = write_native_vendor_method_body_typesafe(out, *atomDecl);
            if (ret != 0) {
                return ret;
            }
        } else {
            fprintf(stderr, "[WARN] Vendor pulled atom - not supported. Vote up b/447079434\n");
            return 1;
        }
    }

    return 0;
}

int write_stats_log_cpp_vendor(FILE* out, const Atoms& atoms, const AtomDecl& attributionDecl,
                               const string& cppNamespace, const string& importHeader,
                               bool includeExtraSrcs) {
    // Print prelude
    int ret = write_native_source_preamble(out, atoms, importHeader, API_LEVEL_CURRENT,
                                           cppNamespace, InterfaceApi::VENDOR, includeExtraSrcs);
    if (ret != 0) {
        return ret;
    }

    ret = write_native_create_vendor_atom_methods(out, atoms.signatureInfoMap, attributionDecl);
    if (ret != 0) {
        return ret;
    }
    // Print footer
    fprintf(out, "\n");
    write_closing_namespace(out, cppNamespace);

    return 0;
}

int write_stats_log_header_vendor(FILE* out, const Atoms& atoms, const AtomDecl& attributionDecl,
                                  const string& cppNamespace, bool includeExtraSrcs) {
    write_native_header_preamble(out, atoms, cppNamespace, InterfaceApi::VENDOR, includeExtraSrcs);
    write_native_atom_constants(out, atoms, attributionDecl, "createVendorAtom(",
                                /*isVendorAtomLogging=*/true);

    // Print Atom classes definitions including enums
    fprintf(out, "//\n");
    fprintf(out, "// Atom definitions including enums\n");
    fprintf(out, "//\n");

    if (write_native_atom_types(out, atoms, /*pushedApiName*/ nullptr, /*includeFields*/ false) !=
        0) {
        return 1;
    };

    fprintf(out, "using ::aidl::android::frameworks::stats::VendorAtom;\n");

    // Print write methods
    fprintf(out, "//\n");
    fprintf(out, "// Write methods\n");
    fprintf(out, "//\n");
    write_native_method_header(out, "VendorAtom createVendorAtom(", atoms.signatureInfoMap,
                               attributionDecl,
                               /*isVendorAtomLogging=*/true);
    fprintf(out, "\n");

    write_native_header_epilogue(out, cppNamespace);

    return 0;
}

int write_stats_log_cpp_vendor_typesafe(FILE* out, const Atoms& atoms, const string& cppNamespace,
                                        const string& importHeader, bool includeExtraSrcs) {
    int ret = write_native_source_preamble(out, atoms, importHeader, API_LEVEL_CURRENT,
                                           cppNamespace, InterfaceApi::VENDOR, includeExtraSrcs);
    if (ret != 0) {
        return ret;
    }

    ret = write_native_create_vendor_atom_methods_typesafe(out, atoms);
    if (ret != 0) {
        return ret;
    }

    // Print footer
    write_closing_namespace(out, cppNamespace);

    return 0;
}

int write_stats_log_header_vendor_typesafe(FILE* out, const Atoms& atoms,
                                           const string& cppNamespace, bool includeExtraSrcs) {
    write_native_header_preamble(out, atoms, cppNamespace, InterfaceApi::VENDOR, includeExtraSrcs);

    // Print Atom classes definitions including enums
    fprintf(out, "//\n");
    fprintf(out, "// Atom definitions including enums\n");
    fprintf(out, "//\n");

    if (write_native_atom_types(out, atoms, "createVendorAtom", /*includeFields*/ true) != 0) {
        return 1;
    };

    fprintf(out, "using ::aidl::android::frameworks::stats::VendorAtom;\n");

    // Print write methods
    fprintf(out, "//\n");
    fprintf(out, "// Write methods\n");
    fprintf(out, "//\n");

    for (auto& atomDecl : atoms.decls) {
        if (atomDecl->atomType == ATOM_TYPE_PUSHED) {
            fprintf(out, "VendorAtom createVendorAtom(const %s& atom);\n",
                    atomDecl->message.c_str());
        } else {
            fprintf(stderr, "[WARN] Vendor pulled atom - not supported. Vote up b/447079434\n");
            return 1;
        }
    }

    write_native_header_epilogue(out, cppNamespace);

    return 0;
}

}  // namespace stats_log_api_gen
}  // namespace android
