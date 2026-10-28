/*
 * navi31_target.h - first-pass, read-only target profile for Navi 31.
 *
 * This header deliberately contains data only.  Nothing here selects a
 * register table, maps a BAR, loads firmware, or writes the GPU.  The profile
 * is the boundary between the Linux inventory work and a future Navi31
 * bring-up implementation.
 *
 * Evidence source: navi31-linux-report.txt collected on 2026-10-27 from a
 * Sapphire NITRO+ RX 7900 XTX (PCI 1002:744c, subsystem 1da2:e471).
 * IP families below are confirmed by the current Linux amdgpu probe log;
 * on-die IP discovery base addresses and NBIO/MMHUB versions are still TODO.
 */
#pragma once

#include <stdint.h>

#include "amdgpu_ip.h"

namespace amdgpu {

enum class TargetKind : uint8_t {
    Navi31,
};

struct TargetIpFamily {
    IPVersion version;
    bool inferred_from_firmware;
};

struct Navi31FirmwareNames {
    const char *gc_imu;
    const char *gc_me;
    const char *gc_mec;
    const char *gc_mes;
    const char *gc_mes1;
    const char *gc_mes_2;
    const char *gc_pfp;
    const char *gc_rlc;
    const char *psp_sos;
    const char *psp_ta;
    const char *sdma;
    const char *smu;
    const char *dcn_dmcub;
};

struct Navi31ObservedGeometry {
    // PCI identity from lspci/sysfs.
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t subsystem_vendor_id;
    uint16_t subsystem_device_id;
    uint8_t revision;

    // Current sizes from the Physical Resizable BAR capability.
    uint64_t bar0_current_bytes;
    uint64_t bar0_supported_max_bytes;
    uint64_t bar2_current_bytes;

    // Runtime memory and engine observations from amdgpu.
    uint64_t vram_bytes;
    uint64_t gtt_bytes;
    uint64_t gart_bytes;
    uint64_t gart_gpu_va_base;
    uint64_t gart_gpu_va_end;
    uint64_t gart_table_mc;
    uint64_t psp_tmr_base_mc;
    uint64_t psp_tmr_bytes;
    uint32_t dmub_version;
    uint32_t mes_vmid_mask_mmhub;
    uint32_t mes_vmid_mask_gfxhub;
    uint32_t mes_gfx_hqd_mask;
    uint32_t mes_compute_hqd_mask;
    uint32_t mes_sdma_hqd_mask;
    uint32_t smu_driver_if_version;
    uint32_t smu_fw_if_version;
    uint32_t smu_fw_version;
    uint8_t sdma_engines;
    uint8_t compute_rings;
    uint8_t vcn_rings;
    uint8_t jpeg_rings;
};

struct Navi31TargetProfile {
    TargetKind kind;
    const char *name;
    const char *marketing_name;

    // These are the IP families selected by the Linux amdgpu probe log.
    TargetIpFamily gc;
    TargetIpFamily gmc;
    TargetIpFamily ih;
    TargetIpFamily dcn;
    TargetIpFamily dce;
    TargetIpFamily sdma;
    TargetIpFamily psp;
    TargetIpFamily smu;
    TargetIpFamily mes;
    TargetIpFamily vcn;
    TargetIpFamily jpeg;

    Navi31FirmwareNames firmware;
    Navi31ObservedGeometry observed;
};

inline constexpr Navi31TargetProfile kNavi31TargetProfile = {
    TargetKind::Navi31,
    "navi31",
    "Radeon RX 7900 XTX",

    { { 11, 0, 0 }, false }, // GC / gfx_v11_0_0
    { { 11, 0, 0 }, false }, // GMC / gmc_v11_0_0
    { {  6, 0, 0 }, false }, // IH / ih_v6_0_0
    { {  3, 2, 0 }, false }, // DCN 3.2
    { {  1, 0, 0 }, false }, // DCE wrapper / dce_v1_0_0
    { {  6, 0, 0 }, false }, // SDMA / sdma_v6_0_0
    { { 13, 0, 0 }, false }, // PSP / psp_v13_0_0
    { { 13, 0, 0 }, false }, // SMU / smu_v13_0_0
    { { 11, 0, 0 }, false }, // MES / mes_v11_0_0
    { {  4, 0, 0 }, false }, // VCN / vcn_v4_0_0
    { {  4, 0, 0 }, false }, // JPEG / jpeg_v4_0_0

    {
        "gc_11_0_0_imu",
        "gc_11_0_0_me",
        "gc_11_0_0_mec",
        "gc_11_0_0_mes",
        "gc_11_0_0_mes1",
        "gc_11_0_0_mes_2",
        "gc_11_0_0_pfp",
        "gc_11_0_0_rlc",
        "psp_13_0_0_sos",
        "psp_13_0_0_ta",
        "sdma_6_0_0",
        "smu_13_0_0",
        "dcn_3_2_0_dmcub",
    },

    {
        0x1002u,       // vendor
        0x744cu,       // Navi 31: RX 7900 XT/XTX/GRE/7900M
        0x1da2u,       // Sapphire
        0xe471u,       // NITRO+ RX 7900 XTX Vapor-X
        0xc8u,

        256ull << 20,  // current BAR0
        32ull << 30,   // largest BAR0 size advertised by ReBAR
        2ull << 20,    // current BAR2

        0x5ff000000ull,
        0x3e9169000ull,
        512ull << 20,
        0x7fff00000000ull,
        0x7fff1fffffffull,
        0x8000300000ull,
        0x85fc000000ull,
        0x01300000ull,
        0x07003300u,
        0x0000ff00u,
        0x0000ff00u,
        0x00000002u,
        0x0000000cu,
        0x000000fcu,
        0x0000003du,
        0x00000041u,
        0x004e8700u,
        2u,            // sdma0, sdma1
        8u,            // comp_1.0.0 .. comp_1.3.1
        2u,            // vcn_unified_0, vcn_unified_1
        1u,            // jpeg_dec
    },
};

constexpr bool navi31_pci_match(uint16_t vendor_id, uint16_t device_id) {
    return vendor_id == kNavi31TargetProfile.observed.vendor_id &&
           device_id == kNavi31TargetProfile.observed.device_id;
}

constexpr bool navi31_geometry_is_sane(const Navi31ObservedGeometry &g) {
    return g.vendor_id == 0x1002u &&
           g.device_id == 0x744cu &&
           g.bar0_current_bytes >= (256ull << 20) &&
           g.bar0_supported_max_bytes >= g.bar0_current_bytes &&
           g.bar2_current_bytes >= (2ull << 20) &&
           g.vram_bytes >= (23ull << 30) &&
           g.gart_bytes == (512ull << 20) &&
           g.gart_gpu_va_end >= g.gart_gpu_va_base &&
           g.gart_gpu_va_end - g.gart_gpu_va_base + 1ull == g.gart_bytes &&
           g.gart_table_mc != 0 &&
           g.psp_tmr_base_mc != 0 &&
           g.psp_tmr_bytes != 0 &&
           g.dmub_version != 0 &&
           g.mes_sdma_hqd_mask != 0 &&
           g.smu_driver_if_version != 0 &&
           g.smu_fw_if_version != 0 &&
           g.smu_fw_version != 0 &&
           g.sdma_engines == 2u;
}

static_assert(navi31_pci_match(0x1002u, 0x744cu), "Navi31 PCI identity");
static_assert(!navi31_pci_match(0x1002u, 0x7550u), "Navi48 is not Navi31");
static_assert(navi31_geometry_is_sane(kNavi31TargetProfile.observed),
              "Navi31 inventory geometry must remain internally consistent");

} // namespace amdgpu
