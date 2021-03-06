Name:       capi-media-metadata-extractor
Summary:    A media metadata extractor library in SLP C API
Version: 0.1.0
Release:    4
Group:      TO_BE/FILLED_IN
License:    TO BE FILLED IN
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(mm-fileinfo)
BuildRequires:  pkgconfig(capi-base-common)
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description
A media metadata extractor  library in SLP C API

%package devel
Summary:  A media metadata extractor  library in SLP C API (Development)
Group:    TO_BE/FILLED_IN
Requires: %{name} = %{version}-%{release}

%description devel
A media metadata extractor  library in SLP C API

%prep
%setup -q


%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
/usr/lib/libcapi-media-metadata-extractor.so

%files devel
/usr/include/media/*.h
/usr/lib/pkgconfig/capi-media-metadata-extractor.pc

