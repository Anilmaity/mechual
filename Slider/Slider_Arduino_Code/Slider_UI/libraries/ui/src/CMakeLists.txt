SET(SOURCES ui_comp_header.c
    ui_comp.c
    ui_Blank.c
    ui_Splash.c
    ui_Home.c
    ui_ManualMode.c
    ui_ABMode.c
    ui_Calibration.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_rk_logo_png.c
    ui_img_rk_logo_40_40_png.c
    ui_img_battery_png.c
    ui_img_hand_16725997_png.c
    ui_img_adapt_10984466_png.c
    ui_img_time_14147652_png.c
    ui_img_back_png.c
    ui_img_back2_png.c
    ui_img_group_4_png.c
    ui_img_infomer_png.c
    ui_img_box_png.c
    ui_img_16417679.c
    ui_img_rectangle_163_png.c)

add_library(ui ${SOURCES})
