check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif() 

build_lib(
    LIBNAME wireless
    SOURCE_FILES model/wireless-channel.cc
                 model/wireless-mac-upcalls.cc
                 model/wireless-phy-upcalls.cc
                 model/wireless-phy.cc
                 model/wireless-plcp-header.cc
                 model/wireless-transmission-unit.cc
                 model/wireless-transmission-vector.cc
    HEADER_FILES model/wireless-channel.h
                 model/wireless-mac-upcalls.h
                 model/wireless-phy-upcalls.h
                 model/wireless-phy.h
                 model/wireless-plcp-header.h
                 model/wireless-transmission-unit.h
                 model/wireless-transmission-vector.h
    LIBRARIES_TO_LINK ${libcore} ${libinternet}
)
    
