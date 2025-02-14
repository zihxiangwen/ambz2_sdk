/*
 * OS specific functions
 * Copyright (c) 2005-2009, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef ROM_WPS_OS_H
#define ROM_WPS_OS_H

#if defined(CONFIG_PLATFORM_8195A) || defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D)

#include <rom_wlan_ram_map.h>
extern struct _rom_wlan_ram_map rom_wlan_ram_map;
#define os_malloc(sz) rom_wlan_ram_map.rtw_malloc(sz)
#define os_free(p, sz) rom_wlan_ram_map.rtw_mfree(((uint8_t*)(p)), (sz))

#endif

extern uint8_t *WPS_realloc(uint8_t *old_buf, uint32_t old_sz, uint32_t new_sz);
#define os_realloc(p, os, ns) WPS_realloc(((uint8_t*)(p)),(os),(ns))

#endif /* ROM_WPS_OS_H */
