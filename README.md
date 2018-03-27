# FRASER_Co-Simulation

## Getting Started

    git clone --recursive https://github.com/Project-Bonfire/FRASER_Co-Simulation.git

Make sure that you also clone the submodules within the repository by adding the flag
--recursive.

## Best way to pull latest of all git submodules
On init running the following command:

    git submodule update --init --recursive

After this you can just run:

    git submodule update --recursive

## Dependency installation
There is a script (*setup_env_for_fraser.sh*) which will install all dependencies for running FRASER (tested on a fresh installation of Linux Mint 18.3, based on Ubuntu LTS 16.04).

For installation on any other OS, or for manual installation, please check the documentation in the script.

## Running

**Before running:**

Edit the 'owner' parameter in the following line in *fraser-template/ansible/roles/deployment/tasks/main.yml* to match your current Linux user name:

```yaml
- name: Fix 'tmp-folder' permission
  file: path=~/tmp_simulation owner=user mode=0775 state=directory recurse=yes
```

**Running:**

```bash
cd fraser-template
make all
```


**Note:** This will ask you for a sudo password in order to install additional dependencies.

## License

Unless otherwise noted the source code is licensed under the
terms of the *GNU GENERAL PUBLIC LICENSE Version 3*.

The following open source software is used:

 - Fraser-template (licensed under MPL V2)
