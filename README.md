# PerfTest

A simple GPU shader memory operation performance test tool. Current implementation is DirectX 11.0 based.

## Features

Designed to measure performance of various types of buffer and image loads. This application is not a GPU memory bandwidth benchmark tool. All tests operate inside GPUs L1 caches (no larger than 16 KB working sets). 

- Coalesced loads (100% L1 cache hit). All threads (in group of 256 threads) access linearly increasing memory addresses.
- Random loads (100% L1 cache hit). Thread's start address is randomized. After that thread loads addresses linearly.
- Typed SRVs: 1/2/4 channels, 8/16/32 bits per channel (2d missing)

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

**TODO:** Add AMD, NV & Intel results here. Key differences + some graphs.

**Preliminary results:**
```markdown
**Nvidia GTX 980**
Load R8 linear: 1.637ms
Load R8 random: 2.606ms
Load RG8 linear: 1.630ms
Load RG8 random: 2.619ms
Load RGBA8 linear: 1.634ms
Load RGBA8 random: 2.592ms
Load R16f linear: 1.855ms
Load R16f random: 2.617ms
Load RG16f linear: 1.632ms
Load RG16f random: 2.593ms
Load RGBA16f linear: 1.634ms
Load RGBA16f random: 2.543ms
Load R32f linear: 1.631ms
Load R32f random: 2.595ms
Load RG32f linear: 1.635ms
Load RG32f random: 2.544ms
Load RGBA32f linear: 1.632ms
Load RGBA32f random: 2.640ms
```

## License

PerfTest is released under the MIT license. See [LICENSE.md](LICENSE.md) for full text.
