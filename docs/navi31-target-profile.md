# Navi31 target profile

This is the first read-only target profile for the RX 7900 XTX port.  It is
based on `navi31-linux-report.txt`, collected on 2026-10-27 from a Sapphire
NITRO+ card (`1002:744c`, subsystem `1da2:e471`).

The profile is data only.  It is not consumed by the kext bring-up ladder and
does not select a register table, map a BAR, load firmware, or write the GPU.
The purpose of this stage is to give the later Navi31 implementation one
reviewable set of hardware observations and Linux-selected IP families.

Confirmed observations:

- BAR0 is currently 256 MiB; ReBAR advertises sizes through 32 GiB.
- BAR2 is currently 2 MiB.
- VRAM is `0x5ff000000` bytes (about 23.98 GiB).
- Linux enabled a 512 MiB GART with its page table at MC address
  `0x8000300000`; the GPU virtual GART range is
  `0x7fff00000000`–`0x7fff1fffffff`.
- The PSP TMR reservation starts at `0x85fc000000` and is `0x1300000` bytes.
- DMUB reports version `0x07003300`.
- The active topology includes two SDMA engines, eight compute rings, two VCN
  rings, and one JPEG ring.

The Linux probe log now confirms these IP families: `gmc_v11_0_0`,
`ih_v6_0_0`, `psp_v13_0_0`, `smu_v13_0_0`, `gfx_v11_0_0`, `sdma_v6_0_0`,
`mes_v11_0_0`, `vcn_v4_0_0`, `jpeg_v4_0_0`, and DCN 3.2.  It also reports
MES masks `vmid=0xff00`, `gfx=0x2`, `compute=0xc`, `sdma=0xfc`, SMU driver
interface `0x3d`, firmware interface `0x41`, and firmware version `0x004e8700`.

The current compressed firmware set was decompressed and hashed without
sudo:

| Firmware | Bytes | SHA-256 |
|---|---:|---|
| `gc_11_0_0_imu.bin` | 132352 | `b4f8fc056b45709a6abf48e7885fb1b4ab8d3cc092cbfa2c554a78564a6403bc` |
| `gc_11_0_0_me.bin` | 324064 | `f2f5a793d811c6abad1a18af0fcf7694c443478f224176da86650c22aa71ca7a` |
| `gc_11_0_0_mec.bin` | 406528 | `1dd1de8ecf5455ea4719c502b64b32ac18763d5601128c01b4a4a36211a122c2` |
| `gc_11_0_0_mes.bin` | 286336 | `3b30fd16d1a0df5006afab78d1117ad6b3b2bdc9c01712070ece20648d362dec` |
| `gc_11_0_0_mes1.bin` | 221120 | `d443e8304bb8737a106310ee097cc1b0a0ab3356f7076b2dd23f4ebb6bd16684` |
| `gc_11_0_0_mes_2.bin` | 263696 | `6e1926d197d9e5f17830c1bba661ac170e2c9e7aba1da56ef26c6f6978b3b79e` |
| `gc_11_0_0_pfp.bin` | 234960 | `b360393c8629144b194f69a3cd961ed509331feff7a5cc1e4eb21c901da2710a` |
| `gc_11_0_0_rlc.bin` | 185376 | `dabd49039772d02f5fd5e48dc21d35ad52a6b1283b470dabca86ca159c4c7c8e` |
| `psp_13_0_0_sos.bin` | 272400 | `4a51299f6d0a15bbba9694419f7891e6accc01dbd2dd67c06add7bfd75a45ac6` |
| `psp_13_0_0_ta.bin` | 271616 | `ac2454a36618d8568e798c6000bf3e19fdabf46d46038b92635adc3cb01bd198` |
| `sdma_6_0_0.bin` | 34560 | `82cd01a24171af12de6c7ac4ee7471aa2dfcf51f9677e7bae9cd4c75e07761ee` |
| `smu_13_0_0.bin` | 292452 | `3551e1ef1f0cf542d1a728bdff123142e7cf86b550974d229668b01804c3992d` |
| `dcn_3_2_0_dmcub.bin` | 284432 | `858ff09f406688c7cfd9ec749de37fb26d6659b078617a628c259aa86c91b0bc` |

Before enabling any write path, the port still needs an on-die IP discovery
dump, exact register offsets and masks, NBIO/MMHUB versions, and a
target-specific GMC/DCN implementation.
