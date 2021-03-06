#!/usr/bin/env python

"""
Set up a module by
- checking that all required modules and dependencies are found
- creating the config header and .cpp and version check .py
- linking .py files from the bin and benchmarks directories into the build dir
- Linking Python apps into the build dir.

If the module cannot be configured, the script exits with an error.
"""

import sys
from optparse import OptionParser
import os.path
import tools
import glob
import pickle
import re

parser = OptionParser()
parser.add_option("-D", "--defines", dest="defines", default="",
                  help="Colon separated list of defines.")
parser.add_option("-n", "--name",
                  dest="name", help="The name of the module.")
parser.add_option("-s", "--source",
                  dest="source", help="The root for IMP source.")
parser.add_option("-d", "--datapath",
                  dest="datapath", default="", help="An extra IMP datapath.")


def add_list_to_defines(cppdefines, data, sym, val, names):
    for n in sorted(names):
        nn = n.replace(".", "_").upper()
        cppdefines.append("#define IMP_%s_%s_%s"
                          % (data["name"].upper(), sym, nn))
        cppdefines.append("#define IMP_%s_HAS_%s %d"
                          % (data["name"].upper(), nn, val))


def make_header(options):
    if options.name == 'kernel':
        dir = os.path.join("include", "IMP")
    else:
        dir = os.path.join("include", "IMP", options.name)
    file = os.path.join(dir, "%s_config.h" % options.name)
    header_template = tools.CPPFileGenerator(os.path.join(options.source,
                            "tools", "build", "config_templates", "header.h"))
    try:
        os.makedirs(dir)
    except:
        # exists
        pass

    data = {}
    data["name"] = options.name
    if options.name == 'kernel':
        data["namespace"] = "IMP"
        data["begin_ns"] = "namespace IMP{"
        data["end_ns"] = "}"
        data["filename"] = "IMP/%s_config.h" % options.name
    else:
        data["namespace"] = "IMP::%s" % options.name
        data["begin_ns"] = "namespace IMP{ namespace %s {" % options.name
        data["end_ns"] = "} }"
        data["filename"] = "IMP/%s/%s_config.h" % (options.name, options.name)
    data["cppprefix"] = "IMP%s" % options.name.upper().replace("_", "")
    if data["name"] != "kernel":
        data["showable"] = """#if !defined(IMP_DOXYGEN) && !defined(SWIG)

#include <IMP/Showable.h>
#include <IMP/hash.h>

%(begin_ns)s
using ::IMP::Showable;
using ::IMP::operator<<;
using ::IMP::hash_value;
%(end_ns)s // namespace
%(begin_ns)s namespace internal {
using ::IMP::Showable;
using ::IMP::operator<<;
using ::IMP::hash_value;
} %(end_ns)s // namespace

#endif // !defined(SWIG) && !defined(IMP_DOXYGEN)
""" % data
    else:
        data["showable"] = ""
    cppdefines = []
    if options.defines != "":
        for define in tools.split(options.defines):
            parts = define.split("=")
            if len(parts) == 2:
                cppdefines.append("#define %s %s" % (parts[0], parts[1]))
            else:
                cppdefines.append("#define %s" % parts[0])

    d = {'required_modules': "", 'lib_only_required_modules': "",
         'required_dependencies': "", 'optional_dependencies': ""}
    exec(open(os.path.join(options.source, "modules", data["name"],
                           "dependencies.py"), "r").read(), d)

    info = tools.get_module_info(data["name"], options.datapath)

    optional_modules = [x for x in info["modules"]
                        if x not in tools.split(d['required_modules'])
                             and x != ""]
    unfound_modules = [x for x in info["unfound_modules"] if x != ""]
    optional_dependencies = [x for x in info["dependencies"]
                             if x not in tools.split(d['required_dependencies'])
                                  and x != ""]
    unfound_dependencies = [x for x in info["unfound_dependencies"] if x != ""]
    add_list_to_defines(cppdefines, data, "USE", 1,
                        ["imp_" + x for x in optional_modules])
    add_list_to_defines(cppdefines, data, "NO", 0,
                        ["imp_" + x for x in unfound_modules])
    add_list_to_defines(cppdefines, data, "USE", 1, optional_dependencies)
    add_list_to_defines(cppdefines, data, "NO", 0, info["unfound_dependencies"])
    data["cppdefines"] = "\n".join(cppdefines)
    header_template.write(file, data)

class ModuleDoxFileGenerator(tools.FileGenerator):
    def __init__(self, template_file, modules):
        self.modules = modules
        tools.FileGenerator.__init__(self, template_file, '#')

    def get_output_file_contents(self, options):
        template = self.template
        name = options.name
        modules = self.modules
        template = template.replace("@IMP_SOURCE_PATH@", options.source)
        template = template.replace("@VERSION@", "NONE")
        template = template.replace("@NAME@", name)
        template = template.replace("@PROJECT_BRIEF@",
                                    '"The Integrative Modeling Platform"')
        template = template.replace("@RECURSIVE@", "YES")
        template = template.replace("@EXCLUDE_PATTERNS@", "*/tutorial/*")
        template = template.replace("@IS_HTML@", "NO")
        template = template.replace("@IS_XML@", "YES")
        template = template.replace("@PROJECT_NAME@", "IMP." + name)
        template = template.replace("@HTML_OUTPUT@", "../../doc/html/" + name)
        template = template.replace("@XML_OUTPUT@", "xml")
        template = template.replace("@TREEVIEW@", "NO")
        template = template.replace("@GENERATE_TAGFILE@", "tags")
        template = template.replace("@LAYOUT_FILE@",
                          "%s/doc/doxygen/module_layout.xml" % options.source)
        template = template.replace("@MAINPAGE@", "README.md")
        template = template.replace("@INCLUDE_PATH@", "include")
        template = template.replace("@FILE_PATTERNS@",
                                    "*.cpp *.h *.py *.md *.dox")
        template = template.replace("@WARNINGS@", "warnings.txt")
        # include lib and doxygen in input
        inputs = []
        if options.name == "kernel":
            inputs.append("lib/IMP/")
            inputs.append("include/IMP/")
            exclude = ["include/IMP/%s include/IMP/%s.h lib/IMP/%s" % (m, m, m)
                       for m, g in tools.get_modules(options.source)]
            exclude.append("include/IMP/base include/IMP/base.h lib/IMP/base")
            template = template.replace("@EXCLUDE@",
                               " \\\n                         ".join(exclude))
        else:
            template = template.replace("@EXCLUDE@", "")
            inputs.append("include/IMP/" + options.name)
            inputs.append("lib/IMP/" + options.name)
        inputs.append("examples/" + options.name)
        # suppress a warning since git removes empty dirs and doxygen
        # gets confused if the input path doesn't exist
        docpath = os.path.join(options.source, "modules", options.name, "doc")
        if os.path.exists(docpath):
            inputs.append(docpath)
        # overview for module
        inputs.append("../generated/IMP_%s.dox" % options.name)
        template = template.replace("@INPUT_PATH@",
                                " \\\n                         ".join(inputs))
        tags = [os.path.join(options.source, 'doc', 'doxygen',
                             'dummy_module_tags.xml')]
        for m in modules:
            tags.append(os.path.join("../", m, "tags") + "=" + "../" + m)
        template = template.replace("@TAGS@",
                                 " \\\n                         ".join(tags))
        if options.name == "example":
            template = template.replace("@EXAMPLE_PATH@",
                         "examples/example %s/modules/example" % options.source)
        else:
            template = template.replace("@EXAMPLE_PATH@",
                                       "examples/" + options.name)
        return template

def make_doxygen(options, modules):
    file = os.path.join("doxygen", options.name, "Doxyfile")
    name = options.name
    g = ModuleDoxFileGenerator(os.path.join(options.source, "tools", "build",
                                            "doxygen_templates", "Doxyfile.in"),
                               modules)
    g.write(file, options)

def write_no_ok(module):
    new_order = [x for x in tools.get_sorted_order() if x != module]
    tools.set_sorted_order(new_order)
    tools.rewrite(os.path.join("data", "build_info", "IMP." + module),
                  "ok=False\n", verbose=False)

def write_ok(module, modules, unfound_modules, dependencies,
             unfound_dependencies, swig_includes, swig_wrapper_includes):
    print("yes")
    config = ["ok=True"]
    vardict = locals()
    for varname in ("modules", "unfound_modules", "dependencies",
                    "unfound_dependencies", "swig_includes",
                    "swig_wrapper_includes"):
        var = vardict[varname]
        if len(var) > 0:
            config.append("%s = %s" % (varname, repr(":".join(var))))
    tools.rewrite(os.path.join("data", "build_info", "IMP." + module),
                   "\n".join(config))

def setup_module(module, source, datapath):
    sys.stdout.write("Configuring module %s ..." % module)
    data = tools.get_module_description(source, module, datapath)
    for d in data["required_dependencies"]:
        if not tools.get_dependency_info(d, datapath)["ok"]:
            print("Required dependency %s not found" % d)
            write_no_ok(module)
            return False, []
    dependencies = data["required_dependencies"]
    unfound_dependencies = []
    for d in data["optional_dependencies"]:
        if tools.get_dependency_info(d, datapath)["ok"]:
            dependencies.append(d)
        else:
            unfound_dependencies.append(d)
    for d in data["required_modules"]:
        if not tools.get_module_info(d, datapath)["ok"]:
            print("Required module IMP.%s not available" % d)
            write_no_ok(module)
            return False, []
    modules = data["required_modules"]
    unfound_modules = []
    for d in data["optional_modules"]:
        if tools.get_module_info(d, datapath)["ok"]:
            modules.append(d)
        else:
            unfound_modules.append(d)
    all_modules = tools.get_dependent_modules(modules, datapath)
    moddir = os.path.join('IMP', '' if module == 'kernel' else module)
    swig_includes = [os.path.split(x)[1]
                     for x in tools.get_glob(
                         [os.path.join(source, "modules", module, "pyext",
                                       "include", "*.i")])] \
                    + [os.path.join(moddir, os.path.split(x)[1])
                       for x in tools.get_glob(
                           [os.path.join("include", moddir, "*_macros.h")])]
    swig_wrapper_includes = [os.path.join(moddir, "internal",
                                          os.path.split(x)[1])
                             for x in tools.get_glob(
                                 [os.path.join(source, "modules", module,
                                               "include", "internal",
                                               "swig*.h")])]
    tools.mkdir(os.path.join("src", module))
    tools.mkdir(os.path.join("src", module + "_swig"))
    write_ok(module, all_modules, unfound_modules,
             tools.get_dependent_dependencies(all_modules, dependencies,
                                              datapath),
             unfound_dependencies, swig_includes, swig_wrapper_includes)
    return True, all_modules


def link_py_apps(options):
    path = os.path.join(options.source, "modules", options.name, "bin")
    tools.mkdir("bin", clean=False)
    tools.link_dir(path, "bin", clean=False, match=["*"],
                   filt=tools.filter_pyapps)

def link_bin(options):
    path = os.path.join("module_bin", options.name)
    tools.mkdir(path, clean=False)
    for old in tools.get_glob([os.path.join(path, "*.py")]):
        os.unlink(old)
    tools.link_dir(os.path.join(options.source, "modules", options.name, "bin"),
                   path, clean=False, match=["*.py"])

def link_benchmark(options):
    path = os.path.join("benchmark", options.name)
    tools.mkdir(path, clean=False)
    for old in tools.get_glob([os.path.join(path, "*.py")]):
        os.unlink(old)
    tools.link_dir(os.path.join(options.source, "modules", options.name,
                                "benchmark"),
                   path, clean=False, match=["*.py"])

def find_cmdline_links(mod, docdir, cmdline_tools):
    """Look for (sub)sections in the .dox or .md docs for each cmdline tool,
       and return a mapping from tool name to (doxygen link, brief desc, num)"""
    links = dict.fromkeys(cmdline_tools)
    num = 0
    todo = {}
    docre = re.compile(r'\\(subsection|section|page)\s+(\S+)\s+(\S+):\s*(.*)$')
    docre_sep = re.compile(r'\\(subsection|section|page)\s+(\S+)\s+(\S+)\s*$')
    mdre = re.compile('#*\s*(\S+):\s*([^#]+)#*\s*{#(\S+)}')
    mdre_sep = re.compile('#*\s*(\S+)\s*#*\s*{#(\S+)}')
    for g in [os.path.join(docdir, "README.md")] \
             + glob.glob(os.path.join(docdir, "doc", "*.dox")) \
             + glob.glob(os.path.join(docdir, "doc", "*.md")):
        for line in tools.open_utf8(g):
            if todo and len(line.rstrip('\r\n ')) > 0 \
               and line[0] not in " =-\\":
                (k, v) = todo.popitem()
                links[k] = (v, line.rstrip('\r\n '), num)
                num += 1
                todo = {}
            m = docre.search(line)
            if m and m.group(3) in links:
                links[m.group(3)] = (m.group(2), m.group(4), num)
                num += 1
            m = docre_sep.search(line)
            if m and m.group(3) in links:
                todo = {m.group(3): m.group(2)}
            m = mdre.search(line)
            if m and m.group(1) in links:
                links[m.group(1)] = (m.group(3), m.group(2), num)
                num += 1
            m = mdre_sep.search(line)
            if m and m.group(1) in links:
                todo = {m.group(1): m.group(2)}
    missing_links = [tool for tool, link in links.items() if link is None]
    if missing_links:
        print("""
Could not find section title for command line tool %s
in IMP.%s docs.

Each command line tool should have a section or page in the documentation
(in %s/README.md or
%s/doc/*.{dox,md})
that describes it. The section title should contain the tool's name and a
brief description (separated by a colon), followed by a unique doxygen ID.
Alternatively, the brief description can be given in the body immediately
following the title. For example, the tool do_foo.py could be documented with

\section do_foo_bin do_foo.py: Do something with foo

or

\section do_foo_bin do_foo.py
Do something with foo

in doxygen (\subsection or \page can also be used) or

doo_foo.py: Do something with foo {#do_foo_bin}
=================================

or

# doo_foo.py: Do something with foo {#do_foo_bin}

or

# doo_foo.py {#do_foo_bin}
Do something with foo

in Markdown.
""" % (", ".join(missing_links), mod, docdir, docdir))
        sys.exit(1)
    return links

def make_overview(options, cmdline_tools):
    docdir = os.path.join(options.source, "modules", options.name)
    cmdline_links = find_cmdline_links(options.name, docdir, cmdline_tools)
    pickle.dump(cmdline_links,
                open(os.path.join("data", "build_info",
                                  "IMP_%s.pck" % options.name), 'wb'), -1)
    rmd = tools.open_utf8(os.path.join(options.source, "modules", options.name,
                                       "README.md"), "r").read()
    tools.rewrite(
        os.path.join("doxygen", "generated", "IMP_%s.dox" % options.name),
                  """/** \\namespace %s
\\tableofcontents

%s
*/
""" % ('IMP' if options.name == 'kernel' else 'IMP::' + options.name, rmd))


def main():
    options, apps = parser.parse_args()
    disabled = tools.split(open("data/build_info/disabled", "r").read(), "\n")
    if options.name in disabled:
        print("%s is disabled" % options.name)
        write_no_ok(options.name)
        tools.rmdir(os.path.join("module_bin", options.name))
        tools.rmdir(os.path.join("benchmark", options.name))
        tools.rmdir(os.path.join("lib", "IMP", options.name))
        sys.exit(1)
    success, modules = setup_module(options.name, options.source,
                                    options.datapath)
    if success:
        make_header(options)
        make_doxygen(options, modules)
        make_overview(options, apps)
        link_bin(options)
        link_py_apps(options)
        link_benchmark(options)
        sys.exit(0)
    else:
        tools.rmdir(os.path.join("module_bin", options.name))
        tools.rmdir(os.path.join("benchmark", options.name))
        tools.rmdir(os.path.join("lib", "IMP", options.name))
        sys.exit(1)

if __name__ == '__main__':
    main()
