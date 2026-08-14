file(GLOB_RECURSE SOURCES_C *.c)
file(GLOB_RECURSE SOURCES_CPP *.cpp)

idf_component_register(
    # SRCS "EV_35_3.c" "EV_35_22.c" "EV_35_1.c" "ColorPic1_320_480.c" "ColorPic2_320_480.c" "ColorPic3_320_480.c" "ColorPic4_320_480.c" "ColorPic5_320_480.c" "home.c" "f4.c" "f3.c" "f2.c" "f5.c" "f1.c" "main.c" "board.c"
    # SRCS "home.c" "f4.c" "f3.c" "f2.c" "f5.c" "f1.c" "f7_240_320.c" "f6_240_320.c" "f5_240_320.c" "f4_240_320.c" "f3_240_320.c" "f2_240_320.c" "f1_240_320.c" "main.c" "board.c"
    # SRCS "f1.c" "f2.c" "f3.c" "f4.c" "f5.c" "main.c" "board.c"
    SRCS "EV_35_1.c" "EV_35_2.c" "EV_35_3.c" "main.c" "board.c"
    INCLUDE_DIRS ".")
