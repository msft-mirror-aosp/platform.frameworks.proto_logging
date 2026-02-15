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

#include "native_writer.h"

#include <stdio.h>

#include <cstdint>
#include <string>

#include "Collation.h"
#include "utils.h"

namespace android {
namespace stats_log_api_gen {

static void write_native_annotation_constants(FILE* out) {
    fprintf(out, "// Annotation constants.\n");

    const map<AnnotationId, AnnotationStruct>& ANNOTATION_ID_CONSTANTS =
            get_annotation_id_constants(ANNOTATION_CONSTANT_NAME_PREFIX);
    for (const auto& [id, annotation] : ANNOTATION_ID_CONSTANTS) {
        fprintf(out, "const uint8_t %s = %hhu;\n", annotation.name.c_str(), id);
    }
    fprintf(out, "\n");
}

static void write_annotation_set(FILE* out, const AnnotationSet& annotations,
                                 const string& fieldArgName, const string& methodPrefix,
                                 const string& methodSuffix, const string& constantPrefix) {
    const map<AnnotationId, AnnotationStruct>& ANNOTATION_ID_CONSTANTS =
            get_annotation_id_constants(ANNOTATION_CONSTANT_NAME_PREFIX);
    int resetState = -1;
    int defaultState = -1;
    for (const shared_ptr<Annotation>& annotation : annotations) {
        const string& annotationConstant =
                ANNOTATION_ID_CONSTANTS.at(annotation->annotationId).name;
        switch (annotation->type) {
            case ANNOTATION_TYPE_INT:
                if (ANNOTATION_ID_TRIGGER_STATE_RESET == annotation->annotationId) {
                    resetState = annotation->value.intValue;
                } else if (ANNOTATION_ID_DEFAULT_STATE == annotation->annotationId) {
                    defaultState = annotation->value.intValue;
                } else if (ANNOTATION_ID_RESTRICTION_CATEGORY == annotation->annotationId) {
                    fprintf(out, "        %saddInt32Annotation(%s%s%s,\n", methodPrefix.c_str(),
                            methodSuffix.c_str(), constantPrefix.c_str(),
                            annotationConstant.c_str());
                    fprintf(out, "                                       %s%s);\n",
                            constantPrefix.c_str(),
                            get_restriction_category_str(annotation->value.intValue).c_str());
                } else {
                    fprintf(out, "        %saddInt32Annotation(%s%s%s, %d);\n",
                            methodPrefix.c_str(), methodSuffix.c_str(), constantPrefix.c_str(),
                            annotationConstant.c_str(), annotation->value.intValue);
                }
                break;
            case ANNOTATION_TYPE_BOOL:
                fprintf(out, "        %saddBoolAnnotation(%s%s%s, %s);\n", methodPrefix.c_str(),
                        methodSuffix.c_str(), constantPrefix.c_str(), annotationConstant.c_str(),
                        annotation->value.boolValue ? "true" : "false");
                break;
            default:
                break;
        }
    }
    if (defaultState != -1 && resetState != -1) {
        const string& annotationConstant =
                ANNOTATION_ID_CONSTANTS.at(ANNOTATION_ID_TRIGGER_STATE_RESET).name;
        fprintf(out, "        if (static_cast<int32_t>(%s) == %d) {\n", fieldArgName.c_str(),
                resetState);
        fprintf(out, "            %saddInt32Annotation(%s%s%s, %d);\n", methodPrefix.c_str(),
                methodSuffix.c_str(), constantPrefix.c_str(), annotationConstant.c_str(),
                defaultState);
        fprintf(out, "        }\n");
    }
}

static void write_annotations(FILE* out, int argIndex,
                              const FieldNumberToAtomDeclSet& fieldNumberToAtomDeclSet,
                              const string& methodPrefix, const string& methodSuffix,
                              const int minApiLevel) {
    const FieldNumberToAtomDeclSet::const_iterator fieldNumberToAtomDeclSetIt =
            fieldNumberToAtomDeclSet.find(argIndex);
    if (fieldNumberToAtomDeclSet.end() == fieldNumberToAtomDeclSetIt) {
        return;
    }
    const AtomDeclSet& atomDeclSet = fieldNumberToAtomDeclSetIt->second;
    const string constantPrefix = minApiLevel > API_R ? "ASTATSLOG_" : "";
    for (const shared_ptr<AtomDecl>& atomDecl : atomDeclSet) {
        const string atomConstant = make_constant_name(atomDecl->name);
        fprintf(out, "    if (%s == code) {\n", atomConstant.c_str());
        const AnnotationSet& annotations = atomDecl->fieldNumberToAnnotations.at(argIndex);
        const string fieldArgName = "arg" + std::to_string(argIndex);
        write_annotation_set(out, annotations, fieldArgName, methodPrefix, methodSuffix,
                             constantPrefix);
        fprintf(out, "    }\n");
    }
}

static int write_native_method_body(FILE* out, const vector<java_type_t>& signature,
                                    const FieldNumberToAtomDeclSet& fieldNumberToAtomDeclSet,
                                    const AtomDecl& attributionDecl, const int minApiLevel) {
    int argIndex = 1;
    fprintf(out, "    AStatsEvent_setAtomId(event, code);\n");
    write_annotations(out, ATOM_ID_FIELD_NUMBER, fieldNumberToAtomDeclSet, "AStatsEvent_",
                      "event, ", minApiLevel);
    for (vector<java_type_t>::const_iterator arg = signature.begin(); arg != signature.end();
         arg++) {
        if (minApiLevel < API_T && is_repeated_field(*arg)) {
            fprintf(stderr, "Found repeated field type with minApiLevel < T.");
            return 1;
        }
        switch (*arg) {
            case JAVA_TYPE_ATTRIBUTION_CHAIN: {
                const char* uidName = attributionDecl.fields.front().name.c_str();
                const char* tagName = attributionDecl.fields.back().name.c_str();
                fprintf(out,
                        "    AStatsEvent_writeAttributionChain(event, "
                        "reinterpret_cast<const uint32_t*>(%s), %s.data(), "
                        "static_cast<uint8_t>(%s_length));\n",
                        uidName, tagName, uidName);
                break;
            }
            case JAVA_TYPE_BYTE_ARRAY:
                fprintf(out,
                        "    AStatsEvent_writeByteArray(event, "
                        "reinterpret_cast<const uint8_t*>(arg%d.arg), "
                        "arg%d.arg_length);\n",
                        argIndex, argIndex);
                break;
            case JAVA_TYPE_BOOLEAN:
                fprintf(out, "    AStatsEvent_writeBool(event, arg%d);\n", argIndex);
                break;
            case JAVA_TYPE_INT:
                [[fallthrough]];
            case JAVA_TYPE_ENUM:
                fprintf(out, "    AStatsEvent_writeInt32(event, arg%d);\n", argIndex);
                break;
            case JAVA_TYPE_FLOAT:
                fprintf(out, "    AStatsEvent_writeFloat(event, arg%d);\n", argIndex);
                break;
            case JAVA_TYPE_LONG:
                fprintf(out, "    AStatsEvent_writeInt64(event, arg%d);\n", argIndex);
                break;
            case JAVA_TYPE_STRING:
                fprintf(out, "    AStatsEvent_writeString(event, arg%d);\n", argIndex);
                break;
            case JAVA_TYPE_BOOLEAN_ARRAY:
                fprintf(out, "    AStatsEvent_writeBoolArray(event, arg%d, arg%d_length);\n",
                        argIndex, argIndex);
                break;
            case JAVA_TYPE_INT_ARRAY:
                [[fallthrough]];
            case JAVA_TYPE_ENUM_ARRAY:
                fprintf(out,
                        "    AStatsEvent_writeInt32Array(event, arg%d.data(), arg%d.size());\n",
                        argIndex, argIndex);
                break;
            case JAVA_TYPE_FLOAT_ARRAY:
                fprintf(out,
                        "    AStatsEvent_writeFloatArray(event, arg%d.data(), arg%d.size());\n",
                        argIndex, argIndex);
                break;
            case JAVA_TYPE_LONG_ARRAY:
                fprintf(out,
                        "    AStatsEvent_writeInt64Array(event, arg%d.data(), arg%d.size());\n",
                        argIndex, argIndex);
                break;
            case JAVA_TYPE_STRING_ARRAY:
                fprintf(out,
                        "    AStatsEvent_writeStringArray(event, arg%d.data(), arg%d.size());\n",
                        argIndex, argIndex);
                break;

            default:
                // Unsupported types: OBJECT, DOUBLE
                fprintf(stderr, "Encountered unsupported type.\n");
                return 1;
        }
        write_annotations(out, argIndex, fieldNumberToAtomDeclSet, "AStatsEvent_", "event, ",
                          minApiLevel);
        argIndex++;
    }
    return 0;
}

static int write_native_method_body_typesafe(FILE* out, const AtomDecl& atomDecl,
                                             const int minApiLevel) {
    const string constantPrefix = minApiLevel > API_R ? "ASTATSLOG_" : "";

    fprintf(out, "    AStatsEvent_setAtomId(event, %d);\n", atomDecl.code);

    auto atomAnnotations = atomDecl.fieldNumberToAnnotations.find(ATOM_ID_FIELD_NUMBER);
    if (atomAnnotations != atomDecl.fieldNumberToAnnotations.end()) {
        write_annotation_set(out, atomAnnotations->second, "", "AStatsEvent_", "event, ",
                             constantPrefix);
    }

    // looping over atomDecl->fields due to we need to have access to field names
    for (auto& field : atomDecl.fields) {
        const char* fName = field.name.c_str();
        switch (field.javaType) {
            case JAVA_TYPE_ATTRIBUTION_CHAIN:
                fprintf(out, "    std::vector<uint32_t> aChainUids_%s;\n", fName);
                fprintf(out, "    aChainUids_%s.reserve(atom.%s.size());\n", fName, fName);
                fprintf(out, "    std::vector<const char*> aChainTags_%s;\n", fName);
                fprintf(out, "    aChainUids_%s.reserve(atom.%s.size());\n", fName, fName);
                fprintf(out, "    for (auto& aNode : atom.%s) {\n", fName);
                fprintf(out, "        aChainUids_%s.push_back(aNode.uid);\n", fName);
                fprintf(out, "        aChainTags_%s.push_back(aNode.tag.c_str());\n", fName);
                fprintf(out, "    }\n");
                fprintf(out,
                        "    AStatsEvent_writeAttributionChain(event, "
                        "aChainUids_%s.data(), aChainTags_%s.data(), "
                        "static_cast<uint8_t>(aChainUids_%s.size()));\n",
                        fName, fName, fName);
                break;
            case JAVA_TYPE_BYTE_ARRAY:
                fprintf(out,
                        "    AStatsEvent_writeByteArray(event, "
                        "reinterpret_cast<const uint8_t*>(atom.%s.data()), "
                        "atom.%s.size());\n",
                        fName, fName);
                break;
            case JAVA_TYPE_BOOLEAN:
                fprintf(out, "    AStatsEvent_writeBool(event, atom.%s);\n", fName);
                break;
            case JAVA_TYPE_INT:
                [[fallthrough]];
            case JAVA_TYPE_ENUM:
                fprintf(out, "    AStatsEvent_writeInt32(event, static_cast<int32_t>(atom.%s));\n",
                        fName);
                break;
            case JAVA_TYPE_FLOAT:
                fprintf(out, "    AStatsEvent_writeFloat(event, atom.%s);\n", fName);
                break;
            case JAVA_TYPE_LONG:
                fprintf(out, "    AStatsEvent_writeInt64(event, atom.%s);\n", fName);
                break;
            case JAVA_TYPE_STRING:
                fprintf(out, "    AStatsEvent_writeString(event, atom.%s.c_str());\n", fName);
                break;
            case JAVA_TYPE_BOOLEAN_ARRAY:
                fprintf(out,
                        "    AStatsEvent_writeBoolArray(event, reinterpret_cast<const "
                        "bool*>(atom.%s.data()), atom.%s.size());\n",
                        fName, fName);
                break;
            case JAVA_TYPE_INT_ARRAY:
                [[fallthrough]];
            case JAVA_TYPE_ENUM_ARRAY:
                fprintf(out,
                        "    AStatsEvent_writeInt32Array(event, reinterpret_cast<const "
                        "int32_t*>(atom.%s.data()), atom.%s.size());\n",
                        fName, fName);
                break;
            case JAVA_TYPE_FLOAT_ARRAY:
                fprintf(out,
                        "    AStatsEvent_writeFloatArray(event, atom.%s.data(), atom.%s.size());\n",
                        fName, fName);
                break;
            case JAVA_TYPE_LONG_ARRAY:
                fprintf(out,
                        "    AStatsEvent_writeInt64Array(event, atom.%s.data(), atom.%s.size());\n",
                        fName, fName);
                break;
            case JAVA_TYPE_STRING_ARRAY:
                fprintf(out, "    std::vector<const char*> sArray_%s;\n", fName);
                fprintf(out, "    sArray_%s.reserve(atom.%s.size());\n", fName, fName);
                fprintf(out, "    for (const auto& str : atom.%s) {\n", fName);
                fprintf(out, "        sArray_%s.push_back(str.c_str());\n", fName);
                fprintf(out, "    }\n");
                fprintf(out,
                        "    AStatsEvent_writeStringArray(event, sArray_%s.data(), "
                        "sArray_%s.size());\n",
                        fName, fName);
                break;

            default:
                // Unsupported types: OBJECT, DOUBLE
                fprintf(stderr, "Encountered unsupported type.\n");
                return -1;
        }

        auto atomAnnotations = atomDecl.fieldNumberToAnnotations.find(field.fieldNumber);
        if (atomAnnotations != atomDecl.fieldNumberToAnnotations.end()) {
            const string fieldArgName = "atom." + field.name;
            write_annotation_set(out, atomAnnotations->second, fieldArgName, "AStatsEvent_",
                                 "event, ", constantPrefix);
        }
    }
    return 0;
}

static void write_native_method_call(FILE* out, const string& methodName,
                                     const vector<java_type_t>& signature,
                                     const AtomDecl& attributionDecl, int argIndex) {
    fprintf(out, "%s(code", methodName.c_str());
    for (vector<java_type_t>::const_iterator arg = signature.begin(); arg != signature.end();
         arg++) {
        if (*arg == JAVA_TYPE_ATTRIBUTION_CHAIN) {
            for (const auto& chainField : attributionDecl.fields) {
                if (chainField.javaType == JAVA_TYPE_STRING) {
                    fprintf(out, ", %s", chainField.name.c_str());
                } else {
                    fprintf(out, ",  %s,  %s_length", chainField.name.c_str(),
                            chainField.name.c_str());
                }
            }
        } else {
            fprintf(out, ", arg%d", argIndex);

            if (*arg == JAVA_TYPE_BOOLEAN_ARRAY) {
                fprintf(out, ", arg%d_length", argIndex);
            }
        }
        argIndex++;
    }
    fprintf(out, ");\n");
}

static int write_native_stats_write_methods_typesafe(FILE* out, const Atoms& atoms,
                                                     const int minApiLevel) {
    for (auto& atomDecl : atoms.decls) {
        const bool atomHasRepeatedFields = contains_repeated_field(atomDecl->fields);
        if (minApiLevel < API_T && atomHasRepeatedFields) {
            fprintf(stderr, "Found repeated field type with minApiLevel < T.");
            return 1;
        }

        const string apiCloser = atomHasRepeatedFields ? " __INTRODUCED_IN(__ANDROID_API_T__)" : "";

        if (atomDecl->atomType == ATOM_TYPE_PUSHED) {
            fprintf(out, "int stats_write(const %s& atom)%s {\n", atomDecl->message.c_str(),
                    apiCloser.c_str());
            fprintf(out, "    AStatsEvent* event = AStatsEvent_obtain();\n");
            int ret = write_native_method_body_typesafe(out, *atomDecl, minApiLevel);
            if (ret != 0) {
                return ret;
            }
            fprintf(out, "    const int ret = AStatsEvent_write(event);\n");
            fprintf(out, "    AStatsEvent_release(event);\n");
            fprintf(out, "    return ret;\n");
        } else {
            fprintf(out, "void addAStatsEvent(AStatsEventList* pulled_data, const %s& atom)%s {\n",
                    atomDecl->message.c_str(), apiCloser.c_str());
            fprintf(out, "    AStatsEvent* event = AStatsEventList_addStatsEvent(pulled_data);\n");
            int ret = write_native_method_body_typesafe(out, *atomDecl, minApiLevel);
            if (ret != 0) {
                return ret;
            }
            fprintf(out, "    AStatsEvent_build(event);\n");
        }

        fprintf(out, "}\n\n");
    }

    return 0;
}

static int write_native_stats_write_methods(FILE* out, const SignatureInfoMap& signatureInfoMap,
                                            const AtomDecl& attributionDecl, const int minApiLevel,
                                            bool bootstrap) {
    fprintf(out, "\n");
    for (const auto& [signature, fieldNumberToAtomDeclSet] : signatureInfoMap) {
        write_native_method_signature(out, "int stats_write(", signature, attributionDecl, " {");

        // Write method body.
        if (bootstrap) {
            fprintf(out, "    ::android::os::StatsBootstrapAtom atom;\n");
            fprintf(out, "    atom.atomId = code;\n");
            const FieldNumberToAtomDeclSet::const_iterator fieldNumberToAtomDeclSetIt =
                    fieldNumberToAtomDeclSet.find(ATOM_ID_FIELD_NUMBER);
            if (fieldNumberToAtomDeclSet.end() != fieldNumberToAtomDeclSetIt) {
                fprintf(stderr, "Top-level bootstrap atoms do not support annotations\n");
                return 1;
            }
            int argIndex = 1;
            const char* atomVal = "::android::os::StatsBootstrapAtomValue";
            const char* primitiveVal = "::android::os::StatsBootstrapAtomValue::Primitive::";
            const char* annotationVal = "::android::os::StatsBootstrapAtomValue::Annotation";
            const char* annotationIdVal =
                    "::android::os::StatsBootstrapAtomValue::Annotation::Id::";
            const char* annotationPrimitiveVal =
                    "::android::os::StatsBootstrapAtomValue::Annotation::Primitive::";
            for (vector<java_type_t>::const_iterator arg = signature.begin();
                 arg != signature.end(); arg++) {
                fprintf(out, "    %s value%d;\n", atomVal, argIndex);
                switch (*arg) {
                    case JAVA_TYPE_BYTE_ARRAY:
                        fprintf(out,
                                "    const uint8_t* arg%dbyte = reinterpret_cast<const "
                                "uint8_t*>(arg%d.arg);\n",
                                argIndex, argIndex);
                        fprintf(out,
                                "    value%d.value = %smake<%sbytesValue>(std::vector(arg%dbyte, "
                                "arg%dbyte + arg%d.arg_length));\n",
                                argIndex, primitiveVal, primitiveVal, argIndex, argIndex, argIndex);
                        break;
                    case JAVA_TYPE_BOOLEAN:
                        fprintf(out, "    value%d.value = %smake<%sboolValue>(arg%d);\n", argIndex,
                                primitiveVal, primitiveVal, argIndex);
                        break;
                    case JAVA_TYPE_INT:  // Fall through.
                    case JAVA_TYPE_ENUM:
                        fprintf(out, "    value%d.value = %smake<%sintValue>(arg%d);\n", argIndex,
                                primitiveVal, primitiveVal, argIndex);
                        break;
                    case JAVA_TYPE_FLOAT:
                        fprintf(out, "    value%d.value = %smake<%sfloatValue>(arg%d);\n", argIndex,
                                primitiveVal, primitiveVal, argIndex);
                        break;
                    case JAVA_TYPE_LONG:
                        fprintf(out, "    value%d.value = %smake<%slongValue>(arg%d);\n", argIndex,
                                primitiveVal, primitiveVal, argIndex);
                        break;
                    case JAVA_TYPE_STRING:
                        fprintf(out,
                                "    value%d.value = %smake<%sstringValue>("
                                "::android::String16(arg%d));\n",
                                argIndex, primitiveVal, primitiveVal, argIndex);
                        break;
                    case JAVA_TYPE_STRING_ARRAY:
                        fprintf(out,
                                "    value%d.value = %smake<%sstringArrayValue>("
                                "arg%d.begin(), arg%d.end());\n",
                                argIndex, primitiveVal, primitiveVal, argIndex, argIndex);
                        break;
                    default:
                        // Unsupported types: OBJECT, DOUBLE, ATTRIBUTION_CHAIN,
                        // and all repeated fields
                        fprintf(stderr, "Unsupported Bootstrap atom field type %s at position %d\n",
                                java_type_to_string(*arg), argIndex);
                        return 1;
                }
                const FieldNumberToAtomDeclSet::const_iterator fieldNumberToAtomDeclSetIt =
                        fieldNumberToAtomDeclSet.find(argIndex);
                // Scrub for any annotations that aren't UIDs
                if (fieldNumberToAtomDeclSet.end() != fieldNumberToAtomDeclSetIt) {
                    const AtomDeclSet& atomDeclSet = fieldNumberToAtomDeclSetIt->second;
                    for (const shared_ptr<AtomDecl>& atomDecl : atomDeclSet) {
                        const string atomConstant = make_constant_name(atomDecl->name);
                        fprintf(out, "    if (%s == code) {\n", atomConstant.c_str());
                        int32_t annotationIndex = 0;
                        for (const shared_ptr<Annotation>& annotation :
                             atomDecl->fieldNumberToAnnotations.at(argIndex)) {
                            if (annotation->annotationId != ANNOTATION_ID_IS_UID) {
                                fprintf(stderr,
                                        "Bootstrap atom fields do not support non-UID "
                                        "annotations\n");
                                return 1;
                            }

                            if (annotationIndex >= 1) {
                                fprintf(stderr,
                                        "Bootstrap atom fields do not support multiple "
                                        "annotations\n");
                                return 1;
                            }

                            fprintf(out, "        %s annotation%d;\n", annotationVal,
                                    annotationIndex);
                            fprintf(out, "        annotation%d.id = %sIS_UID;\n", annotationIndex,
                                    annotationIdVal);
                            fprintf(out,
                                    "        annotation%d.value = "
                                    "%smake<%sboolValue>(true);\n",
                                    annotationIndex, annotationPrimitiveVal,
                                    annotationPrimitiveVal);
                            fprintf(out, "        value%d.annotations.push_back(annotation%d);\n",
                                    argIndex, annotationIndex);
                            annotationIndex++;
                        }
                        fprintf(out, "    }\n");
                    }
                }
                fprintf(out, "    atom.values.push_back(value%d);\n", argIndex);
                argIndex++;
            }
            fprintf(out,
                    "    bool success = "
                    "::android::os::stats::StatsBootstrapAtomClient::reportBootstrapAtom(atom);\n");
            fprintf(out, "    return success ? 0 : -1;\n");

        } else if (minApiLevel == API_Q) {
            int argIndex = 1;
            fprintf(out, "    StatsEventCompat event;\n");
            fprintf(out, "    event.setAtomId(code);\n");
            write_annotations(out, ATOM_ID_FIELD_NUMBER, fieldNumberToAtomDeclSet, "event.", "",
                              minApiLevel);
            for (vector<java_type_t>::const_iterator arg = signature.begin();
                 arg != signature.end(); arg++) {
                switch (*arg) {
                    case JAVA_TYPE_ATTRIBUTION_CHAIN: {
                        const char* uidName = attributionDecl.fields.front().name.c_str();
                        const char* tagName = attributionDecl.fields.back().name.c_str();
                        fprintf(out, "    event.writeAttributionChain(%s, %s_length, %s);\n",
                                uidName, uidName, tagName);
                        break;
                    }
                    case JAVA_TYPE_BYTE_ARRAY:
                        fprintf(out, "    event.writeByteArray(arg%d.arg, arg%d.arg_length);\n",
                                argIndex, argIndex);
                        break;
                    case JAVA_TYPE_BOOLEAN:
                        fprintf(out, "    event.writeBool(arg%d);\n", argIndex);
                        break;
                    case JAVA_TYPE_INT:  // Fall through.
                    case JAVA_TYPE_ENUM:
                        fprintf(out, "    event.writeInt32(arg%d);\n", argIndex);
                        break;
                    case JAVA_TYPE_FLOAT:
                        fprintf(out, "    event.writeFloat(arg%d);\n", argIndex);
                        break;
                    case JAVA_TYPE_LONG:
                        fprintf(out, "    event.writeInt64(arg%d);\n", argIndex);
                        break;
                    case JAVA_TYPE_STRING:
                        fprintf(out, "    event.writeString(arg%d);\n", argIndex);
                        break;
                    default:
                        // Unsupported types: OBJECT, DOUBLE, and all repeated
                        // fields.
                        fprintf(stderr, "Encountered unsupported type.\n");
                        return 1;
                }
                write_annotations(out, argIndex, fieldNumberToAtomDeclSet, "event.", "",
                                  minApiLevel);
                argIndex++;
            }
            fprintf(out, "    return event.writeToSocket();\n");  // end method body.
        } else {
            fprintf(out, "    AStatsEvent* event = AStatsEvent_obtain();\n");
            const int ret = write_native_method_body(out, signature, fieldNumberToAtomDeclSet,
                                                     attributionDecl, minApiLevel);
            if (ret != 0) {
                return ret;
            }
            fprintf(out, "    const int ret = AStatsEvent_write(event);\n");
            fprintf(out, "    AStatsEvent_release(event);\n");
            fprintf(out, "    return ret;\n");  // end method body.
        }
        fprintf(out, "}\n\n");  // end method.
    }
    return 0;
}

static void write_native_stats_write_non_chained_methods(FILE* out,
                                                         const SignatureInfoMap& signatureInfoMap,
                                                         const AtomDecl& attributionDecl) {
    fprintf(out, "\n");
    for (const auto& [signature, _] : signatureInfoMap) {
        write_native_method_signature(out, "int stats_write_non_chained(", signature,
                                      attributionDecl, " {");

        vector<java_type_t> newSignature;

        // First two args form the attribution node so size goes down by 1.
        newSignature.reserve(signature.size() - 1);

        // First arg is Attribution Chain.
        newSignature.push_back(JAVA_TYPE_ATTRIBUTION_CHAIN);

        // Followed by the originial signature except the first 2 args.
        newSignature.insert(newSignature.end(), signature.begin() + 2, signature.end());

        const char* uidName = attributionDecl.fields.front().name.c_str();
        const char* tagName = attributionDecl.fields.back().name.c_str();
        fprintf(out, "    const int32_t* %s = &arg1;\n", uidName);
        fprintf(out, "    const size_t %s_length = 1;\n", uidName);
        fprintf(out, "    const std::vector<char const*> %s(1, arg2);\n", tagName);
        fprintf(out, "    return ");
        write_native_method_call(out, "stats_write", newSignature, attributionDecl, 2);

        fprintf(out, "}\n\n");
    }
}

static int write_native_build_stats_event_methods(FILE* out,
                                                  const SignatureInfoMap& signatureInfoMap,
                                                  const AtomDecl& attributionDecl,
                                                  const int minApiLevel) {
    fprintf(out, "\n");
    for (const auto& [signature, fieldNumberToAtomDeclSet] : signatureInfoMap) {
        write_native_method_signature(out, "void addAStatsEvent(AStatsEventList* pulled_data, ",
                                      signature, attributionDecl, " {");

        fprintf(out, "    AStatsEvent* event = AStatsEventList_addStatsEvent(pulled_data);\n");
        const int ret = write_native_method_body(out, signature, fieldNumberToAtomDeclSet,
                                                 attributionDecl, minApiLevel);
        if (ret != 0) {
            return ret;
        }
        fprintf(out, "    AStatsEvent_build(event);\n");  // end method body.

        fprintf(out, "}\n\n");  // end method.
    }
    return 0;
}

int write_stats_log_cpp(FILE* out, const Atoms& atoms, const AtomDecl& attributionDecl,
                        const string& cppNamespace, const string& importHeader,
                        const int minApiLevel, InterfaceApi interfaceApi, bool includeExtraSrcs) {
    int ret = write_native_source_preamble(out, atoms, importHeader, minApiLevel, cppNamespace,
                                           interfaceApi, includeExtraSrcs);
    if (ret != 0) {
        return ret;
    }

    ret = write_native_stats_write_methods(out, atoms.signatureInfoMap, attributionDecl,
                                           minApiLevel, interfaceApi == InterfaceApi::BOOTSTRAP);
    if (ret != 0) {
        return ret;
    }
    if (interfaceApi == InterfaceApi::PLATFORM) {
        write_native_stats_write_non_chained_methods(out, atoms.nonChainedSignatureInfoMap,
                                                     attributionDecl);
        ret = write_native_build_stats_event_methods(out, atoms.pulledAtomsSignatureInfoMap,
                                                     attributionDecl, minApiLevel);
        if (ret != 0) {
            return ret;
        }
    }

    // Print footer
    fprintf(out, "\n");
    write_closing_namespace(out, cppNamespace);

    return 0;
}

int write_stats_log_header(FILE* out, const Atoms& atoms, const AtomDecl& attributionDecl,
                           const string& cppNamespace, const int minApiLevel,
                           InterfaceApi interfaceApi, bool includeExtraSrcs) {
    write_native_header_preamble(out, atoms, cppNamespace, interfaceApi, includeExtraSrcs);
    write_native_atom_constants(out, atoms, attributionDecl);
    write_native_atom_enums(out, atoms);

    if (minApiLevel <= API_R) {
        write_native_annotation_constants(out);
    }

    fprintf(out, "struct BytesField {\n");
    fprintf(out,
            "  BytesField(char const* array, size_t len) : arg(array), "
            "arg_length(len) {}\n");
    fprintf(out, "  char const* arg;\n");
    fprintf(out, "  size_t arg_length;\n");
    fprintf(out, "};\n");
    fprintf(out, "\n");

    // Print write methods
    fprintf(out, "//\n");
    fprintf(out, "// Write methods\n");
    fprintf(out, "//\n");
    write_native_method_header(out, "int stats_write(", atoms.signatureInfoMap, attributionDecl);
    fprintf(out, "\n");

    // Attribution chains and pulled atoms are not supported for bootstrap processes.
    if (interfaceApi != InterfaceApi::BOOTSTRAP) {
        fprintf(out, "//\n");
        fprintf(out, "// Write flattened methods\n");
        fprintf(out, "//\n");
        write_native_method_header(out, "int stats_write_non_chained(",
                                   atoms.nonChainedSignatureInfoMap, attributionDecl);
        fprintf(out, "\n");

        // Print pulled atoms methods.
        fprintf(out, "//\n");
        fprintf(out, "// Add AStatsEvent methods\n");
        fprintf(out, "//\n");
        write_native_method_header(out, "void addAStatsEvent(AStatsEventList* pulled_data, ",
                                   atoms.pulledAtomsSignatureInfoMap, attributionDecl);
        fprintf(out, "\n");
    }

    write_native_header_epilogue(out, cppNamespace);

    return 0;
}

int write_stats_log_cpp_typesafe(FILE* out, const Atoms& atoms, const string& cppNamespace,
                                 const string& importHeader, const int minApiLevel,
                                 InterfaceApi interfaceApi, bool includeExtraSrcs) {
    if (interfaceApi == InterfaceApi::BOOTSTRAP) {
        fprintf(stderr,
                "Type-safe APIs generation for C++ bootstrap is not supported."
                "Vote-up http://b/449793167 for support.\n");
        return 1;
    }

    int ret = write_native_source_preamble(out, atoms, importHeader, minApiLevel, cppNamespace,
                                           interfaceApi, includeExtraSrcs);
    if (ret != 0) {
        return ret;
    }

    ret = write_native_stats_write_methods_typesafe(out, atoms, minApiLevel);
    if (ret != 0) {
        return ret;
    }

    // Print footer
    fprintf(out, "\n");
    write_closing_namespace(out, cppNamespace);

    return 0;
}

int write_stats_log_header_typesafe(FILE* out, const Atoms& atoms, const string& cppNamespace,
                                    const int minApiLevel, InterfaceApi interfaceApi,
                                    bool includeExtraSrcs) {
    write_native_header_preamble(out, atoms, cppNamespace, interfaceApi, includeExtraSrcs);

    if (has_attribution_node(atoms.decls)) {
        fprintf(out, "struct AttributionNode final {\n");
        fprintf(out, "  AttributionNode(int32_t aUid, const std::string& aTag)\n");
        fprintf(out, "    : uid(aUid), tag(aTag) {}\n");
        fprintf(out, "  int32_t uid;\n");
        fprintf(out, "  std::string tag;\n");
        fprintf(out, "};\n\n");
    }

    // Print Atom classes definition
    fprintf(out, "//\n");
    fprintf(out, "// Atom definitions\n");
    fprintf(out, "//\n");

    if (write_native_atom_types(out, atoms, "stats_write") != 0) {
        return 1;
    };

    // Print write methods
    fprintf(out, "//\n");
    fprintf(out, "// Write methods\n");
    fprintf(out, "//\n");

    for (auto& atomDecl : atoms.decls) {
        const string closer = contains_repeated_field(atomDecl->fields)
                                      ? " __INTRODUCED_IN(__ANDROID_API_T__)"
                                      : "";

        if (atomDecl->atomType == ATOM_TYPE_PUSHED) {
            fprintf(out, "int stats_write(const %s& atom)%s;\n", atomDecl->message.c_str(),
                    closer.c_str());
        } else {
            fprintf(out, "void addAStatsEvent(AStatsEventList* pulled_data, const %s& atom)%s;\n",
                    atomDecl->message.c_str(), closer.c_str());
        }
    }

    write_native_header_epilogue(out, cppNamespace);

    (void)minApiLevel;
    return 0;
}

}  // namespace stats_log_api_gen
}  // namespace android
