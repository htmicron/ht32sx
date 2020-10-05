/**
  *
  * Copyright (c) 2020 HT Micron Semicondutors S.A.
	* Licensed under the Apache License, Version 2.0 (the "License");
	* you may not use this file except in compliance with the License.
	* You may obtain a copy of the License at
	* http://www.apache.org/licenses/LICENSE-2.0
	* Unless required by applicable law or agreed to in writing, software
	* distributed under the License is distributed on an "AS IS" BASIS,
	* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	* See the License for the specific language governing permissions and
	* limitations under the License.
  *
*/

#include "HT_P2P_s2lp_util.h"

static uint8_t s_RfModuleBand = 0;
static int32_t s_RfModuleOffset=0;

void S2LPManagementSetBand(uint8_t value) {
	s_RfModuleBand = value;
}

uint8_t S2LPManagementGetBand(void) {
	return s_RfModuleBand;
}

void S2LPManagementSetOffset(int32_t value) {
	s_RfModuleOffset=value;
}

int32_t S2LPManagementGetOffset(void) {
	return s_RfModuleOffset;
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
