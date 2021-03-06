#!/usr/bin/env python

"""
Set up the build environment:
- remove .pyc files from the source dir
- write the list of disabled modules (probably should go elsewhere)
- sort the modules based inter-module dependencies
- link module headers in the include dir
- link examples in the example dir
- link doxygen files into the doxygen dir
- link swig files into the swig dir
- link python source into the lib/IMP dir
- create pages that list things in the documentation
- setup the doxygen config files
- generate standards tests

No repository directories are changed.
"""

import os
import sys
import os.path
import shutil
import platform
import tools
from optparse import OptionParser

# main loops


# link all the headers from the module/include directories into the
# correct place in the build dir
def link_headers(source):
    target = os.path.join("include")
    tools.mkdir(target)
    root = os.path.join(target, "IMP")
    tools.mkdir(root)
    for (module, g) in tools.get_modules(source):
        # print g, module
        modroot = os.path.join(root, '' if module == 'kernel' else module)
        if module == "SConscript":
            continue
        tools.link_dir(os.path.join(g, "include"), modroot, match=["*.h"])
        tools.link_dir(os.path.join(g, "include", "internal"),
                       os.path.join(modroot, "internal"), match=["*.h"])
        # ick
        if os.path.exists(os.path.join(g, "include", "eigen3")):
            tools.link_dir(os.path.join(g, "include", "eigen3"),
                           os.path.join(modroot, "eigen3"),
                           match=["*"])

# link example scripts and data from the source dirs into the build tree


def link_examples(source):
    target = os.path.join("doc", "examples")
    tools.mkdir(target)
    for module, g in tools.get_modules(source):
        tools.link_dir(
            os.path.join(g, "examples"), os.path.join(target, module))

# link files from the module/data directries from the source into the
# build tree


def link_data(source):
    target = os.path.join("data")
    tools.mkdir(target)
    for module, g in tools.get_modules(source):
        tools.link_dir(os.path.join(g, "data"), os.path.join(target, module))

# link swig .i files from the source into the build tree


def link_swig(source):
    target = os.path.join("swig")
    tools.mkdir(target)
    for module, g in tools.get_modules(source):
        # they all go in the same dir, so don't remove old links
        tools.link_dir(
            os.path.join(g,
                         "pyext"),
            target,
            match=["*.i"],
            clean=False)
        if os.path.exists(os.path.join(g, "pyext", "include")):
            tools.link_dir(
                os.path.join(g,
                             "pyext",
                             "include"),
                target,
                match=["*.i"],
                clean=False)
        tools.link(
            os.path.join(
                g,
                "pyext",
                "swig.i-in"),
            os.path.join(
                target,
                "IMP_%s.impl.i" %
                module))

# link python source files from pyext/src into the build tree


def link_python(source):
    target = os.path.join("lib")
    tools.mkdir(target, clean=False)
    for module, g in tools.get_modules(source):
        path = os.path.join(target, "IMP", '' if module == 'kernel' else module)
        tools.mkdir(path, clean=False)
        for old in tools.get_glob([os.path.join(path, "*.py")]):
            # don't unlink the generated file
            if os.path.split(old)[1] != "__init__.py" and os.path.split(old)[1] != "_version_check.py":
                os.unlink(old)
                # print "linking", path
        tools.link_dir(os.path.join(g, "pyext", "src"), path, clean=False,
                       make_subdirs=True)


def _make_test_driver(outf, cpps):
    out = open(outf, "w")
    out.write("""import IMP
import IMP.test
import sys
try:
    import subprocess
except ImportError:
    subprocess = None

class TestCppProgram(IMP.test.TestCase):
""")
    for t in cpps:
        tbase = os.path.splitext(t)[0]
        # remove suffix
        nm = os.path.split(str(tbase))[1].replace(".", "_")
        # Strip .exe extension, so test name on Windows matches other platforms
        exename = os.path.join(os.path.split(outf)[0], os.path.split(tbase)[1])
        if platform.system == "Windows":
            exename = exename + ".exe"
        out.write(
"""    def test_%(name)s(self):
        \"\"\"Running C++ test %(name)s\"\"\"
        if subprocess is None:
            self.skipTest("subprocess module unavailable")
        # Note: Windows binaries look for needed DLLs in the current
        # directory. So we need to change into the directory where the DLLs have
        # been installed for the binary to load correctly.
        p = subprocess.Popen(["%(path)s"],
                             shell=False, cwd="%(libdir)s")
        self.assertEqual(p.wait(), 0)
""" % {'name': nm, 'path': os.path.abspath(exename), 'libdir': os.path.abspath("lib")})
    out.write("""
if __name__ == '__main__':
    IMP.test.main()
""")


def generate_tests(source, scons):
    template = """import IMP
import IMP.test
import %(module)s

spelling_exceptions=%(spelling_exceptions)s

class StandardsTest(IMP.test.TestCase):
    def test_value_objects(self):
        "Test that module classes are either values or objects"
        exceptions= %(value_object_exceptions)s
        return self.assertValueObjects(%(module)s,exceptions)
    def test_classes(self):
        "Test that module class names follow the standards"
        exceptions=%(value_object_exceptions)s
        return self.assertClassNames(%(module)s, exceptions,
                                     spelling_exceptions)
    def test_functions(self):
        "Test that module function names follow the standards"
        exceptions= %(function_name_exceptions)s
        return self.assertFunctionNames(%(module)s, exceptions,
                                        spelling_exceptions)
    def test_show(self):
        "Test all objects have show"
        exceptions=%(show_exceptions)s
        return self.assertShow(%(module)s, exceptions)

if __name__ == '__main__':
    IMP.test.main()
    """
    target = os.path.join("test")
    tools.mkdir(target)
    for module, g in tools.get_modules(source):
        targetdir = os.path.join(target, module)
        tools.mkdir(targetdir)
        exceptions = os.path.join(g, "test", "standards_exceptions")
        d = {'plural_exceptions': [],
             'show_exceptions': [],
             'function_name_exceptions': [],
             'value_object_exceptions': [],
             'class_name_exceptions': [],
             'spelling_exceptions': []}
        try:
            exec(open(exceptions, "r").read(), d)
        except IOError:
            pass
        impmodule = "IMP" if module == 'kernel' else "IMP." + module
        test = template % ({'module': impmodule,
                            'plural_exceptions': str(d['plural_exceptions']),
                            'show_exceptions': str(d['show_exceptions']),
                            'function_name_exceptions':
                                str(d['function_name_exceptions']),
                            'value_object_exceptions':
                                str(d['value_object_exceptions']),
                            'class_name_exceptions':
                                str(d['class_name_exceptions']),
                            'spelling_exceptions':
                                str(d['spelling_exceptions'])})
        gen = tools.PythonFileGenerator()
        gen.write(os.path.join("test", module, "medium_test_standards.py"),
                  test, show_diff=False)

        cpptests = tools.get_glob([os.path.join(g, "test", "test_*.cpp")])
        ecpptests = tools.get_glob(
            [os.path.join(g, "test", "expensive_test_*.cpp")])
        cppexamples = tools.get_glob([os.path.join(g, "examples", "*.cpp")])

        if len(cpptests) > 0 and scons:
            _make_test_driver(
                os.path.join(
                    targetdir,
                    "test_cpp_tests.py"),
                cpptests)
        if len(ecpptests) > 0 and scons:
            _make_test_driver(
                os.path.join(targetdir,
                             "expensive_test_cpp_tests.py"),
                cpptests)
        if len(cppexamples) > 0 and scons:
            _make_test_driver(
                os.path.join(targetdir,
                             "cpp_examples_test.py"),
                cppexamples)


def clean_pyc(dir):
    for root, dirnames, filenames in os.walk('.'):
        for d in dirnames:
            for f in tools.get_glob([os.path.join(d, "*.pyc")]):
                os.unlink(f)


def generate_src_dirs(source):
    """Make src directories for each module. This way we don't have to worry about whether
    it exists later."""
    for module, g in tools.get_modules(source):
        tools.mkdir(os.path.join("src", module), clean=False)

parser = OptionParser()
parser.add_option("-s", "--source", dest="source",
                  help="IMP source directory.")
parser.add_option("-d", "--datapath", dest="datapath",
                  help="Extra data path for IMP.")
parser.add_option("-m", "--disabled", dest="disabled",
                  help="Disabled modules.")
parser.add_option("--scons", default=False, action="store_true",
                  help="Set if we are running scons.")


def main():
    (options, args) = parser.parse_args()
    clean_pyc(options.source)
    tools.mkdir(os.path.join("data", "build_info"))
    tools.mkdir(os.path.join("cmake_tests"))
    tools.rewrite(os.path.join("data", "build_info", "disabled"),
                  options.disabled.replace(":", "\n"))
    tools.setup_sorted_order(options.source,
                             options.datapath)
    link_headers(options.source)
    link_examples(options.source)
    link_swig(options.source)
    link_python(options.source)
    link_data(options.source)
    generate_tests(options.source, options.scons)
    generate_src_dirs(options.source)

if __name__ == '__main__':
    main()
