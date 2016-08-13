#pragma once
#include "common/types.h"

namespace vpad
{

void
VPADInit();

void
VPADSetAccParam(uint32_t chan, float unk1, float unk2);

void
VPADSetBtnRepeat(uint32_t chan, float unk1, float unk2);

int
VPADControlMotor(uint32_t chan, uint8_t *buffer, uint32_t size);

} // namespace vpad
