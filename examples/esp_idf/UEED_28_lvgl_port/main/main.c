/*
 *November 2024
 *Author: LiuLong
 *Shenzhen VIEWE TECHNOLOGY Co., LTD
 */

#include <stdio.h>
#include "lv_demos.h"
#include "board.h"

static const char *TAG = "EXAMPLE";


#define IMG_COUNT 3

// 全局状态管理
typedef enum {
    UI_MODE_DEMO,
    UI_MODE_SLIDESHOW
} ui_mode_t;

static lv_obj_t * screens[IMG_COUNT];

static ui_mode_t current_mode = UI_MODE_DEMO;
static lv_obj_t *demo_screen = NULL;
static lv_obj_t *slideshow_screen = NULL;
static lv_obj_t *img = NULL;
static uint8_t current_img = 0;
static bool is_auto_playing = true;
static lv_timer_t *auto_timer;
static bool backlight_on = true;    // 背光状态

#if VIEWE_35_T //图片分辨率为320*480，换图片时请不要超过这个分辨率
// LV_IMG_DECLARE(f1);
// LV_IMG_DECLARE(f2);
// LV_IMG_DECLARE(f3);
// LV_IMG_DECLARE(f4);
// LV_IMG_DECLARE(f5);
// LV_IMG_DECLARE(f6);
LV_IMG_DECLARE(EV_35_1);
LV_IMG_DECLARE(EV_35_2);
LV_IMG_DECLARE(EV_35_3);
static const lv_img_dsc_t *img_src_list[IMG_COUNT] = {&EV_35_1, &EV_35_2, &EV_35_3};
#elif VIEWE_H35
LV_IMG_DECLARE(ColorPic1_320_480);
LV_IMG_DECLARE(ColorPic2_320_480);
LV_IMG_DECLARE(ColorPic3_320_480);
LV_IMG_DECLARE(ColorPic4_320_480);
LV_IMG_DECLARE(ColorPic5_320_480);
static const lv_img_dsc_t *img_src_list[IMG_COUNT] = {&ColorPic1_320_480, &ColorPic2_320_480, &ColorPic3_320_480, &ColorPic4_320_480, &ColorPic5_320_480};
#elif VIEWE_28_T || VIEWE_24_T
LV_IMG_DECLARE(f1_240_320);
LV_IMG_DECLARE(f2_240_320);
LV_IMG_DECLARE(f3_240_320);
LV_IMG_DECLARE(f4_240_320);
LV_IMG_DECLARE(f5_240_320);
static const lv_img_dsc_t *img_src_list[IMG_COUNT] = {&f1_240_320, &f2_240_320, &f3_240_320, &f4_240_320, &f5_240_320};
#endif

static void create_slideshow_ui(void);
static void toggle_backlight(void);

// 切换到图片
static void display_next_image(lv_timer_t * timer) {
    current_img = (current_img + 1) % IMG_COUNT;
    ESP_LOGI(TAG, "Switching to image %d", current_img);
    lv_img_set_src(img, img_src_list[current_img]); // 切换图片
}

static void do_slideshow_single_click(void)
{
    if (current_mode != UI_MODE_SLIDESHOW || img == NULL) {
        return;
    }
    if (auto_timer) {
        lv_timer_pause(auto_timer);
    }
    display_next_image(NULL);
}

static void do_enter_slideshow(void)
{
    create_slideshow_ui();
    lv_scr_load(slideshow_screen);
    current_mode = UI_MODE_SLIDESHOW;
    if (auto_timer) {
        lv_timer_resume(auto_timer);
    }
}

static void do_long_press_backlight(void)
{
    ESP_LOGI(TAG, "Long Press: Toggling backlight.");
    toggle_backlight();
}

static void setup_touch_layer(lv_obj_t *parent);
static void touch_event_cb(lv_event_t *e);

typedef enum {
    UI_ACTION_SINGLE_CLICK,
    UI_ACTION_DOUBLE_CLICK,
    UI_ACTION_LONG_PRESS,
} ui_action_t;

static void ui_action_async_cb(void *user_data)
{
    ui_action_t action = (ui_action_t)(size_t)user_data;
    switch (action) {
        case UI_ACTION_SINGLE_CLICK:
            do_slideshow_single_click();
            break;
        case UI_ACTION_DOUBLE_CLICK:
            do_enter_slideshow();
            break;
        case UI_ACTION_LONG_PRESS:
            do_long_press_backlight();
            break;
        default:
            break;
    }
}

// 创建轮播界面
static void create_slideshow_ui(void) {
    if (slideshow_screen) return;

    // 创建新屏幕
    slideshow_screen = lv_obj_create(NULL);
    lv_obj_set_size(slideshow_screen, LV_HOR_RES, LV_VER_RES);

    // 创建图片控件
    img = lv_img_create(slideshow_screen);
    lv_img_set_src(img, img_src_list[0]);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    // 创建自动切换定时器
    auto_timer = lv_timer_create(display_next_image, 2000, NULL);

    setup_touch_layer(slideshow_screen);
}

// 销毁轮播界面
static void destroy_slideshow_ui() {
    if (auto_timer) {
        lv_timer_del(auto_timer);
        auto_timer = NULL;
    }
    if (slideshow_screen) {
        lv_obj_del(slideshow_screen);
        slideshow_screen = NULL;
    }
    current_img = 0;
}

// 切换背光状态
static void toggle_backlight(void) {
    backlight_on = !backlight_on;
    gpio_set_level(EXAMPLE_LCD_GPIO_BL, backlight_on ? 1 : 0);
    ESP_LOGI(TAG, "Backlight %s", backlight_on ? "ON" : "OFF");
}

/*按键及触摸处理 */

static void setup_touch_layer(lv_obj_t *parent)
{
    lv_obj_t *layer = lv_obj_create(parent);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(layer, LV_OPA_TRANSP, 0);
    lv_obj_remove_flag(layer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(layer, touch_event_cb, LV_EVENT_SINGLE_CLICKED, NULL);
    lv_obj_add_event_cb(layer, touch_event_cb, LV_EVENT_DOUBLE_CLICKED, NULL);
    lv_obj_add_event_cb(layer, touch_event_cb, LV_EVENT_LONG_PRESSED, NULL);
}

static void touch_event_cb(lv_event_t *e)
{
    /* 触摸事件在 indev 处理中同步触发，须异步执行避免 lv_img_set_src 等操作重入卡死 */
    switch (lv_event_get_code(e)) {
        case LV_EVENT_SINGLE_CLICKED:
            ESP_LOGI(TAG, "Touch event: SINGLE_CLICK");
            lv_async_call(ui_action_async_cb, (void *)UI_ACTION_SINGLE_CLICK);
            break;
        case LV_EVENT_DOUBLE_CLICKED:
            ESP_LOGI(TAG, "Touch event: DOUBLE_CLICK");
            lv_async_call(ui_action_async_cb, (void *)UI_ACTION_DOUBLE_CLICK);
            break;
        case LV_EVENT_LONG_PRESSED:
            ESP_LOGI(TAG, "Touch event: LONG_PRESS");
            lv_async_call(ui_action_async_cb, (void *)UI_ACTION_LONG_PRESS);
            break;
        default:
            break;
    }
}

const char *button_event_table[] = {
    "BUTTON_PRESS_DOWN",
    "BUTTON_PRESS_UP",
    "BUTTON_PRESS_REPEAT",
    "BUTTON_PRESS_REPEAT_DONE",
    "BUTTON_SINGLE_CLICK",
    "BUTTON_DOUBLE_CLICK",
    "BUTTON_MULTIPLE_CLICK",
    "BUTTON_LONG_PRESS_START",
    "BUTTON_LONG_PRESS_HOLD",
    "BUTTON_LONG_PRESS_UP",
    "BUTTON_PRESS_END",
};
// 按钮事件回调函数
static void button_event_cb(void *arg, void *data) {
    button_event_t event = (button_event_t)data;
    ESP_LOGI(TAG, "Button event: %s", button_event_table[event]);

    switch (event) {
        case BUTTON_SINGLE_CLICK:
            if (lvgl_port_lock(0)) {
                do_slideshow_single_click();
                lvgl_port_unlock();
            }
            break;

        case BUTTON_DOUBLE_CLICK:
            if (lvgl_port_lock(0)) {
                do_enter_slideshow();
                lvgl_port_unlock();
            }
            break;

        case BUTTON_LONG_PRESS_START:
            do_long_press_backlight();
            break;

        default:
            break;
    }
}

#if CONFIG_ENTER_LIGHT_SLEEP_MODE_MANUALLY
void button_enter_power_save(void *usr_data)
{
    ESP_LOGI(TAG, "Can enter power save now");
    esp_light_sleep_start();
}
#endif

void button_init(uint32_t button_num)
{
    button_config_t btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num = button_num,
            .active_level = BUTTON_ACTIVE_LEVEL,
#if CONFIG_GPIO_BUTTON_SUPPORT_POWER_SAVE
            .enable_power_save = true,
#endif
        },
    };
    button_handle_t btn = iot_button_create(&btn_cfg);
    assert(btn);
    esp_err_t err = iot_button_register_cb(btn, BUTTON_PRESS_DOWN, button_event_cb, (void *)BUTTON_PRESS_DOWN);
    err |= iot_button_register_cb(btn, BUTTON_PRESS_UP, button_event_cb, (void *)BUTTON_PRESS_UP);
    err |= iot_button_register_cb(btn, BUTTON_PRESS_REPEAT, button_event_cb, (void *)BUTTON_PRESS_REPEAT);
    err |= iot_button_register_cb(btn, BUTTON_PRESS_REPEAT_DONE, button_event_cb, (void *)BUTTON_PRESS_REPEAT_DONE);
    err |= iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, button_event_cb, (void *)BUTTON_SINGLE_CLICK);
    err |= iot_button_register_cb(btn, BUTTON_DOUBLE_CLICK, button_event_cb, (void *)BUTTON_DOUBLE_CLICK);
    err |= iot_button_register_cb(btn, BUTTON_LONG_PRESS_START, button_event_cb, (void *)BUTTON_LONG_PRESS_START);
    err |= iot_button_register_cb(btn, BUTTON_LONG_PRESS_HOLD, button_event_cb, (void *)BUTTON_LONG_PRESS_HOLD);
    err |= iot_button_register_cb(btn, BUTTON_LONG_PRESS_UP, button_event_cb, (void *)BUTTON_LONG_PRESS_UP);
    err |= iot_button_register_cb(btn, BUTTON_PRESS_END, button_event_cb, (void *)BUTTON_PRESS_END);

#if CONFIG_ENTER_LIGHT_SLEEP_MODE_MANUALLY
    /*!< For enter Power Save */
    button_power_save_config_t config = {
        .enter_power_save_cb = button_enter_power_save,
    };
    err |= iot_button_register_power_save_cb(&config);
#endif

    ESP_ERROR_CHECK(err);
}

void create_init_ui(void) {
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, 
        "1.Double-click boot button\n"
        "or screen to enter\n"
        "the loop interface.\n\n"
        "2.Single click/tap to manually\n"
        "switch to the next picture.\n\n"
        "3.Long press to change the\n"
        "backlight status.");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(label);

    setup_touch_layer(lv_scr_act());
}


void app_main(void)
{
    /* LCD HW initialization */
    ESP_ERROR_CHECK(app_lcd_init());

    /* Touch initialization */
    ESP_ERROR_CHECK(app_touch_init());

    /* LVGL initialization */
    ESP_ERROR_CHECK(app_lvgl_init());


    button_init(BOOT_BUTTON_NUM);

    /* Show LVGL objects */
    // app_main_display();
    lvgl_port_lock(0);
    // lv_demo_stress();
    // lv_demo_widgets();
    create_init_ui();
    current_mode = UI_MODE_DEMO;
    lvgl_port_unlock();
    ESP_ERROR_CHECK(gpio_set_level(EXAMPLE_LCD_GPIO_BL, EXAMPLE_LCD_BL_ON_LEVEL));
}
