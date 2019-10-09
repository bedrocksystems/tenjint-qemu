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

#ifndef _ASM_X86_KVM_VMI_H
#define _ASM_X86_KVM_VMI_H

#include <linux/types.h>
#include <linux/perf_event.h>

#define MAX_LBR_ENTRIES                   32

#define KVM_VMI_FEATURE_TRAP_TASK_SWITCH  0
#define KVM_VMI_FEATURE_LBR               1
#define KVM_VMI_FEATURE_MTF               2
#define KVM_VMI_FEATURE_SLP               3
#define KVM_VMI_FEATURE_DEBUG             4
#define KVM_VMI_FEATURE_MAX               5

#define KVM_VMI_EVENT_TASK_SWITCH         0
#define KVM_VMI_EVENT_DEBUG               1
#define KVM_VMI_EVENT_MTF                 2
#define KVM_VMI_EVENT_SLP                 3

struct kvm_vmi_feature_task_switch {
	__u32 feature;
	__u8 enable;
	__u64 dtb;
	__u8 incoming;
	__u8 outgoing;
};

struct kvm_vmi_feature_lbr {
	__u32 feature;
	__u8 enable;
	__u64 lbr_select;
};

struct kvm_vmi_feature_mtf {
	__u32 feature;
	__u8  enable;
};

struct kvm_vmi_feature_slp {
	__u32 feature;
	__u8 enable;
	__u8 global_req;
	__u64 gfn;
	__u64 num_pages;
	__u64 violation;
};

struct kvm_vmi_feature_debug {
	__u32 feature;
	__u8 enable;
	__u8 single_step;
	__u8 watchpoint;
	__u64 addr;
};

union kvm_vmi_feature {
	__u32 feature;
	struct kvm_vmi_feature_task_switch ts;
	struct kvm_vmi_feature_lbr lbr;
	struct kvm_vmi_feature_mtf mtf;
	struct kvm_vmi_feature_slp slp;
	struct kvm_vmi_feature_debug debug;
};

struct kvm_vmi_event_task_switch {
	__u32 type;
	__u32 cpu_num;
	__u64 old_cr3;
	__u64 new_cr3;
};

struct kvm_vmi_event_debug {
	__u32 type;
	__u32 cpu_num;
	__u8 single_step;
	__u8 watchpoint;
	__u64 watchpoint_gva;
	__s32 watchpoint_flags;
	__u64 breakpoint_gva;
	__u64 breakpoint_gpa;
};

struct kvm_vmi_event_mtf {
	__u32 type;
	__u32 cpu_num;
};

struct kvm_vmi_event_slp {
	__u32 type;
	__u32 cpu_num;
	__u64 violation;
	__u64 gva;
	__u64 gpa;
	__u8 rwx;
};

union kvm_vmi_event {
	__u32 type;
	struct kvm_vmi_event_task_switch ts;
	struct kvm_vmi_event_debug debug;
	struct kvm_vmi_event_mtf mtf;
	struct kvm_vmi_event_slp slp;
};

struct kvm_vmi_lbr_info {
	__u32 entries;
	__u8 tos;
	__u64 lbr_from[MAX_LBR_ENTRIES];
	__u64 lbr_to[MAX_LBR_ENTRIES];
};

#define KVM_VMI_GET_LBR           _IOW(KVMIO, 0xf8, struct kvm_vmi_lbr_info)

#endif /* _ASM_X86_KVM_VMI_H */


