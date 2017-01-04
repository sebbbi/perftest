# PerfTest

A simple GPU shader memory operation performance test tool. Current implementation is DirectX 11.0 based.

## Features

Designed to measure performance of various types of buffer and image loads. This application is not a GPU memory bandwidth benchmark tool. All tests operate inside GPUs L1 caches (no larger than 16 KB working sets). 

- Random loads (100% L1 cache hit)
- Coalesced loads (100% L1 cache hit)
- Typed SRVs: 1/2/4 channels, 8/16/32 bits per channel

## Todo list

- 32 bit byte address SRVs (load, load2, load4)
- Coherent loads (constant address)
- Coherent loads (SV_GroupID)
- Constant buffer loads (both constant address and indexed)
- Texture loads (1d/2d/3d)
- Extended format support (uint/unorm/float of all widths)

## Wish list

- Texture sampling (1d/2d/3d)
- Extended format support v2 (R10G10B10, R11G11B10f, etc)
- Measure write performance
- Port to Vulkan and/or DX12
- Measure upload heap load performance

## License

PerfTest is released under the MIT license. See [LICENSE.md](LICENSE.md) for full text.
