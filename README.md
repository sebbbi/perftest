# PerfTest

A simple GPU shader memory operation performance test tool. Current implementation is DirectX 11.0 based.

## Features

Designed to measure performance of various types of buffer and image loads. This application is not a GPU memory bandwidth benchmark tool. All tests operate inside GPUs L1 caches (no larger than 16 KB working sets). 

- Coalesced loads (100% L1 cache hit). All threads (in group of 256 threads) access linearly increasing memory addresses.
- Random loads (100% L1 cache hit). Thread's start address is randomized. After that thread loads addresses linearly.
- Typed SRVs: 1/2/4 channels, 8/16/32 bits per channel

## Todo list

- Elements/s and bytes/s output (easier to read & compare results)
- 32 bit byte address SRVs (load, load2, load4)
- Coherent loads (constant address & SV_GroupID)
- Constant buffer loads (both constant address and indexed)
- Texture loads (1d/2d/3d)
- Extended format support (uint/unorm/float of all widths)

## Wish list

- Texture sampling (1d/2d/3d)
- Extended format support v2 (R10G10B10, R11G11B10f, etc)
- Measure write performance
- Port to Vulkan and/or DX12
- Measure upload heap load performance

## Results

**TODO:** Add comprehensive AMD, NV & Intel results here. Add some graphs. Add percentage comparison and/or ops per cycle.

**Preliminary results:**
```markdown
**Nvidia GTX 980**
TODO
```

```markdown
**AMD Radeon 7970 GE (GCN1)**
Load R8 invariant: 0.690ms
Load R8 linear: 0.791ms
Load R8 random: 2.125ms
Load RG8 linear: 2.120ms
Load RG8 linear: 2.120ms
Load RG8 random: 2.120ms
Load RGBA8 linear: 2.120ms
Load RGBA8 linear: 2.120ms
Load RGBA8 random: 2.120ms
Load R16f invariant: 0.668ms
Load R16f linear: 0.772ms
Load R16f random: 2.120ms
Load RG16f invariant: 2.120ms
Load RG16f linear: 2.120ms
Load RG16f random: 2.120ms
Load RGBA16f invariant: 2.120ms
Load RGBA16f linear: 2.120ms
Load RGBA16f random: 2.120ms
Load R32f invariant: 0.669ms
Load R32f linear: 0.775ms
Load R32f random: 2.120ms
Load RG32f invariant: 2.120ms
Load RG32f linear: 2.120ms
Load RG32f random: 2.120ms
Load RGBA32f invariant: 2.120ms
Load RGBA32f linear: 2.120ms
Load RGBA32f random: 2.385ms

AMD GCN1 coalesces 1d typed reads only. Linear access pattern (next thread in wave always addresses next element) and invariant access (each thread in wave access same location) coalesce. Coalesced load performance is around 3x. Wide loads (4d) and wide formats (32 bit) do not cause slow down. Widest (RGBA32) loads thus offer best bytes / cycle rate.
```

## License

PerfTest is released under the MIT license. See [LICENSE.md](LICENSE.md) for full text.
