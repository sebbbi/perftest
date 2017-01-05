# PerfTest

A simple GPU shader memory operation performance test tool. Current implementation is DirectX 11.0 based.

## Features

Designed to measure performance of various types of buffer and image loads. This application is not a GPU memory bandwidth benchmark tool. All tests operate inside GPUs L1 caches (no larger than 16 KB working sets). 

- Coalesced loads (100% L1 cache hit). All threads (in group of 256 threads) access linearly increasing memory addresses.
- Random loads (100% L1 cache hit). Thread's start address is randomized. After that thread loads addresses linearly.
- Invariant loads (same address for all threads)
- Typed SRVs: 1/2/4 channels, 8/16/32 bits per channel
- 32 bit byte address SRVs (load, load2, load4)

## Todo list

- Better output (elements/s or bytes/s, etc)
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

**Nvidia GTX 980**
```markdown
TODO
```

**AMD Radeon 7970 GE (GCN1)**
```markdown
Load R8 invariant: 0.541ms
Load R8 linear: 0.539ms
Load R8 random: 2.121ms
Load RG8 linear: 2.386ms
Load RG8 linear: 2.386ms
Load RG8 random: 2.386ms
Load RGBA8 linear: 2.121ms
Load RGBA8 linear: 2.121ms
Load RGBA8 random: 2.121ms
Load R16f invariant: 0.536ms
Load R16f linear: 0.537ms
Load R16f random: 2.121ms
Load RG16f invariant: 2.386ms
Load RG16f linear: 2.385ms
Load RG16f random: 2.385ms
Load RGBA16f invariant: 2.122ms
Load RGBA16f linear: 2.121ms
Load RGBA16f random: 2.121ms
Load R32f invariant: 0.536ms
Load R32f linear: 0.537ms
Load R32f random: 2.121ms
Load RG32f invariant: 2.385ms
Load RG32f linear: 2.385ms
Load RG32f random: 2.385ms
Load RGBA32f invariant: 2.121ms
Load RGBA32f linear: 2.121ms
Load RGBA32f random: 2.385ms
Load1 raw32 invariant: 0.607ms
Load1 raw32 linear: 0.679ms
Load1 raw32 random: 2.122ms
Load2 raw32 invariant: 0.615ms
Load2 raw32 linear: 2.386ms
Load2 raw32 random: 2.386ms
Load4 raw32 invariant: 1.078ms
Load4 raw32 linear: 2.128ms
Load4 raw32 random: 2.371ms
```

Typed loads: AMD GCN1 coalesces 1d typed loads only (all formats). Coalesced load performance is 4x. Both linear access pattern (next thread in wave always addresses next element) and invariant access (each thread in wave access same location) coalesce. Wide loads (4d) and wide formats (32 bit) do not cause any slow down. Widest (RGBA32) loads thus offer best bytes / cycle rate.

Raw (ByteAddressBuffer) loads: Similar to typed loads. 1d formats coalesce perfectly (4x) on lineat access. Invariant access generates scalar unit loads on GCN1 (separate cache + stored to SGPR -> reduced register & cache pressure & doesn't stress vector load path). Scalar 1d/2d load seems to match coalesced vector load in performance. 4d invariant case is slightly slower, but still 2x faster than linear vector load. Mixed vector + scalar load would be fastest (but I don't have test case for it yet).

## License

PerfTest is released under the MIT license. See [LICENSE.md](LICENSE.md) for full text.
