// Host-only checks for the first-pass Navi31 target profile.

#include "../src/amd/navi31_target.h"

#include <cassert>
#include <cstdio>
#include <cstring>

int main() {
    const amdgpu::Navi31TargetProfile &p = amdgpu::kNavi31TargetProfile;
    const amdgpu::Navi31ObservedGeometry &g = p.observed;

    assert(std::strcmp(p.name, "navi31") == 0);
    assert(amdgpu::navi31_pci_match(g.vendor_id, g.device_id));
    assert(!amdgpu::navi31_pci_match(0x1002u, 0x7550u));
    assert(g.vram_bytes == 0x5ff000000ull);
    assert(g.gart_bytes == (512ull << 20));
    assert(g.gart_gpu_va_base == 0x7fff00000000ull);
    assert(g.gart_gpu_va_end == 0x7fff1fffffffull);
    assert(g.gart_table_mc == 0x8000300000ull);
    assert(g.psp_tmr_base_mc == 0x85fc000000ull);
    assert(g.dmub_version == 0x07003300u);
    assert(g.mes_vmid_mask_mmhub == 0x0000ff00u);
    assert(g.mes_gfx_hqd_mask == 0x2u);
    assert(g.mes_compute_hqd_mask == 0xcu);
    assert(g.mes_sdma_hqd_mask == 0xfcu);
    assert(g.smu_driver_if_version == 0x3du);
    assert(g.smu_fw_if_version == 0x41u);
    assert(g.smu_fw_version == 0x004e8700u);
    assert(p.gc.version.major == 11u && !p.gc.inferred_from_firmware);
    assert(p.gmc.version.major == 11u && !p.gmc.inferred_from_firmware);
    assert(p.dcn.version.major == 3u && p.dcn.version.minor == 2u);
    assert(p.vcn.version.major == 4u && p.jpeg.version.major == 4u);
    assert(std::strcmp(p.firmware.gc_imu, "gc_11_0_0_imu") == 0);
    assert(std::strcmp(p.firmware.psp_sos, "psp_13_0_0_sos") == 0);
    assert(std::strcmp(p.firmware.dcn_dmcub, "dcn_3_2_0_dmcub") == 0);

    std::printf("navi31 profile: PCI %04x:%04x, VRAM %#llx, GART %#llx, DMUB %#x\n",
                g.vendor_id, g.device_id,
                (unsigned long long)g.vram_bytes,
                (unsigned long long)g.gart_bytes,
                g.dmub_version);
    return 0;
}
