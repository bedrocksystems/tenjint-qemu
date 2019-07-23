/*
 * KVM VMI support
 *
 * Copyright (C) 2017 FireEye, Inc. All Rights Reserved.
 *
 * Authors:
 *  Jonas Pfoh      <jonas.pfoh@fireeye.com>
 *  Sebastian Vogl  <sebastian.vogl@fireeye.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef __LINUX_KVM_VMI_H
#define __LINUX_KVM_VMI_H

#include <linux/ioctl.h>
#include <linux/kvm.h>
#include <asm/kvm_vmi.h>

#define KVM_VMI_FEATURE_UPDATE    _IOW(KVMIO, 0xf0, union kvm_vmi_feature)

#define KVM_EXIT_VMI_EVENT        1337
#endif /* __LINUX_KVM_VMI_H */


