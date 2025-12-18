#!/bin/bash

if [ "$1" == "publish" ]; then

  echo ===========================================================================
  echo "[INFO] Publishing ..."
  echo ===========================================================================
  VERSION=`cat VERSION`
  RELEASE=`git tag -l | tail -1`
  if [ "$RELEASE" = "" ];then
    gh release create "v$VERSION" --generate-notes
  else 
    gh release create "v$VERSION" --generate-notes --notes-start-tag `git tag -l | tail -1`
  fi
  git fetch --tags origin
  # echo $VERSION
  # cd build
  # cpack --config CPackConfig.cmake 
  # cd ..
  # if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  #       # ...
  # elif [[ "$OSTYPE" == "darwin"* ]]; then
  #         # Mac OSX
  # elif [[ "$OSTYPE" == "cygwin" ]]; then
  #         # POSIX compatibility layer and Linux environment emulation for Windows
  # elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
  #         # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
  # elif [[ "$OSTYPE" == "win32" ]]; then
  #         # I'm not sure this can happen.
  # elif [[ "$OSTYPE" == "freebsd"* ]]; then
  #         # ...
  # else
  #         # Unknown.
  # fi

fi

if [ "$1" == "unpublish" ]; then

  echo ===========================================================================
  echo "[INFO] Remove Publishing ..."
  echo ===========================================================================
  RELEASE=`git tag -l | tail -1`
  gh release delete $RELEASE -y && git push --delete origin $RELEASE && git tag -d $RELEASE 

fi