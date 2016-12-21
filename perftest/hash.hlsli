// Simple Weyl hash function
// Source: https://gist.github.com/Marc-B-Reynolds/5a939f71fc7237c7af63
uint hash2(uint2 c)
{
  c.x *= 0x3504f333; 
  c.y *= 0xf1bbcdcb;
  c.x ^= c.y;
  c.x *= 741103597;
  return c.x;
}

// Modified from the above. Only single channel
uint hash1(uint c)
{
  c *= 0x3504f333; 
  return c;
}

