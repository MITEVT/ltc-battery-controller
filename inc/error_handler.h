#ifndef _ERROR_HANDLER_H
#define _ERROR_HANDLER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef FSAE_DRIVERS

#define __ERROR_NAMES C(ERROR_LTC6804_PEC)              \
                    C(ERROR_LTC6804_CVST)               \
                    C(ERROR_LTC6804_OWT)                \
                    C(ERROR_EEPROM)                     \
                    C(ERROR_CELL_UNDER_VOLTAGE)         \
                    C(ERROR_CELL_OVER_VOLTAGE)          \
                    C(ERROR_CELL_UNDER_TEMP)            \
                    C(ERROR_CELL_OVER_TEMP)             \
                    C(ERROR_OVER_CURRENT)               \
                    C(ERROR_BRUSA)                      \
                    C(ERROR_CAN)                        \
                    C(ERROR_CONFLICTING_MODE_REQUESTS)  \
                    C(ERROR_VCU_DEAD)                   \
                    C(ERROR_CONTROL_FLOW)               \
                    C(ERROR_MAIN_HB)                    \
                    C(ERROR_NOMINAL_SHUTDOWN)

#else
#define __ERROR_NAMES C(ERROR_LTC6804_PEC)              \
                    C(ERROR_LTC6804_CVST)               \
                    C(ERROR_LTC6804_OWT)                \
                    C(ERROR_EEPROM)                     \
                    C(ERROR_CELL_UNDER_VOLTAGE)         \
                    C(ERROR_CELL_OVER_VOLTAGE)          \
                    C(ERROR_CELL_OVER_TEMP)             \
                    C(ERROR_OVER_CURRENT)               \
                    C(ERROR_BRUSA)                      \
                    C(ERROR_CAN)                        \
                    C(ERROR_CONFLICTING_MODE_REQUESTS)  \
                    C(ERROR_MAIN_HB)                    \
                    C(ERROR_NOMINAL_SHUTDOWN)
#endif

#define C(x) x,

typedef enum error{
    __ERROR_NAMES ERROR_NUM_ERRORS
} ERROR_T;    
#undef C
#define C(x) #x,
static const char * const ERROR_NAMES[ERROR_NUM_ERRORS] = { __ERROR_NAMES};       


// typedef enum error {
//     ERROR_LTC6804_PEC,
//     ERROR_LTC6804_CVST,
//     ERROR_LTC6804_OWT,
//     ERROR_EEPROM,
//     ERROR_CELL_UNDER_VOLTAGE,
//     ERROR_CELL_OVER_VOLTAGE,
// #ifdef FSAE_DRIVERS
//     ERROR_CELL_UNDER_TEMP,
// #endif
//     ERROR_CELL_OVER_TEMP,
//     ERROR_OVER_CURRENT,
//     ERROR_BRUSA,
//     ERROR_CAN,
//     ERROR_CONFLICTING_MODE_REQUESTS,
// #ifdef FSAE_DRIVERS
//     ERROR_VCU_DEAD,
//     ERROR_CONTROL_FLOW,
// #endif
//     ERROR_MAIN_HB,
//     ERROR_NUM_ERRORS
// } ERROR_T;

#define ERROR_NO_ERRORS ERROR_NUM_ERRORS

// static const char * const ERROR_NAMES[ERROR_NUM_ERRORS] = {
//     "ERROR_LTC6804_PEC",
//     "ERROR_LTC6804_CVST",
//     "ERROR_LTC6804_OWT",
//     "ERROR_EEPROM",
//     "ERROR_CELL_UNDER_VOLTAGE",
//     "ERROR_CELL_OVER_VOLTAGE",
// #ifdef FSAE_DRIVERS
//     "ERROR_CELL_UNDER_TEMP",
// #endif
//     "ERROR_CELL_OVER_TEMP",
//     "ERROR_OVER_CURRENT",
//     "ERROR_BRUSA", // [TODO] Remove for FSAE
//     "ERROR_CAN",
//     "ERROR_CONFLICTING_MODE_REQUESTS"
// #ifdef FSAE_DRIVERS
//     ,"ERROR_VCU_DEAD"
//     ,"ERROR_CONTROL_FLOW"
// #endif
//     ,"ERROR_MAIN_HB"
// };

typedef enum hbeats {
    HBEAT_DI = (int)ERROR_NUM_ERRORS,
    HBEAT_MI
} HBEAT_T;

// [TODO] a use the HB errors
// on every HB, clear then create an HB error
// error out if error time is reater than threshold
// warning if time is less than threshold

static const char * const ERROR__HB_NAMES[ERROR_NUM_ERRORS] = {
    "HBEAT_DI",
    "HBEAT_MI"
};

typedef enum error_handler_status {
    HANDLER_FINE,
    HANDLER_HALT
} ERROR_HANDLER_STATUS_T;

typedef struct error_status {
    bool        handling;
    bool        error;
    uint16_t    count;
    uint32_t    time_stamp;
} ERROR_STATUS_T;

typedef  ERROR_HANDLER_STATUS_T (*ERROR_HANDLER_FUNC)(ERROR_STATUS_T* , uint32_t*, const uint32_t);

typedef struct ERROR_HANDLER {
	ERROR_HANDLER_FUNC handler;
    ERROR_HANDLER_FUNC cleanup;
	const uint32_t timeout;
} ERROR_HANDLER;



void Error_Init(void);
void Error_Assert(ERROR_T er_t, uint32_t msTicks);
void Error_Pass(ERROR_T er_t);
void Error_HB_rx(ERROR_T hb, uint32_t msTicks);

const ERROR_STATUS_T *  Error_GetStatus(ERROR_T er_t);
bool Error_ShouldHalt(ERROR_T er_t, uint32_t *msTicks);

ERROR_T Error_ShouldHalt_Status(uint32_t msTicks);

ERROR_HANDLER_STATUS_T Error_Handle(uint32_t *msTicks);

#endif
