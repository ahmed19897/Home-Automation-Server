# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Engineering/programes/esp-idf-v5.1.4/esp-idf-v5.1.4/components/bootloader/subproject"
  "C:/Users/user/esp/SOCKET_ADC_RTOS/build/bootloader"
  "C:/Users/user/esp/SOCKET_ADC_RTOS/build/bootloader-prefix"
  "C:/Users/user/esp/SOCKET_ADC_RTOS/build/bootloader-prefix/tmp"
  "C:/Users/user/esp/SOCKET_ADC_RTOS/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/user/esp/SOCKET_ADC_RTOS/build/bootloader-prefix/src"
  "C:/Users/user/esp/SOCKET_ADC_RTOS/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/user/esp/SOCKET_ADC_RTOS/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/user/esp/SOCKET_ADC_RTOS/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
