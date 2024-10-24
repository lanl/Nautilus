#!/bin/bash
PARENT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]:-$0}" )" >/dev/null 2>&1 && pwd )"
PROJECT_NAME=xcap-sample-repo

print_usage() {
  echo "usage: build_docs [-h] system"
  echo
  echo "Use the environment variable PROJECT_SPACK_ENV to select a different"
  echo "upstream."
  echo
  echo "positional arguments:"
  echo "  system         name of the system"
  echo
  echo "options:"
  echo "  -h, --help     show this help message and exit"
}

VALID_CMD=false
if [ $# -eq 1 ] && [[ ! "$1" =~ ^-.*  ]]; then
  VALID_CMD=true
fi

if ! $VALID_CMD; then
    print_usage
    return
fi

export SYSTEM_NAME=$1
export SUBMIT_TO_CDASH=false

# colors
COLOR_BLUE='\033[1;34m'
COLOR_MAGENTA='\033[1;35m'
COLOR_CYAN='\033[1;36m'
COLOR_PLAIN='\033[0m'

echo "Running on $(hostname)"

if [ ${CI} ]; then
  echo " "
  echo -e "${COLOR_BLUE}######################################################################${COLOR_PLAIN}"
  echo " "
  echo -e "${COLOR_BLUE}To recreate this CI run, follow these steps:${COLOR_PLAIN}"
  echo " "
  echo -e "${COLOR_BLUE}ssh ${SYSTEM_NAME}${COLOR_PLAIN}"
  echo -e "${COLOR_BLUE}cd /your/${PROJECT_NAME}/checkout${COLOR_PLAIN}"
  if [[ ! -z "${LLNL_FLUX_SCHEDULER_PARAMETERS}" ]]; then
    echo -e "${COLOR_BLUE}flux alloc ${LLNL_FLUX_SCHEDULER_PARAMETERS}${COLOR_PLAIN}"
  elif [[ ! -z "${LLNL_LSF_SCHEDULER_PARAMETERS}" ]]; then
    echo -e "${COLOR_BLUE}bsub -I ${LLNL_LSF_SCHEDULER_PARAMETERS}${COLOR_PLAIN}"
  else
    echo -e "${COLOR_BLUE}salloc ${SCHEDULER_PARAMETERS}${COLOR_PLAIN}"
  fi
  echo -e "${COLOR_BLUE}source .gitlab/build_docs.sh ${SYSTEM_NAME}${COLOR_PLAIN}"
  echo " "
  echo -e "${COLOR_BLUE}See 'source .gitlab/build_docs.sh -h' for more options.${COLOR_PLAIN}"
  echo " "
  echo -e "${COLOR_BLUE}######################################################################${COLOR_PLAIN}"
  echo " "
fi

export SHOW_HELP_MESSAGE=false
source ${PARENT_DIR}/build_and_test.sh --until cmake ${SYSTEM_NAME} doc

section start "cmake_build_docs[collapsed=false]" "CMake Build Documentation"
(
source ${BUILD_ENV}
cmake -DCMAKE_VERBOSE_MAKEFILE=off -DENABLE_DOCUMENTATION=on ${BUILD_DIR}
cmake --build ${BUILD_DIR} --target docs
)
section end "cmake_build_docs"
