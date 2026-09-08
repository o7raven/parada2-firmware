#include "../../third_party/unity/unity.h"
#include "../../src/system/state_machine.h"
#include <stdbool.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_state_machine_init(void)
{
    state_machine_t sm;
    system_context_t ctx;

    status_t result = state_machine_init(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATUS_OK, result);

    TEST_ASSERT_EQUAL(STATE_RUN, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_RUN, sm.previous_state);
    TEST_ASSERT_EQUAL_UINT32(0, sm.state_time_ms);

    TEST_ASSERT_FALSE(ctx.sensors_ok);
    TEST_ASSERT_FALSE(ctx.power_ok);
    TEST_ASSERT_FALSE(ctx.radio_ok);
}

/*
void test_state_machine_step_check_correct(void){
    state_machine_t sm = {STATE_CHECK, STATE_CHECK, 100};
    system_context_t ctx = {true, true, true, false, false, false, false};
    
    state_machine_step(&sm, &ctx);
    TEST_ASSERT_EQUAL(STATE_RUN, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_CHECK, sm.previous_state);
    
    TEST_ASSERT_TRUE(ctx.sensors_ok);
    TEST_ASSERT_TRUE(ctx.power_ok);
    TEST_ASSERT_TRUE(ctx.radio_ok);

    TEST_ASSERT_FALSE(ctx.gps_found);
    TEST_ASSERT_FALSE(ctx.radio_connected);
    TEST_ASSERT_FALSE(ctx.critical_fault);
    TEST_ASSERT_FALSE(ctx.low_battery);
    TEST_ASSERT_EQUAL(0, sm.state_time_ms);
}

void test_state_machine_step_check_ctx_sensor_fail(void){
    state_machine_t sm = {STATE_CHECK, STATE_CHECK, 0};
    system_context_t ctx = {false, true, true, false, false, false, false};
    
    state_machine_step(&sm, &ctx);
    TEST_ASSERT_EQUAL(STATE_SAFE, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_CHECK, sm.previous_state);
    
    TEST_ASSERT_FALSE(ctx.sensors_ok);
    TEST_ASSERT_TRUE(ctx.power_ok);
    TEST_ASSERT_TRUE(ctx.radio_ok);
    
    TEST_ASSERT_FALSE(ctx.gps_found);
    TEST_ASSERT_FALSE(ctx.radio_connected);
    TEST_ASSERT_FALSE(ctx.critical_fault);
    TEST_ASSERT_FALSE(ctx.low_battery);
    TEST_ASSERT_EQUAL(0, sm.state_time_ms);
}

void test_state_machine_step_check_ctx_radio_fail(void){
    state_machine_t sm = {STATE_CHECK, STATE_CHECK, 0};
    system_context_t ctx = {true, false, true, false, false, false, false};
    
    state_machine_step(&sm, &ctx);
    TEST_ASSERT_EQUAL(STATE_SAFE, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_CHECK, sm.previous_state);
    
    TEST_ASSERT_FALSE(ctx.radio_ok);
    TEST_ASSERT_TRUE(ctx.power_ok);
    TEST_ASSERT_TRUE(ctx.sensors_ok);
    
    TEST_ASSERT_FALSE(ctx.gps_found);
    TEST_ASSERT_FALSE(ctx.radio_connected);
    TEST_ASSERT_FALSE(ctx.critical_fault);
    TEST_ASSERT_FALSE(ctx.low_battery);
    TEST_ASSERT_EQUAL(0, sm.state_time_ms);
}
void test_state_machine_step_check_ctx_power_fail(void){
    state_machine_t sm = {STATE_CHECK, STATE_CHECK, 0};
    system_context_t ctx = {true, true, false, false, false, false, false};
    
    state_machine_step(&sm, &ctx);
    TEST_ASSERT_EQUAL(STATE_SAFE, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_CHECK, sm.previous_state);
    
    TEST_ASSERT_FALSE(ctx.power_ok);
    TEST_ASSERT_TRUE(ctx.radio_ok);
    TEST_ASSERT_TRUE(ctx.sensors_ok);
    
    TEST_ASSERT_FALSE(ctx.gps_found);
    TEST_ASSERT_FALSE(ctx.radio_connected);
    TEST_ASSERT_FALSE(ctx.critical_fault);
    TEST_ASSERT_FALSE(ctx.low_battery);
    TEST_ASSERT_EQUAL(0, sm.state_time_ms);
}

void test_state_machine_step_run_from_check(void){
    state_machine_t sm = {STATE_RUN, STATE_CHECK, 100};
    system_context_t ctx = {true, true, true, false, false, true, true};

    state_machine_step(&sm, &ctx);
    TEST_ASSERT_EQUAL(STATE_RUN, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_CHECK, sm.previous_state);
    TEST_ASSERT_EQUAL(100, sm.state_time_ms);

    TEST_ASSERT_TRUE(ctx.power_ok);
    TEST_ASSERT_TRUE(ctx.sensors_ok);
    TEST_ASSERT_TRUE(ctx.radio_ok);
    TEST_ASSERT_TRUE(ctx.radio_connected);
    TEST_ASSERT_TRUE(ctx.gps_found);

    TEST_ASSERT_FALSE(ctx.critical_fault);
    TEST_ASSERT_FALSE(ctx.low_battery);


}
    */
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_state_machine_init);
    /*
    RUN_TEST(test_state_machine_step_check_correct);
    RUN_TEST(test_state_machine_step_check_ctx_sensor_fail);
    RUN_TEST(test_state_machine_step_check_ctx_radio_fail);
    RUN_TEST(test_state_machine_step_check_ctx_power_fail);
    RUN_TEST(test_state_machine_step_run_from_check);
    */

    return UNITY_END();
}