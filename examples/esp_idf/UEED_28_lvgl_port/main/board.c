/*
 *November 2024
 *Author: LiuLong
 *Shenzhen VIEWE TECHNOLOGY Co., LTD
 */

#include "board.h"

static const char *TAG = "boardconfig";

/* LCD IO and panel */
static esp_lcd_panel_io_handle_t lcd_io = NULL;
static esp_lcd_panel_handle_t lcd_panel = NULL;
static esp_lcd_touch_handle_t touch_handle = NULL;

/* LVGL display and touch */
static lv_display_t *lvgl_disp = NULL;
static lv_indev_t *lvgl_touch_indev = NULL;

#if  VIEWE_35_T //3.5半透半反初始化
static const st7796_lcd_init_cmd_t lcd_init_cmds[] = {
// {cmd, { data }, data_size, delay_ms}
 //3.5半透半反老屏CMD
    {0x11, (uint8_t []){0x00}, 0, 120},
    {0xF0, (uint8_t []){0xC3}, 1, 0},
    {0xF0, (uint8_t []){0x96}, 1, 0},
    {0x36, (uint8_t []){0x28}, 1, 0},
    {0x3A, (uint8_t []){0x55}, 1, 0},

    
    {0xB4, (uint8_t []){0x01}, 1, 0},
    {0xB7, (uint8_t []){0xC6}, 1, 0},
    {0xC0, (uint8_t []){0x80,0x04}, 2, 0},

    {0xC1, (uint8_t []){0x13}, 1, 0},
    {0xC2, (uint8_t []){0xA7}, 1, 0},
    {0xC5, (uint8_t []){0x16}, 1, 0},


    {0xE8, (uint8_t []){0x40,0x8A,0x00,0x00,0x29,0x19,0xA5,0x33}, 8, 0},
    {0xE0, (uint8_t []){0xF0,0x19,0x20,0x10,0x11,0x0A,0x46,0x44,0x57,0x09,0x1A,0x1B,0x2A,0x2D}, 14, 0},
    {0xE1, (uint8_t []){0xF0,0x12,0x1A,0x0A,0x0C,0x18,0x45,0x44,0x56,0x3F,0x15,0x11,0x24,0x26}, 14, 0},


    {0xF0, (uint8_t []){0xC3}, 1, 0},
    {0xF0, (uint8_t []){0x69}, 1, 0},

    {0x21, (uint8_t []){0x00}, 0, 0},
    {0x29, (uint8_t []){0x00}, 0, 0},
    {0x2c, (uint8_t []){0x00}, 0, 0},

  //3.5半透半反新屏CMD
    // {0x11, (uint8_t []){0x00}, 0, 120},
    // {0x36, (uint8_t []){0x28}, 1, 0},
    // {0x3A, (uint8_t []){0x55}, 1, 0},
    // {0xF0, (uint8_t []){0xC3}, 1, 0},
    // {0xF0, (uint8_t []){0x96}, 1, 0},

    // {0xB4, (uint8_t []){0x02}, 1, 0},
    // {0xB7, (uint8_t []){0xC6}, 1, 0},
    // {0xB9, (uint8_t []){0x02,0xE0}, 2, 0},
    // {0xC0, (uint8_t []){0x80,0x16}, 2, 0},

    // {0xC1, (uint8_t []){0x13}, 1, 0},
    // {0xC2, (uint8_t []){0xA7}, 1, 0},

    // {0xC5, (uint8_t []){0x13}, 1, 0},

    // {0xE8, (uint8_t []){0x40,0x8A,0x00,0x00,0x25,0x0A,0x38,0x33}, 8, 0},
    // {0xE0, (uint8_t []){0xF0,0x01,0x05,0x02,0x00,0x00,0x29,0x44,0x3B,0x10,0x03,0x03,0x16,0x25}, 14, 0},
    // {0xE1, (uint8_t []){0xF0,0x0D,0x11,0x0D,0x0B,0x27,0x28,0x44,0x3B,0x39,0x0F,0x0F,0x21,0x31}, 14, 0},

    // {0x2A, (uint8_t []){0x00,0x00,0x01,0xDF}, 4, 0},
    // {0x2B, (uint8_t []){0x00,0x00,0x01,0x3F}, 4, 0},

    // {0xF0, (uint8_t []){0x3C}, 1, 0},
    // {0xF0, (uint8_t []){0x69}, 1, 120},

    // {0x21, (uint8_t []){0x00}, 0, 0},
    // {0x29, (uint8_t []){0x00}, 0, 0},
    // {0x2c, (uint8_t []){0x00}, 0, 0},
};
   

#elif VIEWE_24_T || VIEWE_28_T  //半透半反
static const gc9a01_lcd_init_cmd_t lcd_init_cmds[] = {
    {0x11, (uint8_t []){0x00}, 0, 120},
    {0x36, (uint8_t []){0x60}, 1, 0},
    {0x20, (uint8_t []){0x00}, 0, 0},

    {0x3A, (uint8_t []){0x05}, 1, 0},
    {0xB7, (uint8_t []){0x35}, 1, 0},

    {0xBB, (uint8_t []){0x06}, 1, 0},

    {0xC0, (uint8_t []){0x2C}, 1, 0},
    {0xC2, (uint8_t []){0x01,0xFF}, 2, 0},
    {0xC3, (uint8_t []){0x13}, 1, 0},
    {0xC6, (uint8_t []){0x0F}, 1, 0},

    {0xE0, (uint8_t []){ 0x70,0x27,0x28,0x07,0x04,0x27,0x38,0x33,0x56,0x3A,0x13,0x10,0x25,0x27}, 14, 0},
    {0xE1, (uint8_t []){ 0x70,0x1C,0x1D,0x00,0x00,0x09,0x44,0x47,0x4C,0x0D,0x1D,0x19,0x20,0x25}, 14, 0},

    {0x11, (uint8_t []){0x00}, 0, 120},
    {0x29, (uint8_t []){0x00}, 0, 120},
    {0x2c, (uint8_t []){0x00}, 0, 0},

};
#endif

esp_err_t app_lcd_init(void)
{
    esp_err_t ret = ESP_OK;

    /* LCD backlight */
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << EXAMPLE_LCD_GPIO_BL
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    gpio_config_t im1_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << EXAMPLE_PIN_NUM_IM1
    };
    ESP_ERROR_CHECK(gpio_config(&im1_gpio_config));
    gpio_config_t im0_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << EXAMPLE_PIN_NUM_IM0
    };
    ESP_ERROR_CHECK(gpio_config(&im0_gpio_config));
#if VIEWE_35_T
    ESP_ERROR_CHECK(gpio_set_level(EXAMPLE_PIN_NUM_IM0, EXAMPLE_PIN_NUM_IM_ON_LEVEL));
    ESP_ERROR_CHECK(gpio_set_level(EXAMPLE_PIN_NUM_IM1, EXAMPLE_PIN_NUM_IM_ON_LEVEL));
#elif VIEWE_24_T || VIEWE_28_T
    ESP_ERROR_CHECK(gpio_set_level(EXAMPLE_PIN_NUM_IM0, EXAMPLE_PIN_NUM_IM_OFF_LEVEL));
    ESP_ERROR_CHECK(gpio_set_level(EXAMPLE_PIN_NUM_IM1, EXAMPLE_PIN_NUM_IM_ON_LEVEL));
#endif
    /* LCD initialization */
    ESP_LOGD(TAG, "Initialize SPI bus");
    const spi_bus_config_t buscfg = {
        .sclk_io_num = EXAMPLE_LCD_GPIO_SCLK,
        .mosi_io_num = EXAMPLE_LCD_GPIO_MOSI,
        .miso_io_num = GPIO_NUM_NC,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES * sizeof(uint16_t),
    };
    ESP_RETURN_ON_ERROR(spi_bus_initialize(EXAMPLE_LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO), TAG, "SPI init failed");

    ESP_LOGD(TAG, "Install panel IO");
    const esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = EXAMPLE_LCD_GPIO_DC,
        .cs_gpio_num = EXAMPLE_LCD_GPIO_CS,
        .pclk_hz = EXAMPLE_LCD_PIXEL_CLK_HZ,
        .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
        .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)EXAMPLE_LCD_SPI_NUM, &io_config, &lcd_io), err, TAG, "New panel IO failed");

    ESP_LOGD(TAG, "Install LCD driver");

    #if VIEWE_24_T || VIEWE_28_T
        gc9a01_vendor_config_t vendor_config = {  // Uncomment these lines if use custom initialization commands
            .init_cmds = lcd_init_cmds,
            .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(gc9a01_lcd_init_cmd_t),
        };
    #elif VIEWE_35_T
        st7796_vendor_config_t vendor_config = {  // Uncomment these lines if use custom initialization commands
            .init_cmds = lcd_init_cmds,
            .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(st7796_lcd_init_cmd_t),
        };
    #endif

    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_LCD_GPIO_RST,
        .color_space = EXAMPLE_LCD_COLOR_SPACE,
        .bits_per_pixel = EXAMPLE_LCD_BITS_PER_PIXEL,
        .vendor_config = &vendor_config,            // Uncomment this line if use custom initialization commands
    };
#if VIEWE_24_T || VIEWE_28_T
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_gc9a01(lcd_io, &panel_config, &lcd_panel), err, TAG, "New panel failed");
#elif VIEWE_35_T
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_st7796(lcd_io, &panel_config, &lcd_panel), err, TAG, "New panel failed");
#endif

    esp_lcd_panel_reset(lcd_panel);
    esp_lcd_panel_init(lcd_panel);
    // esp_lcd_panel_swap_xy(lcd_panel, true);
    esp_lcd_panel_mirror(lcd_panel, true, false);

#if VIEWE_35_T || VIEWE_28_T
    esp_lcd_panel_invert_color(lcd_panel, true);
#else
    esp_lcd_panel_invert_color(lcd_panel, false);
#endif
    esp_lcd_panel_disp_on_off(lcd_panel, true);

    /* LCD backlight on */
    // ESP_ERROR_CHECK(gpio_set_level(EXAMPLE_LCD_GPIO_BL, EXAMPLE_LCD_BL_ON_LEVEL));
    
    return ret;

err:
    if (lcd_panel) {
        esp_lcd_panel_del(lcd_panel);
    }
    if (lcd_io) {
        esp_lcd_panel_io_del(lcd_io);
    }
    spi_bus_free(EXAMPLE_LCD_SPI_NUM);
    return ret;
}

esp_err_t app_touch_init(void)
{
    /* Initilize I2C */
    const i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = EXAMPLE_TOUCH_I2C_SDA,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_io_num = EXAMPLE_TOUCH_I2C_SCL,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = EXAMPLE_TOUCH_I2C_CLK_HZ
    };
    ESP_RETURN_ON_ERROR(i2c_param_config(EXAMPLE_TOUCH_I2C_NUM, &i2c_conf), TAG, "I2C configuration failed");
    ESP_RETURN_ON_ERROR(i2c_driver_install(EXAMPLE_TOUCH_I2C_NUM, i2c_conf.mode, 0, 0, 0), TAG, "I2C initialization failed");

    /* Initialize touch HW */
    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = EXAMPLE_LCD_H_RES,
        .y_max = EXAMPLE_LCD_V_RES,
        .rst_gpio_num = GPIO_NUM_NC, // Shared with LCD reset
        .int_gpio_num = EXAMPLE_TOUCH_GPIO_INT,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {//屏幕方向一般在这里改
            .swap_xy = false,//false,  屏幕方向一般在这里改
            .mirror_x = false,//true,
            .mirror_y = false,
        },
    };
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    const esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_CST816S_CONFIG();
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)EXAMPLE_TOUCH_I2C_NUM, &tp_io_config, &tp_io_handle), TAG, "");
    return esp_lcd_touch_new_i2c_cst816s(tp_io_handle, &tp_cfg, &touch_handle);
}

esp_err_t app_lvgl_init(void)
{
    /* Initialize LVGL */
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 4,         /* LVGL task priority */
        .task_stack = 7096,         /* LVGL task stack size */
        .task_affinity = -1,        /* LVGL task pinned to core (-1 is no affinity) */
        .task_max_sleep_ms = 500,   /* Maximum sleep in LVGL task */
        .timer_period_ms = 5        /* LVGL timer tick period in ms */
    };
    ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "LVGL port initialization failed");

    /* Add LCD screen */
    ESP_LOGD(TAG, "Add LCD screen");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = lcd_io,
        .panel_handle = lcd_panel,
        .buffer_size = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DRAW_BUFF_HEIGHT,
        .double_buffer = EXAMPLE_LCD_DRAW_BUFF_DOUBLE,
        .hres = EXAMPLE_LCD_H_RES,
        .vres = EXAMPLE_LCD_V_RES,
        .monochrome = false,
#if LVGL_VERSION_MAJOR >= 9
        .color_format = LV_COLOR_FORMAT_RGB565,
#endif
        .rotation = {
            .swap_xy = false,//false,  屏幕方向一般在这里改
#if VIEWE_35_T
            .mirror_x = true,//true,
#else
            .mirror_x = false,//true,
#endif
            .mirror_y = false,
        },
        .flags = {
            .buff_dma = true,
#if LVGL_VERSION_MAJOR >= 9
            .swap_bytes = true,
#endif
        }
    };
    lvgl_disp = lvgl_port_add_disp(&disp_cfg);

    /* Add touch input (for selected screen) */
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = lvgl_disp,
        .handle = touch_handle,
    };
    lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);

    return ESP_OK;
}