/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for LoadStore exception handlers. This test performs unaligned load and store in 32bit aligned addresses
*/

#అన్తర్భావ "sdkconfig.h"

#ఒకవేళ CONFIG_IDF_లక్ష్యం_ARCH_XTENSA && CONFIG_ESP32_IRAM_AS_8_బిట్టు_ACCESSIBLE_జ్ఞాపకం

#అన్తర్భావ <esp_types.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <esp_heap_caps.h>
#అన్తర్భావ "esp_attr.h"
#అన్తర్భావ "esp_random.h"
#అన్తర్భావ "esp_intr_alloc.h"
#అన్తర్భావ "xtensa_api.h"
#అన్తర్భావ "esp_rom_sys.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ "unity.h"
#అన్తర్భావ "esp_log_buffer.h"

#నిర్వచించు SW_ISR_అంకె_L1  7    // CPU interrupt number for internal SW0 (level 1)
#నిర్వచించు SW_ISR_అంకె_L3  29   // CPU interrupt number for internal SW1 (level 3)

రకనిర్వచనం నిర్మాణం {
    uint8_t  b8;
    uint16_t b16;
    uint8_t  b8_2;
    uint32_t b32;
} iram_దత్తాంశం_t;

స్థిర అస్థిర iram_దత్తాంశం_t *s_iram;
స్థిర అస్థిర పూర్ణాంకం isr_తగిలినవి;

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

    uint32_t అతిపెద్ద_free = కుప్ప_caps_get_అతిపెద్ద_free_ఖండం(MALLOC_CAP_IRAM_8_బిట్టు);

    int8_t *arr = కుప్ప_caps_malloc(అతిపెద్ద_free * పరిమాణం(int8_t), MALLOC_CAP_IRAM_8_బిట్టు);
    TEST_ASSERT(arr != ఖాళీ);

    int8_t *arr8 =  arr;
    int16_t *arr16 = (int16_t *)arr;
    int32_t *arr32 = (int32_t *)arr;

    కొరకు (పూర్ణాంకం i = 0; i < 1024; i++) {

        // LoadStoreError

        uint32_t స్థానభ్రంశం = esp_random() % (అతిపెద్ద_free - 20);
        uint32_t offset8, offset16, offset32;

        // Get word aligned offset
        offset8 = స్థానభ్రంశం & ~3;
        offset16 = offset8 / 2;
        offset32 = offset8 / 4;

        arr8[offset8] = val8_0;
        arr8[offset8 + 1] = val8_1;
        arr8[offset8 + 2] = val8_2;
        arr8[offset8 + 3] = val8_3;

        // Just to make sure compiler doesn't read stale data
        asm అస్థిర("memw\n");
        TEST_ASSERT_EQUAL(val8_0, arr8[offset8]);
        TEST_ASSERT_EQUAL(val8_1, arr8[offset8 + 1]);
        TEST_ASSERT_EQUAL(val8_2, arr8[offset8 + 2]);
        TEST_ASSERT_EQUAL(val8_3, arr8[offset8 + 3]);

        arr16[offset16] = val16_0;
        arr16[offset16 + 1] = val16_1;
        arr16[offset16 + 2] = val16_2;
        arr16[offset16 + 3] = val16_3;

        // Just to make sure compiler doesn't read stale data
        asm అస్థిర("memw\n");
        TEST_ASSERT_EQUAL(val16_0, arr16[offset16]);
        TEST_ASSERT_EQUAL(val16_1, arr16[offset16 + 1]);
        TEST_ASSERT_EQUAL(val16_2, arr16[offset16 + 2]);
        TEST_ASSERT_EQUAL(val16_3, arr16[offset16 + 3]);

        // LoadStoreAlignement Error

        // Check that it doesn't write to adjacent bytes
        int8_t *ptr8_0 = (శూన్య *)&arr8[offset8];
        int8_t *ptr8_1 = (శూన్య *)&arr8[offset8] + 5;
        int8_t *ptr8_2 = (శూన్య *)&arr8[offset8] + 10;
        int8_t *ptr8_3 = (శూన్య *)&arr8[offset8] + 15;

        *ptr8_0 = 0x73;
        *ptr8_1 = 0x73;
        *ptr8_2 = 0x73;
        *ptr8_3 = 0x73;

        int16_t *ptr16_0 = (శూన్య *)&arr16[offset16] + 1;
        int16_t *ptr16_1 = (శూన్య *)&arr16[offset16] + 3;

        *ptr16_0 = val16_0;
        *ptr16_1 = val16_1;

        // Just to make sure compiler doesn't read stale data
        asm అస్థిర("memw\n");
        TEST_ASSERT_EQUAL(val16_0, *ptr16_0);
        TEST_ASSERT_EQUAL(0x73, *ptr8_0);
        TEST_ASSERT_EQUAL(val16_1, *ptr16_1);
        TEST_ASSERT_EQUAL(0x73, *ptr8_1);

        int32_t *ptr32_0 = (శూన్య *)&arr32[offset32] + 1;
        int32_t *ptr32_1 = (శూన్య *)&arr32[offset32] + 6;
        int32_t *ptr32_2 = (శూన్య *)&arr32[offset32] + 11;
        *ptr32_0 = val0;
        *ptr32_1 = val1;
        *ptr32_2 = val2;

        // Just to make sure compiler doesn't read stale data
        asm అస్థిర("memw");
        TEST_ASSERT_EQUAL(0x73, *ptr8_0);
        TEST_ASSERT_EQUAL(val0, *ptr32_0);
        TEST_ASSERT_EQUAL(0x73, *ptr8_1);
        TEST_ASSERT_EQUAL(val1, *ptr32_1);
        TEST_ASSERT_EQUAL(0x73, *ptr8_2);
        TEST_ASSERT_EQUAL(val2, *ptr32_2);
        TEST_ASSERT_EQUAL(0x73, *ptr8_3);
    }

    TEST_ASSERT_TRUE(కుప్ప_caps_check_integrity_all(true));
    కుప్ప_caps_free(arr);
}

స్థిర శూన్య IRAM_ATTR level_isr(శూన్య *arg)
{
    స్థిరరాశి పూర్ణాంకం cpu_intr_అంకె = *(స్థిరరాశి పూర్ణాంకం *)arg;

    // Clear the SW interrupt source
    xt_సమితి_intclear(1 << cpu_intr_అంకె);

    // Schedule another pending level interrupt while still inside this ISR
    // to keep dispatch_c_isr in the interrupt loop handling. The pending
    // interrupt will be handled after this ISR returns.
    ఒకవేళ (isr_తగిలినవి == 0) {
        xt_సమితి_intset(1 << cpu_intr_అంకె);
    }

    // 8/16-bit accesses to s_iram will trigger LoadStore/Alignment exceptions which changes the EXCSAVE_1 value
    s_iram->b16++;
    s_iram->b8 = 0x5A;
    s_iram->b8_2 = 0xA5;

    isr_తగిలినవి++;
}

స్థిర శూన్య నిరంతరం_sw_isr_level(పూర్ణాంకం int_level)
{
    memset((శూన్య *)s_iram, 0, పరిమాణం(iram_దత్తాంశం_t));
    isr_తగిలినవి = 0;

    intr_handle_t handle;
    పూర్ణాంకం cpu_intr_అంకె = (int_level == 1) ? SW_ISR_అంకె_L1 : SW_ISR_అంకె_L3;
    TEST_ASSERT_EQUAL(ESP_OK, esp_intr_alloc((int_level == 1) ? ETS_అంతర్గత_SW0_INTR_ఆధారం : ETS_అంతర్గత_SW1_INTR_ఆధారం,
                                             (int_level == 1) ? ESP_INTR_FLAG_LEVEL1 : ESP_INTR_FLAG_LEVEL3,
                                             level_isr,
                                             (శూన్య *)&cpu_intr_అంకె,
                                             &handle));

    // Trigger the first interrupt; ISR will queue one more while running
    xt_సమితి_intset(1 << cpu_intr_అంకె);

    // Wait for the ISR to be invoked twice
    TickType_t ప్రారంభం = x_Task_Get_Tick_లెక్క();
    అయితే (isr_తగిలినవి < 2 && (x_Task_Get_Tick_లెక్క() - ప్రారంభం) < pd_MS_గమ్యం_TICKS(500)) {
        vTaskDelay(1);
    }

    TEST_ASSERT_EQUAL(2, isr_తగిలినవి);
    TEST_ASSERT_EQUAL_HEX8(0x5A, s_iram->b8);
    TEST_ASSERT_EQUAL_HEX8(0xA5, s_iram->b8_2);
    TEST_ASSERT_EQUAL_UINT16(2, s_iram->b16);
    TEST_ASSERT_EQUAL_UINT32(0, s_iram->b32);

    TEST_ESP_OK(esp_intr_free(handle));
}

TEST_CASE("LoadStore: 8/16-bit field access in IRAM from ISRs when pending interrupts are present", "[freertos]")
{
    s_iram = కుప్ప_caps_calloc(1, పరిమాణం(*s_iram), MALLOC_CAP_IRAM_8_బిట్టు);
    TEST_ASSERT_NOT_NULL(s_iram);

    esp_rom_printf("Running test in level-1 ISR...\n");
    నిరంతరం_sw_isr_level(1);
    esp_rom_printf("test passed\n");

    esp_rom_printf("Running test in level-3 ISR...\n");
    నిరంతరం_sw_isr_level(3);
    esp_rom_printf("test passed\n");

    కుప్ప_caps_free((శూన్య *)s_iram);
    vTaskDelay(100); // Wait in ticks for memory to be freed, to avoid affecting other tests.
}

TEST_CASE("LoadStore: zero-overhead loop continues after IRAM 8-bit store", "[freertos]")
{
    స్థిరరాశి గుర్తులేని len = 32;

    uint8_t *dst = కుప్ప_caps_calloc(len, 1, MALLOC_CAP_IRAM_8_బిట్టు);
    TEST_ASSERT_NOT_NULL(dst);

    uint8_t *src = కుప్ప_caps_calloc(len, 1, MALLOC_CAP_IRAM_8_బిట్టు);
    TEST_ASSERT_NOT_NULL(src);

    కొరకు (గుర్తులేని i = 0; i < len; i++) {
        src[i] = i + 1;
        dst[i] = 0xCC;
    }

    ESP_LOG_బఫరు_HEX("dst before", dst, len);
    ESP_LOG_బఫరు_HEX("src       ", src, len);

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
    గుర్తులేని cnt = len;
    __asm__ అస్థిర(
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

    ESP_LOG_బఫరు_HEX("dst  after", dst, len);

    కొరకు (గుర్తులేని i = 0; i < len; i++) {
        TEST_ASSERT_EQUAL_HEX8(src[i], dst[i]);
    }

    కుప్ప_caps_free(dst);
    కుప్ప_caps_free(src);

    vTaskDelay(100); // Wait for free to complete before running other tests.
}
#అయితే_ముగింపు // CONFIG_IDF_TARGET_ARCH_XTENSA && CONFIG_ESP32_IRAM_AS_8BIT_ACCESSIBLE_MEMORY
