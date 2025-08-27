// Src/aton_user_io_wrap.c  (FINAL)
#include <stdint.h>
#include "ll_aton_NN_interface.h"
#include "ll_aton_runtime.h"

/* Tell the linker we want the real symbols too */
extern void* __real_LL_ATON_Get_User_Input_Buffer_Default(uint32_t num);
extern void* __real_LL_ATON_Get_User_Output_Buffer_Default(uint32_t num);

/* -------- INPUT wrappers -------- */
/* Make Set() fail so the app will memcpy into the default buffer. */
LL_ATON_User_IO_Result_t __wrap_LL_ATON_Set_User_Input_Buffer_Default(uint32_t num, void* buffer, uint32_t size)
{
  (void)num; (void)buffer; (void)size;
  return LL_ATON_User_IO_WRONG_INDEX;   /* force copy path in app */
}

/* Return the ACTUAL default buffer pointer from the runtime. */
void* __wrap_LL_ATON_Get_User_Input_Buffer_Default(uint32_t num)
{
  return __real_LL_ATON_Get_User_Input_Buffer_Default(num);
}

/* -------- OUTPUT wrappers -------- */
/* Accept user output buffers so the runtime exports directly into them. */
LL_ATON_User_IO_Result_t __wrap_LL_ATON_Set_User_Output_Buffer_Default(uint32_t num, void* buffer, uint32_t size)
{
  (void)num; (void)buffer; (void)size;
  return LL_ATON_User_IO_NOERROR;
}

/* For completeness, forward Get() to the real runtime (rarely used now). */
void* __wrap_LL_ATON_Get_User_Output_Buffer_Default(uint32_t num)
{
  return __real_LL_ATON_Get_User_Output_Buffer_Default(num);
}
