#include "../../third_party/unity/unity.h"
#include "../../src/system/state_machine.h"
#include <stdbool.h>

/*
 * --------------------------------------------------------------------------
 * Test doubles / mocks
 * --------------------------------------------------------------------------
 *
 * These are simple fakes for reset_time(), update_time(), and log_info().
 *
 * Adjust the function signatures if your production declarations differ.
 */

static int reset_time_call_count;
static int update_time_call_count;
static uint32_t reset_time_value;
static uint32_t update_time_value;

void setUp(void)
{
    reset_time_call_count = 0;
    update_time_call_count = 0;
    reset_time_value = 0;
    update_time_value = 0;
}

void tearDown(void)
{
}

/*
 * Fake reset_time()
 */
void reset_time(uint32_t *time_ms)
{
    reset_time_call_count++;
    reset_time_value = *time_ms;
}

/*
 * Fake update_time()
 */
void update_time(uint32_t *time_ms)
{
    update_time_call_count++;
    update_time_value = *time_ms;

    /*
     * If your real update_time() increments the value, reproduce that
     * behavior here instead, for example:
     *
     * (*time_ms)++;
     */
}

/*
 * Fake log_info().
 *
 * If log_info() is provided by another module and your test build already
 * supplies a mock for it, remove this function.
 */
void log_info(const char *format, ...)
{
    (void)format;
}


/*
 * --------------------------------------------------------------------------
 * Helper
 * --------------------------------------------------------------------------
 */

static state_machine_t create_state_machine(system_state_t current_state)
{
    state_machine_t sm = {
        .current_state = current_state,
        .previous_state = STATE_ERROR,
        .state_time_ms = 100
    };

    return sm;
}


/*
 * ==========================================================================
 * STATE_RUN tests
 * ==========================================================================
 */

/*
 * RUN + no fault + no low battery + power not OK
 *
 * No transition should occur.
 * update_time() should be called.
 */
void test_state_machine_step_RUN_no_conditions_stays_RUN(void)
{
    state_machine_t sm = create_state_machine(STATE_RUN);

    system_context_t ctx = {
        .critical_fault = false,
        .low_battery = false,
        .power_ok = false
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_RUN, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_ERROR, sm.previous_state);
    TEST_ASSERT_EQUAL_UINT32(100, sm.state_time_ms);

    TEST_ASSERT_EQUAL_INT(0, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(1, update_time_call_count);
}


/*
 * RUN + critical fault
 *
 * Should transition to ERROR.
 */
void test_state_machine_step_RUN_critical_fault_goes_to_ERROR(void)
{
    state_machine_t sm = create_state_machine(STATE_RUN);

    system_context_t ctx = {
        .critical_fault = true,
        .low_battery = false,
        .power_ok = false
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_ERROR, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_RUN, sm.previous_state);

    TEST_ASSERT_EQUAL_UINT32(0, sm.state_time_ms);

    TEST_ASSERT_EQUAL_INT(1, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(0, update_time_call_count);
}


/*
 * RUN + low battery
 *
 * Should transition to SAFE.
 */
void test_state_machine_step_RUN_low_battery_goes_to_SAFE(void)
{
    state_machine_t sm = create_state_machine(STATE_RUN);

    system_context_t ctx = {
        .critical_fault = false,
        .low_battery = true,
        .power_ok = false
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_SAFE, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_RUN, sm.previous_state);

    TEST_ASSERT_EQUAL_UINT32(0, sm.state_time_ms);

    TEST_ASSERT_EQUAL_INT(1, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(0, update_time_call_count);
}


/*
 * RUN + power_ok
 *
 * According to the implementation:
 *
 *     if (ctx->low_battery || ctx->power_ok)
 *         next_state = STATE_SAFE;
 *
 * Therefore power_ok causes SAFE.
 */
void test_state_machine_step_RUN_power_ok_goes_to_SAFE(void)
{
    state_machine_t sm = create_state_machine(STATE_RUN);

    system_context_t ctx = {
        .critical_fault = false,
        .low_battery = false,
        .power_ok = true
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_SAFE, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_RUN, sm.previous_state);

    TEST_ASSERT_EQUAL_UINT32(0, sm.state_time_ms);

    TEST_ASSERT_EQUAL_INT(1, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(0, update_time_call_count);
}


/*
 * RUN + critical fault + low battery
 *
 * IMPORTANT:
 * The second if() overwrites STATE_ERROR with STATE_SAFE.
 *
 * This test documents the CURRENT implementation.
 */
void test_state_machine_step_RUN_critical_fault_and_low_battery_goes_to_SAFE(void)
{
    state_machine_t sm = create_state_machine(STATE_RUN);

    system_context_t ctx = {
        .critical_fault = true,
        .low_battery = true,
        .power_ok = false
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_SAFE, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_RUN, sm.previous_state);

    TEST_ASSERT_EQUAL_UINT32(0, sm.state_time_ms);

    TEST_ASSERT_EQUAL_INT(1, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(0, update_time_call_count);
}


/*
 * ==========================================================================
 * STATE_SAFE tests
 * ==========================================================================
 */

/*
 * SAFE + low battery
 *
 * Critical fault is false and low_battery is true, so no transition.
 */
void test_state_machine_step_SAFE_low_battery_stays_SAFE(void)
{
    state_machine_t sm = create_state_machine(STATE_SAFE);

    system_context_t ctx = {
        .critical_fault = false,
        .low_battery = true,
        .power_ok = false
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_SAFE, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_ERROR, sm.previous_state);
    TEST_ASSERT_EQUAL_UINT32(100, sm.state_time_ms);

    TEST_ASSERT_EQUAL_INT(0, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(1, update_time_call_count);
}


/*
 * SAFE + battery recovered
 *
 * !low_battery causes transition back to RUN.
 */
void test_state_machine_step_SAFE_battery_recovered_goes_to_RUN(void)
{
    state_machine_t sm = create_state_machine(STATE_SAFE);

    system_context_t ctx = {
        .critical_fault = false,
        .low_battery = false,
        .power_ok = false
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_RUN, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_SAFE, sm.previous_state);

    TEST_ASSERT_EQUAL_UINT32(0, sm.state_time_ms);

    TEST_ASSERT_EQUAL_INT(1, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(0, update_time_call_count);
}


/*
 * SAFE + critical fault + low battery
 *
 * Critical fault causes ERROR.
 */
void test_state_machine_step_SAFE_critical_fault_goes_to_ERROR(void)
{
    state_machine_t sm = create_state_machine(STATE_SAFE);

    system_context_t ctx = {
        .critical_fault = true,
        .low_battery = true,
        .power_ok = false
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_ERROR, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_SAFE, sm.previous_state);

    TEST_ASSERT_EQUAL_UINT32(0, sm.state_time_ms);

    TEST_ASSERT_EQUAL_INT(1, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(0, update_time_call_count);
}


/*
 * SAFE + critical fault + battery recovered
 *
 * IMPORTANT:
 * The second if() overwrites STATE_ERROR with STATE_RUN.
 *
 * This test documents the CURRENT implementation.
 */
void test_state_machine_step_SAFE_critical_fault_and_battery_recovered_goes_to_RUN(void)
{
    state_machine_t sm = create_state_machine(STATE_SAFE);

    system_context_t ctx = {
        .critical_fault = true,
        .low_battery = false,
        .power_ok = false
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_RUN, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_SAFE, sm.previous_state);

    TEST_ASSERT_EQUAL_UINT32(0, sm.state_time_ms);

    TEST_ASSERT_EQUAL_INT(1, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(0, update_time_call_count);
}


/*
 * ==========================================================================
 * STATE_ERROR tests
 * ==========================================================================
 */

/*
 * ERROR has no transition logic currently.
 *
 * Therefore it should remain ERROR and update_time() should be called.
 */
void test_state_machine_step_ERROR_stays_ERROR(void)
{
    state_machine_t sm = create_state_machine(STATE_ERROR);

    system_context_t ctx = {
        .critical_fault = false,
        .low_battery = false,
        .power_ok = false
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_ERROR, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_ERROR, sm.previous_state);

    TEST_ASSERT_EQUAL_UINT32(100, sm.state_time_ms);

    TEST_ASSERT_EQUAL_INT(0, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(1, update_time_call_count);
}


/*
 * ERROR should remain ERROR regardless of context because there are
 * currently no transition conditions in the ERROR case.
 */
void test_state_machine_step_ERROR_ignores_context(void)
{
    state_machine_t sm = create_state_machine(STATE_ERROR);

    system_context_t ctx = {
        .critical_fault = true,
        .low_battery = true,
        .power_ok = true
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_ERROR, sm.current_state);
    TEST_ASSERT_EQUAL(STATE_ERROR, sm.previous_state);

    TEST_ASSERT_EQUAL_INT(0, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(1, update_time_call_count);
}


/*
 * ==========================================================================
 * DEFAULT / invalid state tests
 * ==========================================================================
 */

/*
 * Any unknown state should transition to ERROR.
 */
void test_state_machine_step_UNKNOWN_state_goes_to_ERROR(void)
{
    state_machine_t sm = create_state_machine((system_state_t)99);

    system_context_t ctx = {
        .critical_fault = false,
        .low_battery = false,
        .power_ok = false
    };

    state_machine_step(&sm, &ctx);

    TEST_ASSERT_EQUAL(STATE_ERROR, sm.current_state);
    TEST_ASSERT_EQUAL((system_state_t)99, sm.previous_state);

    TEST_ASSERT_EQUAL_UINT32(0, sm.state_time_ms);

    TEST_ASSERT_EQUAL_INT(1, reset_time_call_count);
    TEST_ASSERT_EQUAL_INT(0, update_time_call_count);
}


/*
 ==========================================================================
 * Test runner
 * ==========================================================================
 */

int main(void)
{
    UNITY_BEGIN();

    /* STATE_RUN */
    RUN_TEST(test_state_machine_step_RUN_no_conditions_stays_RUN);
    RUN_TEST(test_state_machine_step_RUN_critical_fault_goes_to_ERROR);
    RUN_TEST(test_state_machine_step_RUN_low_battery_goes_to_SAFE);
    RUN_TEST(test_state_machine_step_RUN_power_ok_goes_to_SAFE);
    RUN_TEST(test_state_machine_step_RUN_critical_fault_and_low_battery_goes_to_SAFE);

    /* STATE_SAFE */
    RUN_TEST(test_state_machine_step_SAFE_low_battery_stays_SAFE);
    RUN_TEST(test_state_machine_step_SAFE_battery_recovered_goes_to_RUN);
    RUN_TEST(test_state_machine_step_SAFE_critical_fault_goes_to_ERROR);
    RUN_TEST(test_state_machine_step_SAFE_critical_fault_and_battery_recovered_goes_to_RUN);

    /* STATE_ERROR */
    RUN_TEST(test_state_machine_step_ERROR_stays_ERROR);
    RUN_TEST(test_state_machine_step_ERROR_ignores_context);

    /* DEFAULT */
    RUN_TEST(test_state_machine_step_UNKNOWN_state_goes_to_ERROR);

    return UNITY_END();
}