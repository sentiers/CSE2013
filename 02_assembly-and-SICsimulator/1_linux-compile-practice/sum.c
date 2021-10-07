// 2019113581 조현지

long plus (long x, long y);

void sumstore(long x, long y, long* dest)
{
  long t = plus(x, y);
  *dest = t;
}
