inherit autotools pkgconfig gtk-doc gettext autotools-brokensep

DEPENDS = "clutter-1.25 dbus-glib gdk-pixbuf"

PROVIDES = "mx-1.0"

RDEPENDS_libmx-1.0 = "libEGL"
