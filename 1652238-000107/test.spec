Name:           test-1652238          
Version:        1.0.0             
Release:        1%{?dist}         
Summary:       test-1652238   
License:        GPL                           
Packager:       abel 
Source0:        %_sourcedir/test.tar.bz2
%description                                 
%prep                                         
%setup -q                                               
%build  
make                  
%install                                             
make install DIRROOT=%{buildroot}
%pre        
echo "׼����װ test-1652238"
%post       
systemctl daemon-reload
systemctl enable test-1652238
systemctl start test-1652238
echo "��ɰ�װ test-1652238s"
%preun      
echo "׼��ж�� test-1652238"
systemctl stop test-1652238
%postun     
echo "���ж�� test-1652238"        
%files 
%{_sbindir}/test-1652238
%{_libdir}/lib1652238.so
%{_prefix}/1652238/1652238.dat 
%{_sysconfdir}/1652238.conf
%{_unitdir}/test-1652238.service
%changelog 