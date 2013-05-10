#! /usr/bin/env python
import sys
import os.path
import subprocess
import glob

os.system("git submodule init")
os.system("git submodule update")

module = False

if len(sys.argv) == 2:
    if sys.argv[1] == "--module":
        module=True
    else:
        print >> sys.stderr, "usage:", sys.argv[0], "[--module]"
        print >> sys.stderr, "The module argument is for running it in a repository containing only a module (not the full IMP repository)."
        exit(1)

if not os.path.exists(".git"):
    print >> sys.stderr, "Script must be run from a git root directory"
    exit(1)

if not module and not os.path.exists("modules"):
    print >> sys.stderr, "Script must be run from a IMP git directory"
    exit(1)

if module and not os.path.exists("include"):
    print >> sys.stderr, "Module must have an include directory"
    exit(1)

cmd = subprocess.Popen(["git", "rev-parse", "--abbrev-ref", "HEAD"],
                           stdout = subprocess.PIPE)
branch = cmd.stdout.read()

imp_root = os.path.split(sys.argv[0])[0]

if module:
    print "imp root is", imp_root

sys.path.append(os.path.join(imp_root, "tools"))
import build.tools

config = os.path.join(imp_root, "tools", "git")
if module:
    build.tools.link_dir(os.path.join(config, "module_config", "hooks"), os.path.join(".git", "hooks"))
else:
    build.tools.link_dir(os.path.join(config, "config", "hooks"), os.path.join(".git", "hooks"))

config_contents = open(os.path.join(".git", "config"), "r").read()

cmd = subprocess.Popen(["git", "branch", "-r"],
                           stdout = subprocess.PIPE)
branches = cmd.stdout.read()

if config_contents.find("gitimp") != -1:
    pass
elif branches.find("develop") == -1 or branches.find("master") == -1:
    print "Git imp not set up as the repository does not have both a master and a develop branch."
else:
    if module:
        os.system(os.path.join(imp_root, "tools", "git", "gitflow", "git-imp") + " init")
    else:
        os.system(os.path.join("tools", "git", "gitflow", "git-imp") + " init")

# hard to check for
os.system("git config push.default nothing")
os.system("git config log.decorate full")

if config_contents.find("color \"branch\"") == -1:
    print "Updating git colors"
    os.system("git config color.ui true")
    os.system("git config color.branch true")
    os.system("git config color.diff true")
    os.system("git config color.status true")
    os.system("git config color.branch.current yellow reverse")
    os.system("git config color.branch.local yellow")
    os.system("git config color.branch.remote green")
    os.system("git config color.diff.meta \"yellow bold\"")
    os.system("git config color.diff.frag \"magenta bold\"")
    os.system("git config color.diff.old red")
    os.system("git config color.diff.new cyan")
    os.system("git config color.status.added yellow")
    os.system("git config color.status.changed green")
    os.system("git config color.status.untracked cyan")
if config_contents.find("whitespace = fix,-indent-with-non-tab,trailing-space,cr-at-eol") == -1:
    print "Telling git to clean up whitespace"
    os.system("git config core.whitespace \"fix,-indent-with-non-tab,trailing-space,cr-at-eol\"")

if config_contents.find("autosetuprebase = always") == -1:
    print "Telling git to rebase by default on pull"
    os.system("git config branch.autosetuprebase always")
# hard to check for
os.system("git config branch.develop.rebase true")
os.system("git config branch.master.rebase true")

print "Adding git-flow imp aliases"
os.system("git config alias.imp !tools/git/gitflow/git-imp")

os.system("git config commit.template tools/git/commit_message.txt")

if not module:
    # anyone who is confused by branches should be on master
    os.system("git checkout "+branch)
else:
    # make sure VERSION is ignored
    path = os.path.join(".git", "info", "exclude")
    ignored = open(path, "r").readlines()
    if not "VERSION" in ignored:
        ignored.append("VERSION")
        open(path, "w").writelines(ignored)
