import glob
import os
import os.path
import copy
import shutil
import sys
import difflib
import subprocess
import signal

# cmake paths are always /-separated; on platforms where the path is not /
# (e.g. Windows) convert a path to or from a form cmake will like
if os.sep == '/':
    def to_cmake_path(p):
        return p

    def from_cmake_path(p):
        return p
else:
    def to_cmake_path(p):
        return p.replace(os.sep, '/')

    def from_cmake_path(p):
        return p.replace('/', os.sep)


def get_existing_links(path):
    """Get any symlinks in the given directory"""
    return [f for f in glob.glob(os.path.join(path, "*")) if os.path.islink(f)]


def mkdir(path, clean=True):
    if os.path.isdir(path):
        if clean:
            # remove any old links
            for f in glob.glob(os.path.join(path, "*")):
                if os.path.islink(f):
                    os.unlink(f)
            # Remove old lists of Python tests
            for f in glob.glob(os.path.join(path, "*.pytests")):
                os.unlink(f)
        else:
            # clean broken links
            for f in glob.glob(os.path.join(path, "*")):
                if os.path.islink(f) and not os.path.exists(os.readlink(f)):
                    os.unlink(f)
        return
    if os.path.isfile(path):
        os.unlink(path)
    os.makedirs(path)


def quote(st):
    out = st.replace(
        "\\f",
        ""). replace(
        "\\b",
        "").replace(
        "\\",
        "\\\\").replace(
        "\"",
        "\\\"")
    return out


def get_glob(patterns):
    ret = []
    for p in patterns:
        c = sorted(glob.glob(p))
        ret += c
    return ret


def rewrite(filename, contents, verbose=True):
    try:
        old = open(filename, "r").read()
        if old == contents:
            return
        elif verbose:
            print("    Different " + filename)
            for l in difflib.unified_diff(old.split("\n"), contents.split("\n")):
                stl = str(l)
                if (stl[0] == '-' or stl[0] == '+') and stl[1] != '-' and stl[1] != '+':
                    print("    " + stl)
    except:
        pass
        # print "Missing", filename
    dirpath = os.path.split(filename)[0]
    if dirpath != "":
        mkdir(dirpath, False)
    open(filename, "w").write(contents)

class FileGenerator(object):
    """Auto-generate an output file.
       The file is marked as being auto-generated with a suitable comment.
       `template_file` names an input file containing a suitable template
       (or None) and `start_comment` specifies the string used to start
       a single-line comment (e.g. #, ;, //)
    """
    def __init__(self, template_file, start_comment):
        self.template_file = template_file
        self.start_comment = start_comment
        if template_file:
            self.template = open(template_file).read()
            self.template_file = os.path.relpath(template_file,
                                                 self.get_imp_top())

    def get_imp_top(self):
        ourdir = os.path.dirname(__file__)
        return os.path.abspath(os.path.join(ourdir, '..', '..', '..'))

    def get_gen_prog(self):
        """Get the name of the program that generated the file"""
        return os.path.relpath(sys.argv[0], self.get_imp_top())

    def write(self, outfile, output):
        """Write the output file `outfile` using `output`.
           If `template_file` specified a file above, read the contents of that
           file and make Python-style substitutions using output (which should
           be a dict). If `template_file` is None, simply write `output` to the
           file."""
        if self.template_file:
            output = '%s Autogenerated by %s\n%s from %s\n' \
                     '%s Do not edit - any changes will be lost!\n\n' \
                     % (self.start_comment, self.get_gen_prog(),
                        self.start_comment, self.template_file,
                        self.start_comment) + self.template % output
        else:
            output = '%s Autogenerated by %s\n' \
                     '%s Do not edit - any changes will be lost!\n\n' \
                     % (self.start_comment, self.get_gen_prog(),
                        self.start_comment) + output
        rewrite(outfile, output)


class CMakeFileGenerator(FileGenerator):
    def __init__(self, template_file=None):
        FileGenerator.__init__(self, template_file, '#')

class CPPFileGenerator(FileGenerator):
    def __init__(self, template_file=None):
        FileGenerator.__init__(self, template_file, '//')


def rmdir(path):
    try:
        shutil.rmtree(path)
    except:
        pass


def link(source, target, verbose=False):
    tpath = os.path.abspath(target)
    spath = os.path.abspath(source)
    # print tpath, spath
    if not os.path.exists(spath):
        if verbose:
            print("no source", spath)
        return
    if os.path.islink(tpath):
        if os.readlink(tpath) == spath:
            return
        else:
            os.unlink(tpath)
    elif os.path.isdir(tpath):
        shutil.rmtree(tpath)
    elif os.path.exists(tpath):
        os.unlink(tpath)
    if verbose:
        print("linking", spath, tpath)
    if hasattr(os, 'symlink'):
        os.symlink(spath, tpath)
    # Copy instead of link on platforms that don't support symlinks (Windows)
    elif os.path.isdir(spath):
        def excludes(src, names):
            return ['.svn', 'data']
        shutil.copytree(spath, tpath, ignore=excludes)
    else:
        shutil.copy(spath, tpath)

def has_python_hashbang(fname):
    line = open(fname).readline()
    return line.startswith('#!') and 'python' in line

def filter_pyapps(fname):
    """A Python application ends in .py, or starts with #!(something)python;
       exclude dependencies.py."""
    return os.path.isfile(fname) and not fname.endswith('dependencies.py') \
           and (fname.endswith('.py') or has_python_hashbang(fname))

def link_dir(source_dir, target_dir, match=["*"], exclude=[],
             clean=True, verbose=False, filt=None, make_subdirs=False):
    if not isinstance(match, list):
        raise TypeError("Expecting a list object for match")
    exclude = exclude + ["SConscript", "CMakeLists.txt", "Files.cmake", ".svn"]
    # print "linking", source_dir, target_dir
    mkdir(target_dir, clean=False)
    if clean:
        existing_links = get_existing_links(target_dir)
    # Don't clean links here, as that forces any valid symlinks to be
    # recreated (potentially forcing a useless rebuild). We'll handle them
    # at the end of this function.
    files = []
    targets = {}
    for m in match:
        files.extend(get_glob([os.path.join(source_dir, m)]))
    if filt:
        files = [x for x in files if filt(x)]
    for g in files:
        name = os.path.split(g)[1]
        if name not in exclude:
            target = os.path.join(target_dir, name)
            targets[target] = None
            if os.path.isdir(g) and make_subdirs:
                if os.path.islink(target):
                    os.unlink(target)
                link_dir(g, target, match, exclude, clean=clean,
                         verbose=verbose, filt=filt, make_subdirs=True)
            else:
                link(g, target, verbose=verbose)
    if clean:
        # Remove any old links that are no longer valid
        for ln in existing_links:
            if ln not in targets:
                os.unlink(ln)


def get_modules(source):
    path = os.path.join(source, "modules", "*")
    globs = get_glob([path])
    mods = [(os.path.split(g)[1], g) for g in globs if (os.path.split(g)[1] != "SConscript")
            and os.path.exists(os.path.join(g, "dependencies.py"))]
    return mods


def get_dependency_description(path):
    d = {'pkg_config_name':  None, 'headers': "", 'libraries': "",
         'extra_libraries': "", 'version_cpp': "", 'version_headers': "",
         'body': "", 'python_module': "", 'is_cmake': False,
         'name': os.path.splitext(os.path.split(path)[1])[0]}
    exec(open(path, "r").read(), d)
    passlibs = split(d['libraries'])
    passheaders = split(d['headers'])
    extra_libs = split(d['extra_libraries'])
    cmakef = os.path.splitext(path)[0] + ".cmake"
    if os.path.exists(cmakef):
        cmake = "include(\"${PROJECT_SOURCE_DIR}/%s\")" % (
            to_cmake_path(os.path.splitext(path)[0] + ".cmake"))
    else:
        cmake = ""
    if d['pkg_config_name'] is None:
        d['pkg_config_name'] = d['name'].lower()
    return {"name": d['name'],
            "pkg_config_name": d['pkg_config_name'],
            "headers": passheaders,
            "libraries": passlibs,
            "extra_libraries": extra_libs,
            "body": d['body'],
            "version_cpp": split(d['version_cpp']),
            "version_headers": split(d['version_headers']),
            "cmake": cmake,
            "python_module": d['python_module']}


def get_module_description(source, module, extra_data_path, root="."):
    df = os.path.join(root, source, "modules", module, "dependencies.py")
    if os.path.exists(df):
        d = {'required_modules': "",
             'optional_modules': "",
             'required_dependencies': "",
             'optional_dependencies': ""}
        exec(open(df, "r").read(), d)
        return {"required_modules": split(d['required_modules']),
                "optional_modules": split(d['optional_modules']),
                "required_dependencies": split(d['required_dependencies']),
                "optional_dependencies": split(d['optional_dependencies'])}
    else:
        info = get_module_info(module, extra_data_path)
        return {"required_modules": info["modules"],
                "optional_modules": [],
                "required_dependencies": info["dependencies"],
                "optional_dependencies": []}


def get_all_modules(source, modules, extra_data_path, ordered, root="."):
    ret = []
    stack = modules
    while len(stack) > 0:
        cur = stack[-1]
        stack = stack[:-1]
        descr = get_module_description(source, cur, extra_data_path)
        for m in descr["required_modules"] + descr["optional_modules"]:
            if m not in ret:
                ret.append(m)
                stack.append(m)
    ret.sort(key=lambda x: ordered.index(x))
    return ret


def get_all_dependencies(source, modules, extra_data_path, ordered, root="."):
    mods = modules + get_all_modules(
        source,
        modules,
        extra_data_path,
        ordered,
        root)
    ret = []
    for m in mods:
        descr = get_module_description(source, m, extra_data_path)
        for d in descr["required_dependencies"] + descr["optional_dependencies"]:
            if d not in ret:
                ret.append(d)
    return ret


dependency_info_cache = {}


def get_dependency_info(dependency, extra_data_path, root="."):
    global dependency_info_cache
    if dependency in dependency_info_cache:
        return dependency_info_cache[dependency]
    df = os.path.join(root, "data", "build_info", dependency)
    if not os.path.exists(df) and extra_data_path != "":
        df = os.path.join(extra_data_path, "build_info", dependency)
    d = {'libraries': "", 'version': "", 'includepath': "",
         'libpath': "", 'swigpath': "", 'ok': False}
    # try:
    exec(open(df, "r").read(), d)
    # except:
    #    print >> sys.stderr, "Error reading dependency", dependency, "at", df
    ret = {"ok": d['ok'],
           "libraries": split(d['libraries']),
           "version": split(d['version']),
           "includepath": d['includepath'],
           "libpath": d['libpath'],
           "swigpath": d['swigpath']}
    dependency_info_cache[dependency] = ret
    return ret

module_info_cache = {}


def get_module_info(module, extra_data_path, root="."):
    global module_info_cache
    if module in module_info_cache:
        return module_info_cache[module]
    if module.find("/") != -1:
        raise ValueError("module name invalid: " + module)
    df = os.path.join(root, "data", "build_info", "IMP." + module)
    external = False
    if not os.path.exists(df) and extra_data_path != "":
        external = True
        df = os.path.join(extra_data_path, "build_info", "IMP." + module)
    d = {'modules': "", 'unfound_modules': "",
         'dependencies': "", 'unfound_dependencies': "",
         'swig_wrapper_includes': "", 'swig_includes': "",
         'swig_path': "", 'include_path': "", 'lib_path': "", 'ok': False}
    exec(open(df, "r").read(), d)
    ret = {"ok": d['ok'],
           "modules": split(d['modules']),
           "unfound_modules": split(d['unfound_modules']),
           "dependencies": split(d['dependencies']),
           "unfound_dependencies": split(d['unfound_dependencies']),
           "swig_includes": split(d['swig_includes']),
           "swig_wrapper_includes": split(d['swig_wrapper_includes'])}
    if external:
        ret["external"] = True
    module_info_cache[module] = ret
    return ret


# a version of split that doesn't return empty strings when there are no items


def split(string, sep=":"):
    return (
        [x.replace("@", ":")
         for x in string.replace("\:", "@").split(sep) if x != ""]
    )


def toposort2(data):
    ret = []
    while True:
        ordered = [item for item, dep in data.items() if not dep]
        if not ordered:
            break
        ret.extend(sorted(ordered))
        d = {}
        for item, dep in data.items():
            if item not in ordered:
                d[item] = [x for x in dep if x not in ordered]
        data = d
    return ret

order_cache = None


def get_sorted_order(root="."):
    global order_cache
    if order_cache:
        return order_cache
    order = split(
        open(
            os.path.join(
                root,
                "data",
                "build_info",
                "sorted_modules"),
            "r").read(
        ),
        "\n")
    order_cache = order
    return order


def set_sorted_order(
    sorted,
    outpath=os.path.join(
        "data",
        "build_info",
        "sorted_modules")):
    global order_cache
    order_cache = sorted
    rewrite(outpath,
            "\n".join(sorted), verbose=False)


def compute_sorted_order(source, extra_data_path):
    data = {}
    for m, path in get_modules(source):
        df = os.path.join(path, "dependencies.py")
        if not os.path.exists(df):
            continue
        info = get_module_description(source, m, extra_data_path)
        data[m] = info["required_modules"] + info["optional_modules"]
        # toposort is destructive
        # get external modules, a bit sloppy for now
    while True:
        to_add = {}
        for mk in data.keys():
            for m in data[mk]:
                if m not in data:
                    print('adding', m)
                    info = get_module_info(m, extra_data_path)
                    to_add[m] = info["modules"]
        for m in to_add.keys():
            data[m] = to_add[m]
        if len(to_add.keys()) == 0:
            break
    sorted = toposort2(data)
    return sorted


def setup_sorted_order(source, extra_data_path,
                       outpath=os.path.join("data", "build_info", "sorted_modules")):
    sorted = compute_sorted_order(source, extra_data_path)
    set_sorted_order(sorted, outpath)
    # return sorted


def get_dependent_modules(modules, extra_data_path, root="."):
    for x in modules:
        if x.find("/") != -1:
            raise value_error("bad module name: " + x)
    sorted_order = get_sorted_order(root)
    new_modules = modules
    all_modules = new_modules
    while len(new_modules) > 0:
        m = new_modules[-1]
        new_modules = new_modules[:-1]
        cur_modules = [x for x in get_module_info(m, extra_data_path, root)["modules"]
                       if x not in all_modules]
        all_modules += cur_modules
        new_modules += cur_modules
    all_modules.sort(key=lambda x: sorted_order.index(x), reverse=True)
    return all_modules


def get_dependent_dependencies(modules, dependencies, extra_data_path,
                               root="."):
    for x in modules:
        if x.find("/") != -1:
            raise value_error("bad module name: " + x)
    sorted_order = get_sorted_order(root)
    ret_names = []
    for m in modules:
        info = get_module_info(m, extra_data_path, root)
        ret_names.extend(info["dependencies"])
    ret = list(set(ret_names + dependencies))
    return ret


def get_module_version(module, source_dir):
    in_module_source = os.path.join(source_dir, "modules", module, "VERSION")
    in_module_build = os.path.join("modules", module, "VERSION")
    in_source = os.path.join(source_dir, "VERSION")
    in_build = "VERSION"
    if os.path.exists(in_module_source):
        return open(in_module_source, "r").read().strip()
    elif os.path.exists(in_module_build):
        return open(in_module_build, "r").read().strip()
    elif os.path.exists(in_source):
        return open(in_source, "r").read().strip()
    else:
        return open(in_build, "r").read().strip()


def get_disabled_modules(extra_data_path, root="."):
    all = get_glob([os.path.join(root, "data", "build_info", "IMP.*")])
    modules = [os.path.splitext(a)[1][1:] for a in all]
    return (
        [x for x in modules if not get_module_info(
            x, extra_data_path, root)["ok"]]
    )


_subprocesses = []


def run_subprocess(command, **kwargs):
    global _subprocesses
    # if not kwargs.has_key("stdout"):
    #    kwargs["stdout"] = subprocess.PIPE
    # if not kwargs.has_key("stderr"):
    #    kwargs["stderr"] = subprocess.PIPE
    pro = subprocess.Popen(
        command, preexec_fn=os.setsid, stderr=subprocess.PIPE,
        stdout=subprocess.PIPE, universal_newlines=True, **kwargs)
    _subprocesses.append(pro)
    output, error = pro.communicate()
    ret = pro.returncode
    if ret != 0:
        sys.stderr.write(error + '\n')
        raise OSError("subprocess failed with return code %d: %s\n%s"
                      % (ret, " ".join(command), error))
    return output


def _sigHandler(signum, frame):
    print("starting handler")
    signal.signal(signal.SIGTERM, signal.SIG_DFL)
    global _subprocesses
    for p in _subprocesses:
        print("killing", p)
        try:
            os.kill(p.pid, signal.SIGTERM)
        except:
            pass
    sys.exit(1)

signal.signal(signal.SIGTERM, _sigHandler)
