/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for LoadStore exception handlers. This test performs unaligned load and store in 32bit aligned addresses
*/

#شامل "sdkconfig.h"

#اگر CONFIG_IDF_ہدف_ARCH_XTENSA && CONFIG_ESP32_IRAM_AS_8_ثنائی_ہندسہ_ACCESSIBLE_حافظہ

#شامل <esp_types.h>
#شامل <stdio.h>
#شامل <esp_heap_caps.h>
#شامل "esp_attr.h"
#شامل "esp_random.h"
#شامل "esp_intr_alloc.h"
#شامل "xtensa_api.h"
#شامل "esp_rom_sys.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل <string.h>
#شامل "unity.h"
#شامل "esp_log_buffer.h"

#تعریف_کرو SW_ISR_عدد_L1  7    // CPU interrupt number for internal SW0 (level 1)
#تعریف_کرو SW_ISR_عدد_L3  29   // CPU interrupt number for internal SW1 (level 3)

قسمتعریف ساخت {
    uint8_t  b8;
    uint16_t b16;
    uint8_t  b8_2;
    uint32_t b32;
} iram_ڈیٹا_t;

ساکن متلون iram_ڈیٹا_t *s_iram;
ساکن متلون صحیح isr_کامیابیاں;

TEST_CASE("LoadStore Exception handler", "[freertos]")
{
    int32_t val0 = 0xDEADBEEF;
    int32_t val1 = 0xBBAA9988;
    int32_t val2 = 0x77665544;
    int32_t val3 = 0x33221100;

    int8_t val8_0 = val0 & 0xff;
    int8_t val8_1 = val1 & 0xff;
    int8_t val8_2 = val2 & 0xff;
    int8_t val8_3 = val3 & 0xff;

    int16_t val16_0 = val0 & 0xffff;
    int16_t val16_1 = val1 & 0xffff;
    int16_t val16_2 = val2 & 0xffff;
    int16_t val16_3 = val3 & 0xffff;

    uint32_t سب_سے_بڑا_free = ڈھیر_caps_get_سب_سے_بڑا_free_بلاک(MALLOC_CAP_IRAM_8_ثنائی_ہندسہ);

    int8_t *arr = ڈھیر_caps_malloc(سب_سے_بڑا_free * حجم(int8_t), MALLOC_CAP_IRAM_8_ثنائی_ہندسہ);
    TEST_ASSERT(arr != تہی);

    int8_t *arr8 =  arr;
    int16_t *arr16 = (int16_t *)arr;
    int32_t *arr32 = (int32_t *)arr;

    برائے (صحیح i = 0; i < 1024; i++) {

        // LoadStoreError

        uint32_t انحراف = esp_random() % (سب_سے_بڑا_free - 20);
        uint32_t offset8, offset16, offset32;

        // Get word aligned offset
        offset8 = انحراف & ~3;
        offset16 = offset8 / 2;
        offset32 = offset8 / 4;

        arr8[offset8] = val8_0;
        arr8[offset8 + 1] = val8_1;
        arr8[offset8 + 2] = val8_2;
        arr8[offset8 + 3] = val8_3;

        // Just to make sure compiler doesn't read stale data
        asm متلون("memw\n");
        TEST_ASSERT_EQUAL(val8_0, arr8[offset8]);
        TEST_ASSERT_EQUAL(val8_1, arr8[offset8 + 1]);
        TEST_ASSERT_EQUAL(val8_2, arr8[offset8 + 2]);
        TEST_ASSERT_EQUAL(val8_3, arr8[offset8 + 3]);

        arr16[offset16] = val16_0;
        arr16[offset16 + 1] = val16_1;
        arr16[offset16 + 2] = val16_2;
        arr16[offset16 + 3] = val16_3;

        // Just to make sure compiler doesn't read stale data
        asm متلون("memw\n");
        TEST_ASSERT_EQUAL(val16_0, arr16[offset16]);
        TEST_ASSERT_EQUAL(val16_1, arr16[offset16 + 1]);
        TEST_ASSERT_EQUAL(val16_2, arr16[offset16 + 2]);
        TEST_ASSERT_EQUAL(val16_3, arr16[offset16 + 3]);

        // LoadStoreAlignement Error

        // Check that it doesn't write to adjacent bytes
        int8_t *ptr8_0 = (خالی *)&arr8[offset8];
        int8_t *ptr8_1 = (خالی *)&arr8[offset8] + 5;
        int8_t *ptr8_2 = (خالی *)&arr8[offset8] + 10;
        int8_t *ptr8_3 = (خالی *)&arr8[offset8] + 15;

        *ptr8_0 = 0x73;
        *ptr8_1 = 0x73;
        *ptr8_2 = 0x73;
        *ptr8_3 = 0x73;

        int16_t *ptr16_0 = (خالی *)&arr16[offset16] + 1;
        int16_t *ptr16_1 = (خالی *)&arr16[offset16] + 3;

        *ptr16_0 = val16_0;
        *ptr16_1 = val16_1;

        // Just to make sure compiler doesn't read stale data
        asm متلون("memw\n");
        TEST_ASSERT_EQUAL(val16_0, *ptr16_0);
        TEST_ASSERT_EQUAL(0x73, *ptr8_0);
        TEST_ASSERT_EQUAL(val16_1, *ptr16_1);
        TEST_ASSERT_EQUAL(0x73, *ptr8_1);

        int32_t *ptr32_0 = (خالی *)&arr32[offset32] + 1;
        int32_t *ptr32_1 = (خالی *)&arr32[offset32] + 6;
        int32_t *ptr32_2 = (خالی *)&arr32[offset32] + 11;
        *ptr32_0 = val0;
        *ptr32_1 = val1;
        *ptr32_2 = val2;

        // Just to make sure compiler doesn't read stale data
        asm متلون("memw");
        TEST_ASSERT_EQUAL(0x73, *ptr8_0);
        TEST_ASSERT_EQUAL(val0, *ptr32_0);
        TEST_ASSERT_EQUAL(0x73, *ptr8_1);
        TEST_ASSERT_EQUAL(val1, *ptr32_1);
        TEST_ASSERT_EQUAL(0x73, *ptr8_2);
        TEST_ASSERT_EQUAL(val2, *ptr32_2);
        TEST_ASSERT_EQUAL(0x73, *ptr8_3);
    }

    TEST_ASSERT_TRUE(ڈھیر_caps_check_integrity_all(true));
    ڈھیر_caps_free(arr);
}

ساکن خالی IRAM_ATTR level_isr(خالی *arg)
{
    ثابت صحیح cpu_intr_عدد = *(ثابت صحیح *)arg;

    // Clear the SW interrupt source
    xt_سیٹ_intclear(1 << cpu_intr_عدد);

    // Schedule another pending level interrupt while still inside this ISR
    // to keep dispatch_c_isr in the interrupt loop handling. The pending
    // interrupt will be handled after this ISR returns.
    اگر (isr_کامیابیاں == 0) {
        xt_سیٹ_intset(1 << cpu_intr_عدد);
    }

    // 8/16-bit accesses to s_iram will trigger LoadStore/Alignment exceptions which changes the EXCSAVE_1 value
    s_iram->b16++;
    s_iram->b8 = 0x5A;
    s_iram->b8_2 = 0xA5;

    isr_کامیابیاں++;
}

ساکن خالی تسلسل_sw_isr_level(صحیح int_level)
{
    memset((خالی *)s_iram, 0, حجم(iram_ڈیٹا_t));
    isr_کامیابیاں = 0;

    intr_handle_t handle;
    صحیح cpu_intr_عدد = (int_level == 1) ? SW_ISR_عدد_L1 : SW_ISR_عدد_L3;
    TEST_ASSERT_EQUAL(ESP_OK, esp_intr_alloc((int_level == 1) ? ETS_اندرونی_SW0_INTR_ماخذ : ETS_اندرونی_SW1_INTR_ماخذ,
                                             (int_level == 1) ? ESP_INTR_FLAG_LEVEL1 : ESP_INTR_FLAG_LEVEL3,
                                             level_isr,
                                             (خالی *)&cpu_intr_عدد,
                                             &handle));

    // Trigger the first interrupt; ISR will queue one more while running
    xt_سیٹ_intset(1 << cpu_intr_عدد);

    // Wait for the ISR to be invoked twice
    TickType_t آغاز = x_Task_Get_Tick_گنتی();
    جب (isr_کامیابیاں < 2 && (x_Task_Get_Tick_گنتی() - آغاز) < pd_MS_مقصد_TICKS(500)) {
        vTaskDelay(1);
    }

    TEST_ASSERT_EQUAL(2, isr_کامیابیاں);
    TEST_ASSERT_EQUAL_HEX8(0x5A, s_iram->b8);
    TEST_ASSERT_EQUAL_HEX8(0xA5, s_iram->b8_2);
    TEST_ASSERT_EQUAL_UINT16(2, s_iram->b16);
    TEST_ASSERT_EQUAL_UINT32(0, s_iram->b32);

    TEST_ESP_OK(esp_intr_free(handle));
}

TEST_CASE("LoadStore: 8/16-bit field access in IRAM from ISRs when pending interrupts are present", "[freertos]")
{
    s_iram = ڈھیر_caps_calloc(1, حجم(*s_iram), MALLOC_CAP_IRAM_8_ثنائی_ہندسہ);
    TEST_ASSERT_NOT_NULL(s_iram);

    esp_rom_printf("Running test in level-1 ISR...\n");
    تسلسل_sw_isr_level(1);
    esp_rom_printf("test passed\n");

    esp_rom_printf("Running test in level-3 ISR...\n");
    تسلسل_sw_isr_level(3);
    esp_rom_printf("test passed\n");

    ڈھیر_caps_free((خالی *)s_iram);
    vTaskDelay(100); // Wait in ticks for memory to be freed, to avoid affecting other tests.
}

TEST_CASE("LoadStore: zero-overhead loop continues after IRAM 8-bit store", "[freertos]")
{
    ثابت غیرعلامتی len = 32;

    uint8_t *dst = ڈھیر_caps_calloc(len, 1, MALLOC_CAP_IRAM_8_ثنائی_ہندسہ);
    TEST_ASSERT_NOT_NULL(dst);

    uint8_t *src = ڈھیر_caps_calloc(len, 1, MALLOC_CAP_IRAM_8_ثنائی_ہندسہ);
    TEST_ASSERT_NOT_NULL(src);

    برائے (غیرعلامتی i = 0; i < len; i++) {
        src[i] = i + 1;
        dst[i] = 0xCC;
    }

    ESP_LOG_بفر_HEX("dst before", dst, len);
    ESP_LOG_بفر_HEX("src       ", src, len);

    /*
    * Use Xtensa zero-overhead loop where the final instruction (LEND)
    * is an 8-bit store into IRAM. With CONFIG_ESP32_IRAM_AS_8BIT_ACCESSIBLE_MEMORY
    * each store triggers the LoadStoreError handler. The handler must mimic
    * the loop hardware to keep iterating; otherwise the loop exits after one
    * iteration. This assembly below performs a simple byte copy from src to dst,
    * and the test verifies that all bytes are copied correctly,
    * indicating that the loop continued to execute after each store exception.
    */
    uint32_t tmp_val;
    uint8_t *dst_it = dst;
    uint8_t *src_it = src;
    غیرعلامتی cnt = len;
    __asm__ متلون(
        "addi       %0, %0, -1\n"   /* dst-- */
        "addi       %1, %1, -1\n"   /* src-- */
        "loopnez    %2, .endLoop\n"
        "addi       %1, %1, 1\n"   /* src++ */
        "l8ui       %3, %1, 0\n"   /* tmp_val = src[] */
        "addi       %0, %0, 1\n"   /* dst++ */
        "s8i        %3, %0, 0\n"   /* dst[] = tmp_val, LEND: store is last in loop body */
        ".endLoop:\n"
        : "+r"(dst_it), "+r"(src_it), "+r"(cnt), "=&r"(tmp_val)
        :
        : "memory"
    );

    ESP_LOG_بفر_HEX("dst  after", dst, len);

    برائے (غیرعلامتی i = 0; i < len; i++) {
        TEST_ASSERT_EQUAL_HEX8(src[i], dst[i]);
    }

    ڈھیر_caps_free(dst);
    ڈھیر_caps_free(src);

    vTaskDelay(100); // Wait for free to complete before running other tests.
}
#اگر_ختم // CONFIG_IDF_TARGET_ARCH_XTENSA && CONFIG_ESP32_IRAM_AS_8BIT_ACCESSIBLE_MEMORY
