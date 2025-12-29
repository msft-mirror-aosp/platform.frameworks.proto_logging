/*
 * Copyright (C) 2025, The Android Open Source Project
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

#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/stubs/common.h>

//#include <stdlib.h>
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <stdio.h>
#include <string.h>
#include <string>

#include "absl/strings/match.h"

#include "Collation.h"
#include "frameworks/proto_logging/stats/atoms.pb.h"
#include "frameworks/proto_logging/stats/attribution_node.pb.h"
#include "java_writer.h"
#include "native_writer.h"
#include "rust_writer.h"
#include "utils.h"

#ifdef WITH_VENDOR
#include "java_writer_vendor.h"
#include "native_writer_vendor.h"
#endif

namespace android {
namespace stats_log_api_gen {

namespace fs = std::filesystem;
using android::os::statsd::Atom;

enum class InterfaceApi : uint8_t{
    INVALID,
    PLATFORM,
    VENDOR,
    BOOTSTRAP,
};

static InterfaceApi string_to_interface_api(const std::string& value) {
    std::string upper_value = value;
    std::transform(upper_value.begin(), upper_value.end(), upper_value.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    if (upper_value == "VENDOR") {
        return InterfaceApi::VENDOR;
    }
    if (upper_value == "BOOTSTRAP") {
        return InterfaceApi::BOOTSTRAP;
    }
    if (upper_value == "PLATFORM") {
        return InterfaceApi::PLATFORM;
    }
    return InterfaceApi::INVALID;
}

static void print_usage() {
    fprintf(stderr, "usage: stats-log-api-gen OPTIONS\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "OPTIONS\n");
    fprintf(stderr, "  --cpp FILENAME       the cpp file to output for write helpers\n");
    fprintf(stderr, "  --header FILENAME    the header file to output for write helpers\n");
    fprintf(stderr, "  --help               this message\n");
    fprintf(stderr, "  --java FILENAME      the java file to output\n");
    fprintf(stderr, "  --rust FILENAME      the rust file to output\n");
    fprintf(stderr,
            "  --rustHeader FILENAME the rust file to output for write helpers. "
            "Not needed/supported for --vendor-proto\n");
    fprintf(stderr,
            "  --rustHeaderCrate NAME        header crate to be used while "
            "generating the code. Note: this should be the same as the crate_name "
            "created by rust_library for the header. Not needed for --vendor-proto\n");
    fprintf(stderr, "  --module NAME        optional, module name to generate outputs for\n");
    fprintf(stderr,
            "  --namespace COMMA,SEP,NAMESPACE or COLON::SEP::NAMESPACE   required for cpp/header"
            " with module\n");
    fprintf(stderr,
            "                                                             comma or double-colon "
            "separated namespace of the files\n");
    fprintf(stderr,
            "  --importHeader NAME  required for cpp/jni to say which header to "
            "import for write helpers\n");
    fprintf(stderr, "  --javaPackage PACKAGE             the package for the java file.\n");
    fprintf(stderr, "                                    required for java with module\n");
    fprintf(stderr, "  --javaClass CLASS    the class name of the java class.\n");
    fprintf(stderr, "  --nonStatic          generate java classes with non-static methods\n");
    fprintf(stderr, "  --minApiLevel API_LEVEL           lowest API level to support.\n");
    fprintf(stderr, "                                    Default is \"current\".\n");
    fprintf(stderr,
            "  --worksource         Include support for logging WorkSource "
            "objects.\n");
    fprintf(stderr, "                                        Default is \"current\".\n");
    fprintf(stderr, "  --interface          The code gen API to use.\n"
            " Supported APIs are platform (default), vendor or bootstrap\n"
            " Bootstrap only supported for cpp. Do not use unless necessary.\n");
#ifdef WITH_VENDOR
    fprintf(stderr,
            "  --vendor-proto       Path to the proto file for vendor atoms logging\n"
            "code generation (deprecated, use \"--interface VENDOR --proto <path>\" instead).\n");
#endif
    fprintf(stderr, "  --proto       Path to proto files for atoms logging code generation.\n");
    fprintf(stderr, "                Can be specified multiple times to include extension\n");
    fprintf(stderr, "                files. Only one file must define the Atom proto.\n");
    fprintf(stderr, "  --omitExtraSrcs       Exclude extra srcs.\n");
}

static const Descriptor* load_protos_and_find_atom_descriptor(
        google::protobuf::compiler::Importer& importer, const vector<string>& protos) {
    const Descriptor* atomDescriptor = nullptr;
    for (const string& proto : protos) {
        if (absl::StartsWith(proto, "external/protobuf/src")) {
            continue;
        }
        const google::protobuf::FileDescriptor* cur = importer.Import(proto);
        if (cur == nullptr) {
            return nullptr;
        }
        const Descriptor* curAtomDescriptor = cur->FindMessageTypeByName("Atom");
        if (curAtomDescriptor != nullptr) {
            if (atomDescriptor == nullptr) {
                atomDescriptor = curAtomDescriptor;
            } else {
                fprintf(stderr, "Error: Multiple files contain Atom protos\n");
                return nullptr;
            }
        }
    }

    if (atomDescriptor == nullptr) {
        fprintf(stderr, "Error: Atom message not found in any --proto file\n");
        return nullptr;
    }
    return atomDescriptor;
}

static int collate_atoms_from_sources(const string& moduleName, const vector<string>& protos,
                                      Atoms& atoms) {
    if (protos.empty()) {
        return collate_atoms(*Atom::descriptor(), moduleName, atoms);
    }

    MFErrorCollector errorCollector;
    google::protobuf::compiler::DiskSourceTree sourceTree;
    google::protobuf::compiler::Importer importer(&sourceTree, &errorCollector);
    sourceTree.MapPath("", fs::current_path().c_str());

    const char* androidBuildTop = std::getenv("ANDROID_BUILD_TOP");

    fs::path protobufSrc = androidBuildTop != nullptr ? androidBuildTop : fs::current_path();
    protobufSrc /= "external/protobuf/src";
    sourceTree.MapPath("", protobufSrc.c_str());

    if (androidBuildTop != nullptr) {
        sourceTree.MapPath("", androidBuildTop);
    }

    const Descriptor* atomDescriptor = load_protos_and_find_atom_descriptor(importer, protos);
    if (atomDescriptor == nullptr) {
        return 1;
    }
    return collate_atoms(*atomDescriptor, moduleName, atoms);
}

/**
 * Do the argument parsing and execute the tasks.
 */
static int run(int argc, char const* const* argv) {
    string cppFilename;
    string headerFilename;
    string javaFilename;
    string javaPackage;
    string javaClass;
    string rustFilename;
    string rustHeaderFilename;
    string rustHeaderCrate;
    string moduleName = DEFAULT_MODULE_NAME;
    string cppNamespace = DEFAULT_CPP_NAMESPACE;
    string cppHeaderImport = DEFAULT_CPP_HEADER_IMPORT;
    vector<string> protos;
    InterfaceApi interface = InterfaceApi::PLATFORM;
    bool supportWorkSource = false;
    int minApiLevel = API_LEVEL_CURRENT;
    bool javaStaticMethods = true;
    bool typeSafe = false;
    bool includeExtraSrcs = true;

    int index = 1;
    while (index < argc) {
        if (0 == strcmp("--help", argv[index])) {
            print_usage();
            return 0;
        } else if (0 == strcmp("--cpp", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            cppFilename = argv[index];
        } else if (0 == strcmp("--header", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            headerFilename = argv[index];
        } else if (0 == strcmp("--java", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            javaFilename = argv[index];
        } else if (0 == strcmp("--rust", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            rustFilename = argv[index];
        } else if (0 == strcmp("--rustHeader", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            rustHeaderFilename = argv[index];
        } else if (0 == strcmp("--rustHeaderCrate", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            rustHeaderCrate = argv[index];
        } else if (0 == strcmp("--module", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            moduleName = argv[index];
        } else if (0 == strcmp("--namespace", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            cppNamespace = argv[index];
        } else if (0 == strcmp("--importHeader", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            cppHeaderImport = argv[index];
        } else if (0 == strcmp("--javaPackage", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            javaPackage = argv[index];
        } else if (0 == strcmp("--javaClass", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            javaClass = argv[index];
        } else if (0 == strcmp("--nonStatic", argv[index])) {
            javaStaticMethods = false;
        } else if (0 == strcmp("--supportQ", argv[index])) {
            minApiLevel = API_Q;
        } else if (0 == strcmp("--worksource", argv[index])) {
            supportWorkSource = true;
        } else if (0 == strcmp("--minApiLevel", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            if (0 != strcmp("current", argv[index])) {
                minApiLevel = atoi(argv[index]);
            }
#ifdef WITH_VENDOR
        } else if (0 == strcmp("--vendor-proto", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            interface = InterfaceApi::VENDOR;
            protos.push_back(argv[index]);
#endif
        } else if (0 == strcmp("--interface", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }

            interface = string_to_interface_api(argv[index]);
            if (interface == InterfaceApi::INVALID) {
                fprintf(stderr, "Error: Invalid value for --interface: %s\n", argv[index]);
                print_usage();
                return 1;
            }
        } else if (0 == strcmp("--proto", argv[index])) {
            index++;
            if (index >= argc) {
                print_usage();
                return 1;
            }
            protos.push_back(argv[index]);
            index++;
            while (index < argc) {
                if (0 == strncmp(argv[index], "--", 2)) {
                    index--;
                    break;
                }
                protos.push_back(argv[index]);
                index++;
            }
        } else if (0 == strcmp("--type-safe", argv[index])) {
            typeSafe = true;
        } else if (0 == strcmp("--omitExtraSrcs", argv[index])) {
            includeExtraSrcs = false;
        }

        index++;
    }

    if (index < argc) {
        fprintf(stderr, "Error: Unknown command line argument\n");
        print_usage();
        return 1;
    }

    if (cppFilename.empty() && headerFilename.empty() && javaFilename.empty() &&
        rustFilename.empty() && rustHeaderFilename.empty()) {
        print_usage();
        return 1;
    }
    if (DEFAULT_MODULE_NAME == moduleName && minApiLevel != API_LEVEL_CURRENT) {
        // Default module only supports current API level.
        fprintf(stderr, "%s cannot support older API levels\n", moduleName.c_str());
        return 1;
    }

    if (minApiLevel < API_Q) {
        // Cannot support pre-Q.
        fprintf(stderr, "minApiLevel must be %d or higher.\n", API_Q);
        return 1;
    }

    if (interface == InterfaceApi::BOOTSTRAP) {
        if (cppFilename.empty() && headerFilename.empty()) {
            fprintf(stderr, "Bootstrap flag can only be used for cpp/header files.\n");
            return 1;
        }
        if (supportWorkSource) {
            fprintf(stderr, "Bootstrap flag does not support worksources");
            return 1;
        }
        if (minApiLevel != API_LEVEL_CURRENT) {
            fprintf(stderr, "Bootstrap flag does not support older API levels");
            return 1;
        }
    }

    // Collate the parameters.
    Atoms atoms;
    int errorCount = collate_atoms_from_sources(moduleName, protos, atoms);

    if (errorCount != 0) {
        return 1;
    }

    AtomDecl attributionDecl;
    vector<java_type_t> attributionSignature;
    collate_atom(*android::os::statsd::AttributionNode::descriptor(), attributionDecl,
                 attributionSignature);

    const bool isVendor = interface == InterfaceApi::VENDOR;

    replace_all(cppNamespace, "::", ",");

    // Write the .cpp file
    if (!cppFilename.empty()) {
        // If this is for a specific module, the namespace must also be provided.
        if (moduleName != DEFAULT_MODULE_NAME && cppNamespace == DEFAULT_CPP_NAMESPACE) {
            fprintf(stderr, "Must supply --namespace if supplying a specific module\n");
            return 1;
        }
        // If this is for a specific module, the header file to import must also be
        // provided.
        if (moduleName != DEFAULT_MODULE_NAME && cppHeaderImport == DEFAULT_CPP_HEADER_IMPORT) {
            fprintf(stderr, "Must supply --headerImport if supplying a specific module\n");
            return 1;
        }
        FILE* out = fopen(cppFilename.c_str(), "we");
        if (out == nullptr) {
            fprintf(stderr, "Unable to open file for write: %s\n", cppFilename.c_str());
            return 1;
        }
        if (!isVendor) {
            if (typeSafe) {
                errorCount = android::stats_log_api_gen::write_stats_log_cpp_typesafe(
                        out, atoms, attributionDecl, cppNamespace, cppHeaderImport, minApiLevel,
                        interface == InterfaceApi::BOOTSTRAP, includeExtraSrcs);

            } else {
                errorCount = android::stats_log_api_gen::write_stats_log_cpp(
                        out, atoms, attributionDecl, cppNamespace, cppHeaderImport, minApiLevel,
                        interface == InterfaceApi::BOOTSTRAP, includeExtraSrcs);
            }

#ifdef WITH_VENDOR
        } else {
            if (typeSafe) {
                errorCount = android::stats_log_api_gen::write_stats_log_cpp_vendor_typesafe(
                        out, atoms, attributionDecl, cppNamespace, cppHeaderImport,
                        includeExtraSrcs);
            } else {
                errorCount = android::stats_log_api_gen::write_stats_log_cpp_vendor(
                        out, atoms, attributionDecl, cppNamespace, cppHeaderImport,
                        includeExtraSrcs);
            }
#endif
        }
        fclose(out);
    }

    // Write the .h file
    if (!headerFilename.empty()) {
        // If this is for a specific module, the namespace must also be provided.
        if (moduleName != DEFAULT_MODULE_NAME && cppNamespace == DEFAULT_CPP_NAMESPACE) {
            fprintf(stderr, "Must supply --namespace if supplying a specific module\n");
        }
        FILE* out = fopen(headerFilename.c_str(), "we");
        if (out == nullptr) {
            fprintf(stderr, "Unable to open file for write: %s\n", headerFilename.c_str());
            return 1;
        }
        if (!isVendor) {
            if (typeSafe) {
                errorCount = android::stats_log_api_gen::write_stats_log_header_typesafe(
                        out, atoms, attributionDecl, cppNamespace, minApiLevel,
                        interface == InterfaceApi::BOOTSTRAP, includeExtraSrcs);
            } else {
                errorCount = android::stats_log_api_gen::write_stats_log_header(
                        out, atoms, attributionDecl, cppNamespace, minApiLevel,
                        interface == InterfaceApi::BOOTSTRAP, includeExtraSrcs);
            }
#ifdef WITH_VENDOR
        } else {
            if (typeSafe) {
                errorCount = android::stats_log_api_gen::write_stats_log_header_vendor_typesafe(
                        out, atoms, attributionDecl, cppNamespace, includeExtraSrcs);
            } else {
                errorCount = android::stats_log_api_gen::write_stats_log_header_vendor(
                        out, atoms, attributionDecl, cppNamespace, includeExtraSrcs);
            }
#endif
        }
        fclose(out);
    }

    // Write the .java file
    if (!javaFilename.empty()) {
        if (javaClass.empty()) {
            fprintf(stderr, "Must supply --javaClass if supplying a Java filename");
            return 1;
        }

        if (javaPackage.empty()) {
            fprintf(stderr, "Must supply --javaPackage if supplying a Java filename");
            return 1;
        }

        if (moduleName.empty() || moduleName == DEFAULT_MODULE_NAME) {
            fprintf(stderr, "Must supply --module if supplying a Java filename");
            return 1;
        }

        FILE* out = fopen(javaFilename.c_str(), "we");
        if (out == nullptr) {
            fprintf(stderr, "Unable to open file for write: %s\n", javaFilename.c_str());
            return 1;
        }

        if (!isVendor) {
            errorCount = android::stats_log_api_gen::write_stats_log_java(
                    out, atoms, attributionDecl, javaClass, javaPackage, minApiLevel,
                    supportWorkSource, javaStaticMethods, includeExtraSrcs);
#ifdef WITH_VENDOR
        } else {
            if (supportWorkSource) {
                fprintf(stderr, "The attribution chain is not supported for vendor atoms");
                return 1;
            }

            errorCount = android::stats_log_api_gen::write_stats_log_java_vendor(
                    out, atoms, javaClass, javaPackage, javaStaticMethods, includeExtraSrcs);
#endif
        }

        fclose(out);
    }

    // Write the main .rs file
    if (!rustFilename.empty()) {
        if (rustHeaderCrate.empty() && !isVendor) {
            fprintf(stderr, "rustHeaderCrate flag is either not passed or is empty\n");
            return 1;
        }

        FILE* out = fopen(rustFilename.c_str(), "we");
        if (out == nullptr) {
            fprintf(stderr, "Unable to open file for write: %s\n", rustFilename.c_str());
            return 1;
        }

        if (!isVendor) {
            errorCount += android::stats_log_api_gen::write_stats_log_rust(
                    out, atoms, attributionDecl, minApiLevel, rustHeaderCrate.c_str());
#ifdef WITH_VENDOR
        } else {
            errorCount += android::stats_log_api_gen::write_stats_log_rust_vendor(out, atoms,
                                                                                  attributionDecl);
#endif
        }

        fclose(out);
    }

    // Write the header .rs file
    if (!rustHeaderFilename.empty()) {
#ifdef WITH_VENDOR
        if (isVendor) {
            fprintf(stderr, "rustHeaderFilename is not needed for vendor proto\n");
            return 1;
        }
#endif
        if (rustHeaderCrate.empty()) {
            fprintf(stderr, "rustHeaderCrate flag is either not passed or is empty");
            return 1;
        }

        FILE* out = fopen(rustHeaderFilename.c_str(), "we");
        if (out == nullptr) {
            fprintf(stderr, "Unable to open file for write: %s\n", rustHeaderFilename.c_str());
            return 1;
        }

        android::stats_log_api_gen::write_stats_log_rust_header(out, atoms, attributionDecl,
                                                                rustHeaderCrate.c_str());

        fclose(out);
    }

    return errorCount;
}

}  // namespace stats_log_api_gen
}  // namespace android

/**
 * Main.
 */
int main(int argc, char const* const* argv) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    return android::stats_log_api_gen::run(argc, argv);
}
