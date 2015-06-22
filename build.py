import os
import shutil
import tarfile
import subprocess

from fill_database import main as fill_dummy_database

HOME_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_FILE = 'build.tar.gz'

def make_cgi():
    os.chdir(os.path.join(HOME_DIR, 'c', 'rest'))
    subprocess.check_call('make')
    os.chdir(HOME_DIR)

def make_uploader():
    os.chdir(os.path.join(HOME_DIR, 'c', 'uploader'))
    subprocess.check_call('make')
    os.chdir(HOME_DIR)

def create_tarfile():
    ignored_dirs = {'c', 'misc', 'schemas'}
    ignored_files = {'download_from_ftp.sh', '.gitignore'}
    ignored_extensions = {".py", ".pyc"}
    os.chdir(HOME_DIR)
    should_add = lambda f: any((os.path.isdir(f) and f not in ignored_dirs,
                                os.path.isfile(f) and f not in ignored_files
                                and not any(f.endswith(e) for e in ignored_extensions)))
    with tarfile.open(BUILD_FILE, 'w:gz') as tar:
        for f in os.listdir(os.curdir):
            if should_add(f):
                tar.add(f)

def copy_to_tftpboot():
    shutil.copyfile(BUILD_FILE, os.path.join("/tftpboot", BUILD_FILE))


def main():
    make_cgi()
    make_uploader()
    fill_dummy_database()
    create_tarfile()
    # copy_to_tftpboot()

if __name__ == "__main__":
    main()
