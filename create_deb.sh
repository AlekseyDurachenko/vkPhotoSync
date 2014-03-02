#!/bin/bash

rm -r debian_package

mkdir debian_package
mkdir debian_package/DEBIAN
mkdir debian_package/usr
mkdir debian_package/usr/bin
mkdir debian_package/usr/share
mkdir debian_package/usr/share/applications
mkdir debian_package/usr/share/icons
mkdir debian_package/usr/share/icons/hicolor
mkdir debian_package/usr/share/icons/hicolor/128x128
mkdir debian_package/usr/share/icons/hicolor/128x128/apps
mkdir debian_package/usr/share/pixmaps

cp bin/vkphotosync debian_package/usr/bin/
strip debian_package/usr/bin/vkphotosync
cp vkphotosync.desktop debian_package/usr/share/applications/
cp qrc/icons/wicon.png debian_package/usr/share/icons/hicolor/128x128/apps/vkphotosync.png
cp qrc/icons/wicon.png debian_package/usr/share/pixmaps/vkphotosync.png

chmod 0755 debian_package/DEBIAN
chmod 0755 debian_package/usr
chmod 0755 debian_package/usr/bin
chmod 0755 debian_package/usr/share
chmod 0755 debian_package/usr/share/applications
chmod 0755 debian_package/usr/share/icons
chmod 0755 debian_package/usr/share/icons/hicolor
chmod 0755 debian_package/usr/share/icons/hicolor/128x128
chmod 0755 debian_package/usr/share/icons/hicolor/128x128/apps
chmod 0755 debian_package/usr/share/pixmaps
chmod 0755 debian_package/usr/bin/vkphotosync
chmod 0644 debian_package/usr/share/applications/vkphotosync.desktop
chmod 0644 debian_package/usr/share/icons/hicolor/128x128/apps/vkphotosync.png
chmod 0644 debian_package/usr/share/pixmaps/vkphotosync.png

VER=`git describe --match=v* HEAD --tags`
VER=`echo $VER| sed 's/^v//'`
SIZE=`du -cb ./debian_package/usr | grep total | awk '{print int($1/1024)}'`
ARCH=`dpkg --print-architecture`
CODENAME=`lsb_release -c | awk '{print $2}'`

echo "Package: vkphotosync
Version: $VER
Section: net
Priority: optional
Architecture: $ARCH
Installed-Size: $SIZE
Depends: libc6 (>= 2.14), libgcc1 (>= 1:4.1.1), libqt4-webkit (>= 4:4.8.0), libqt4-network (>= 4:4.8.0), libqt4-xml (>= 4:4.8.0), libqtcore4 (>= 4:4.8.0), libqtgui4 (>= 4:4.8.0)
Homepage: https://github.com/AlekseyDurachenko/vkPhotoSync
Maintainer: Aleksey Durachenko <durachenko.aleksey@gmail.com>
Description: vkontakte photo downloader
" > debian_package/DEBIAN/control

cd debian_package || exit 1
md5deep -r -l usr > ../debian_package/DEBIAN/md5sums
cd ..

chmod 644 debian_package/DEBIAN/*

fakeroot dpkg-deb --build debian_package
mv debian_package.deb "vkphotosync_""$VER~$CODENAME""_""$ARCH"".deb"
lintian "vkphotosync_""$VER~$CODENAME""_""$ARCH"".deb"
