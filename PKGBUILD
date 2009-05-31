# Maintainer: Pierre Schmitz <pierre@archlinux.de>

pkgname=archlinux-torrent-utils-git
pkgver=20090531
pkgrel=1
pkgdesc='Utilities to create and list (Arch Linux iso) torrents'
arch=('i686' 'x86_64')
url='http://git.archlinux.de/archlinux-torrent-utils/'
license=('GPL')
depends=('ktorrent' 'subversion')
makedepends=('git')

_gitroot="http://git.archlinux.de/~pierre/archlinux-torrent-utils.git"
_gitname="archlinux-torrent-utils"

build() {
	cd "$srcdir"
	msg "Connecting to GIT server...."

	if [ -d $_gitname ] ; then
		cd $_gitname && git pull origin
		msg "The local files are updated."
	else
		git clone $_gitroot
	fi

	msg "GIT checkout done or server timeout"
	msg "Starting make..."

	rm -rf "$srcdir/$_gitname-build"
	git clone "$srcdir/$_gitname" "$srcdir/$_gitname-build"
	cd "$srcdir/$_gitname-build"

	cmake . \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_SKIP_RPATH=ON \
		-DCMAKE_{SHARED,MODULE,EXE}_LINKER_FLAGS='-Wl,--no-undefined -Wl,--as-needed' \
		-DCMAKE_INSTALL_PREFIX=/usr
	make
	make DESTDIR=$pkgdir install
} 
