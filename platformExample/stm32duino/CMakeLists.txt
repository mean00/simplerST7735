
SET(S ${CMAKE_CURRENT_SOURCE_DIR}/../..)

SET(SIMPLER ${S}/simplerST7735.cpp ${S}/simplerST7735_ex.cpp ${S}/simplerST7735_ex_ll.cpp stm32duinoST7735.cpp)

generate_arduino_library(${libPrefix}simplerST7735
                            SRCS ${SIMPLER}
                            BOARD ${ARDUINO_DEFAULT_BOARD}
                        )

TARGET_INCLUDE_DIRECTORIES(${libPrefix}simplerST7735  PRIVATE ${S} .)                           
#ADD_SUBDIRECTORY(simplerST7735)
