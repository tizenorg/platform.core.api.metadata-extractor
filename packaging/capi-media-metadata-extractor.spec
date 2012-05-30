Name:       capi-media-metadata-extractor
Summary:    A media metadata extractor library in SLP C API
Version: 0.1.0
Release:    2
Group:      TO_BE/FILLED_IN
License:    TO BE FILLED IN
Source0:    %{name}-%{version}.tar.gz
Source1001: packaging/capi-media-metadata-extractor.manifest 
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(mm-fileinfo)
BuildRequires:  pkgconfig(mm-player)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(capi-media-sound-manager)
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description
A media metadata extractor  library in SLP C API

%package devel
Summary:  A media metadata extractor  library in SLP C API (Development)
Group:    TO_BE/FILLED_IN

%description devel
A media metadata extractor  library in SLP C API

%prep
%setup -q


%build
cp %{SOURCE1001} .
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest capi-media-metadata-extractor.manifest

%files devel
%manifest capi-media-metadata-extractor.manifest
/usr/include/media/*.h
/usr/lib/libcapi-media-metadata-extractor.so
/usr/lib/pkgconfig/capi-media-metadata-extractor.pc

