#!/bin/bash

fw_installed swift3 && return 0

fw_depends clang-3.9

sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-3.9 100
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-3.9 100

fw_depends cmake-3.5

SWIFT_SNAPSHOT="swift-3.0.2-RELEASE"
SWIFT_SNAPSHOT_LOWERCASE="swift-3.0.2-release"
UBUNTU_VERSION="ubuntu14.04"
UBUNTU_VERSION_NO_DOTS="ubuntu1404"

fw_get -O https://swift.org/builds/$SWIFT_SNAPSHOT_LOWERCASE/$UBUNTU_VERSION_NO_DOTS/$SWIFT_SNAPSHOT/$SWIFT_SNAPSHOT-$UBUNTU_VERSION.tar.gz
fw_untar $SWIFT_SNAPSHOT-$UBUNTU_VERSION.tar.gz
mv $SWIFT_SNAPSHOT-$UBUNTU_VERSION swift

echo -e "export PATH=${IROOT}/swift/usr/bin:\$PATH" >> $IROOT/swift3.installed

source $IROOT/swift3.installed
